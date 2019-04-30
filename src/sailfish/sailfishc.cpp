#include "sailfishc.h"

std::string
builtinTypesTranslator(std::string type)
{
    if (type == "int")
        return "int";

    if (type == "flt")
        return "float";

    if (type == "str")
        return "char*";

    if (type == "bool")
        return "int";

    if (type == "[int]")
        return "int*";

    if (type == "[flt]")
        return "float*";

    if (type == "[str]")
        return "char**";

    if (type == "[bool]")
        return "int*";

    if (type == "own")
        return "this";

    else
        return type;
}

UdtFlagAndBufer
parseFile(const std::string& filename, bool shouldDisplayErrors)
{
    try
    {
        sailfishc* sfc = new sailfishc(filename, shouldDisplayErrors);
        sfc->parse();
        return std::make_tuple(std::move(sfc->getUDTTable()),
                               sfc->getIsUDTFlag(),
                               sfc->getTranspiler()->getBuffer());
    }
    catch (const std::string msg)
    {
        throw msg; // propogate message up
    }
    catch (char const* msg)
    {
        throw msg; // propogate message up
    }
}

// given a filename, extracts the udt name since udt's are named by the file in
// which they are defined
std::string
extractUDTName(const std::string& s)
{
    auto front = s.find_last_of('/') + 1;
    auto back = s.find_last_of('.') - front;
    return s.substr(front, back);
}

// -------- Parser Helper Code --------- //

std::string
extractListType(const std::string& s)
{
    return s.substr(1, s.size() - 2);
}

void
sailfishc::advanceAndCheckToken(const TokenKind& k)
{
    // first check value and kind
    if (currentToken->kind != k)
        errorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Expected a token of type: " + displayKind(k),
                  "Received: ", currentToken->value,
                  " of type " + displayKind(currentToken->kind) + ".")));

    advanceToken();
}

void
sailfishc::advanceToken()
{
    currentToken = lexar->getNextToken();

    // catch errors from the lexar
    if (currentToken->kind == TokenKind::ERROR)
        errorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line, "Lexar Error.",
                  "Error: ", currentToken->value, "")));

    while (currentToken->kind == TokenKind::COMMENT ||
           currentToken->kind == TokenKind::COMMA)
    {
        currentToken = lexar->getNextToken();

        // catch errors from the lexar
        if (currentToken->kind == TokenKind::ERROR)
            errorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line, "Lexar Error.",
                      "Error: ", currentToken->value, "")));
    }
}

// -------- Semantic Analysis Helper Code --------- //
void
sailfishc::checkType(const std::string& t0, const std::string& t1)
{
    // convert left and right to type of var if they are vars
    auto left =
        symboltable->hasVariable(t0) ? symboltable->getSymbolType(t0) : t0;
    auto right =
        symboltable->hasVariable(t1) ? symboltable->getSymbolType(t1) : t1;

    // if either is a udt, make sure we check udt type, not U which is returned
    // by symbol table
    left = left == "U" && udttable->hasUDT(t0) ? t0 : left;
    right = right == "U" && udttable->hasUDT(t1) ? t1 : right;

    // adjust for own
    if (left == "own")
        left = extractUDTName(filename);
    if (right == "own")
        right = extractUDTName(filename);

    // should actually check types here

    // edge case lists
    if (left.size() > 0 && right.size() > 0 &&
        (left.at(0) == '[' || right.at(0) == '['))
    {
        // if one is a list, both must be lists
        if (left.at(0) == '[')
            left = extractListType(left);

        if (right.at(0) == '[')
            right = extractListType(right);

        if (t0 != "none" && t1 != "none" && left != right)
            semanticerrorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Mismatched list types. Expected is: [" + left + "].",
                      "Received is: ", "[" + right + "]", ".")));
    }

    // edge case left is num and thus right can be either int or flt type
    else if (left == "num")
    {
        if ("int" != right && "flt" != right)
            semanticerrorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Mismatched types. Expected/LeftHand is: int or flt.",
                      "Received/Right Hand is: ", right, ".")));
    }

    // normal error check, with edge case that right is an empty
    else if (left != right && right != "empty")
        semanticerrorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Mismatched types. Expected/LeftHand is: " + t0 + ".",
                  "Received/Right Hand is: ", right, ".")));
}

bool
isPrimitive(const std::string& s)
{
    return s == "int" || s == "flt" || s == "void" || s == "bool" || s == "str";
}

void
sailfishc::checkUnique(const std::string& s)
{
    if (symboltable->hasVariable(s))
        semanticerrorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line, "Illegal redeclaration.",
            "Redeclared variable named: ", s, ".")));
}

void
sailfishc::checkExists(const std::string& s)
{

    auto type = s;
    if (type == "own")
    {
        if (isUdt)
            type = extractUDTName(filename);
        else
            errorhandler->handle(std::make_unique<Error>(Error(
                currentToken->col, currentToken->line,
                "illegal usage of own in a non udt method.", "", "", "")));
    }

    if (s.at(0) == '[')
        type = extractListType(type);

    if (!symboltable->hasVariable(type) && !isPrimitive(type) &&
        !udttable->hasUDT(type))
        semanticerrorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line, "Unknown variable or type.",
            "Unknown variable/type named: ", type, ".")));
}

void
sailfishc::checkUDTExists(const std::string& s)
{
    auto udtname = s;
    if (udtname == "own")
    {
        if (isUdt)
            udtname = extractUDTName(filename);
        else
            errorhandler->handle(std::make_unique<Error>(Error(
                currentToken->col, currentToken->line,
                "illegal usage of own in a non udt method.", "", "", "")));
    }

    if (!udttable->hasUDT(udtname) &&
        (!symboltable->hasVariable(udtname) &&
         !udttable->hasUDT(symboltable->getSymbolType(udtname))))
        semanticerrorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line, "Unknown udt type.",
                  "Unknown type named: ", udtname, ".")));
}

std::string
sailfishc::checkFunctionCall(const std::string& name,
                             std::shared_ptr<SymbolTable> st)
{
    auto fcInputs = parseFunctionInputTypes(parseFunctionCall());

    // check if function exists
    if (!st->hasVariable(name))
    {
        semanticerrorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Nonexistent member function.",
                  "Nonexistent member function named: ", name, ".")));
        return "";
    }

    // get method signature
    auto functionSig = st->getSymbolType(name);
    auto inputs = parseFunctionInputTypes(functionSig);
    auto output = parseFunctionReturnType(functionSig);

    if (fcInputs.size() > inputs.size())
    {
        semanticerrorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line,
            "Too many inputs in function call " + name,
            "Expected " + std::to_string(inputs.size()) + " and received: ",
            std::to_string(fcInputs.size()), ".")));
        return "";
    }
    else if (fcInputs.size() < inputs.size())
    {
        semanticerrorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line,
            "Too few inputs in function call " + name,
            "Expected " + std::to_string(inputs.size()) + " and received: ",
            std::to_string(fcInputs.size()), ".")));
        return "";
    }
    else
    {
        bool flag = false;
        for (int i = 0; i < inputs.size(); i++)
        {
            auto left = symboltable->hasVariable(inputs[i])
                            ? symboltable->getSymbolType(inputs[i])
                            : inputs[i];
            auto right = symboltable->hasVariable(fcInputs[i])
                             ? symboltable->getSymbolType(fcInputs[i])
                             : fcInputs[i];

            if (left.at(0) == '[')
                left = extractListType(left);

            if (right.at(0) == '[')
                right = extractListType(right);

            if (left != right)
            {
                semanticerrorhandler->handle(std::make_unique<Error>(Error(
                    currentToken->col, currentToken->line,
                    "Function input parameter type mismatch in "
                    "function call " +
                        name,
                    "Expected " + inputs[i] + " and received: ", fcInputs[i],
                    ".")));
                return "";
            }
        }
    }

    return output;
}

std::string
sailfishc::parseFunctionReturnType(const std::string& s)
{
    return s.substr(s.find_last_of(")") + 1, s.size());
}

std::vector<std::string>
sailfishc::parseFunctionInputTypes(const std::string& s)
{
    std::vector<std::string> inputs;

    if (s.find_first_of("(") + 1 == s.find_last_of(")"))
    {
        semanticerrorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "At least one input is required for a function call. For a "
                  "function with no arguments, use the 'void' keyword.",
                  "", "", "")));
        return inputs;
    }

    auto inputsOnly = s.substr(s.find_first_of("(") + 2,
                               s.find_last_of(")") - s.find_first_of("(") - 2);

    std::stringstream test(inputsOnly);
    std::string segment;

    while (std::getline(test, segment, '_'))
        inputs.push_back(segment);

    return inputs;
}

std::string
parseListValues(const std::string& s)
{
    std::string inputs = "";

    auto inputsOnly = s.substr(s.find_first_of("[") + 1,
                               s.find_last_of("]") - s.find_first_of("[") - 1);

    std::stringstream test(inputsOnly);
    std::string segment;

    while (std::getline(test, segment, ','))
        inputs += " " + segment;

    return inputs + " "; // so that we don't get EOF before done parsing
}

bool
containsUDT(const std::string& filename)
{
    auto lex = std::make_unique<Lexar>(filename, true);
    auto currentToken = lex->getNextToken();
    while (currentToken->kind != TokenKind::EOF_)
    {
        if (currentToken->kind == TokenKind::UAT)
            return true;
        currentToken = lex->getNextToken();
    }

    return false;
}

// constructor
sailfishc::sailfishc(const std::string& file, bool sde)
{
    filename = file;
    lexar = std::make_unique<Lexar>(file, true);
    advanceToken();
    errorhandler = std::make_unique<ParserErrorHandler>(ParserErrorHandler());
    semanticerrorhandler = std::make_unique<SemanticAnalyzerErrorHandler>(
        SemanticAnalyzerErrorHandler(file, sde));
    symboltable = std::make_shared<SymbolTable>(SymbolTable());
    udttable = std::make_unique<UDTTable>(UDTTable());
    isUdt = false;
    shouldDisplayErrors = sde;
    transpiler = std::make_unique<Transpiler>(Transpiler());
}

// public interface method
void
sailfishc::parse()
{
    parseProgram();
}

// -------- Parser Parse Methods --------- //
/**
 * Program := Source
 */
void
sailfishc::parseProgram()
{
    parseSource();
}

/**
 * Source := Import Source | SourcePart
 */
void
sailfishc::parseSource()
{
    if (containsUDT(filename))
        isUdt = true;

    if (!isUdt)
        recursiveParse(false, TokenKind::IMPORT,
                       [this]() { this->parseImportInfo(); });
    else
    {
        if (currentToken->kind == TokenKind::IMPORT)
            semanticerrorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Illegal import in udt file.", "", "", "")));
    }
    parseSourcePart();
}

/**
 * ImportInfo := UDName Location
 *
 * Semantic Checks:
 *      - import file exists
 *      - import is a udt
 */
void
sailfishc::parseImportInfo()
{
    advanceAndCheckToken(TokenKind::IMPORT); // eat 'Import'
    auto name = parseUDName();
    advanceAndCheckToken(TokenKind::COLON); // eat colon
    auto loc = parseLocation();

    auto file = loc.substr(1, loc.size() - 2);

    Prettify::Formatter green(Prettify::FG_GREEN);
    Prettify::Formatter blue(Prettify::FG_LIGHT_BLUE);
    Prettify::Formatter normal(Prettify::RESET);

    std::cout << "Compiling import: " << blue << file << normal << ".\n";
    try
    {
        auto udtFlagAndBufer = parseFile(file, shouldDisplayErrors);

        auto table = std::move(std::get<0>(udtFlagAndBufer));
        auto flag = std::get<1>(udtFlagAndBufer);
        auto buf = std::get<2>(udtFlagAndBufer);

        if (!flag)
            errorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Expected imported file of type UDT",
                      "Received: ", "\"" + file + "\"", " of type script")));

        if (name != extractUDTName(file))
            errorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Expected imported udt filename to match corresponding "
                      "variable name.",
                      "Received: ", name,
                      " and expected: " + extractUDTName(file) + ".")));

        // add to own udt table under imported name, throwing an error if
        // the name already exists
        if (!udttable->hasUDT(name))
            udttable->addUDT(
                name, table->getAttributeSymbolTable(extractUDTName(file)),
                table->getMethodSymbolTable(extractUDTName(file)));

        symboltable->addSymbol(extractUDTName(file), "U");

        // aggregate udt buffers
        transpiler->append(buf);

        std::cout << green << "Successfully compiled import: " << normal << blue
                  << file << "\n"
                  << normal;
    }
    catch (char const* msg)
    {
        throw msg;
    }
}

/**
 * UDName := Identifier
 */
std::string
sailfishc::parseUDName()
{
    return parseIdentifier();
}

/**
 * Location := String
 */
std::string
sailfishc::parseLocation()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // true eat string
    return v;
}

/**
 * SourcePart := UDT | Script
 */
void
sailfishc::parseSourcePart()
{
    switch (currentToken->kind)
    {
    case TokenKind::UAT:
        parseUDT();
        break;
    default:
        parseScript();

        // don't write to out.c if we are testing, aka we don' care about errors
        if (shouldDisplayErrors)
            transpiler->write(semanticerrorhandler->getErrors().size() == 0);
    }
}

/**
 * UDT := UserDefinedType
 */
void
sailfishc::parseUDT()
{
    parseUserDefinedType();
}

/**
 * UserDefinedType := Attributes Methods
 */
void
sailfishc::parseUserDefinedType()
{
    auto udtname = extractUDTName(filename);

    auto a_st = std::make_shared<SymbolTable>(SymbolTable());
    auto m_st = std::make_shared<SymbolTable>(SymbolTable());

    udttable->addUDT(udtname, a_st, m_st);

    transpiler->genUDTHeader(udtname);

    this->parseAttributes(a_st);

    transpiler->genRightCurley();
    transpiler->genSemiColonAndNewline();

    this->parseMethods(m_st);

    transpiler->genUDTFooter(udtname);
}

/**
 * Attributes := 'Uat' [Identifier Identifier]*
 *
 * Semantic Checks:
 *      - type exists
 *      - unique attribute
 */
void
sailfishc::parseAttributes(std::shared_ptr<SymbolTable> st)
{
    advanceAndCheckToken(TokenKind::UAT);     // consume uat
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley

    st->clear();
    st->addSymbol(extractUDTName(filename), "U");

    recursiveParse(true, TokenKind::RCURLEY, [&st, this]() {
        auto sands = this->parseVariable();
        auto name = std::get<0>(sands);
        auto type = std::get<1>(sands);

        auto outtype = type;
        if ((st->hasVariable(outtype) && st->getSymbolType(outtype) == "U") ||
            udttable->hasUDT(outtype) ||
            (symboltable->hasVariable(outtype) &&
             symboltable->getSymbolType(outtype) == "U"))
            outtype = "struct " + outtype + "*";
        else
            outtype = builtinTypesTranslator(outtype);

        transpiler->genTypeAndNameNewLine(outtype, name);

        // check if unique name
        if (st->hasVariable(name))
        {
            semanticerrorhandler->handle(std::make_unique<Error>(Error(
                currentToken->col, currentToken->line, "Illegal redeclaration.",
                "Redeclared variable named: ", name, ".")));
        }

        // check if type exists
        if (type.at(0) == '[')
            type = extractListType(type);

        if (type == "own")
        {
            if (isUdt)
                type = extractUDTName(filename);
            else
                errorhandler->handle(std::make_unique<Error>(Error(
                    currentToken->col, currentToken->line,
                    "illegal usage of own in a non udt method.", "", "", "")));
        }

        if (!st->hasVariable(type) && !isPrimitive(type) &&
            !udttable->hasUDT(type) && !symboltable->hasVariable(type))
        {
            semanticerrorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Unknown variable or type.",
                      "Unknown variable/type named: ", type, ".")));
        }

        auto ok = st->addSymbol(name, type);
        if (!ok)
            semanticerrorhandler->handle(std::make_unique<Error>(
                Error(currentToken->col, currentToken->line,
                      "Unexpected redeclaration of " + name +
                          ", originally defined as type " +
                          st->getSymbolType(name) + ".",
                      "Received second declaration of type: ", type, ".")));
    });

    st->removeSymbol(extractUDTName(filename));

    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
}

/**
 * Methods := 'Ufn' [FunctionDefinition]*
 */
void
sailfishc::parseMethods(std::shared_ptr<SymbolTable> st)
{
    advanceAndCheckToken(TokenKind::UFN);     // consume ufn
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley

    // temporarily capture methods into symbol table for udt
    std::shared_ptr<SymbolTable> temp = symboltable;
    symboltable = st;

    // add udt so it can reference self (or own in Sailfish lingo)
    symboltable->addSymbol(extractUDTName(filename), "U");

    recursiveParse(true, TokenKind::RCURLEY,
                   [this]() { this->parseFunctionDefinition(); });

    // set back the normal symbol table and capture temp table
    st = symboltable;
    symboltable = temp;

    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
}

/**
 * Script := FunctionDefinition Script | Start
 */
void
sailfishc::parseScript()
{
    recursiveParse(true, TokenKind::START,
                   [this]() { this->parseFunctionDefinition(); });
    parseStart();
}

/**
 * FunctionDefinition := '(' 'fun' identifier FunctionInfo ')'
 *
 *  * Semantic Checks:
 *      - function is a unique declaration
 */
void
sailfishc::parseFunctionDefinition()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    advanceAndCheckToken(TokenKind::FUN);    // consume fun

    // parse left child
    auto id = parseIdentifier();

    // parse right child
    parseFunctionInfo(id);

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
}

/**
 * FunctionInfo := FunctionInOut Block
 *
 * Semantic Check:
 *  - actual return type matches expected return type
 */
void
sailfishc::parseFunctionInfo(const std::string& name)
{

    auto type = parseFunctionInOut(name);

    type = "F" + type;

    // add function itself to symbol table, one scope level back
    auto ok = symboltable->addSymbol(name, type);
    if (!ok)
        semanticerrorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Unexpected redeclaration of " + name +
                      ", originally defined as type " +
                      symboltable->getSymbolType(name) + ".",
                  "Received second declaration of type: ", type, ".")));

    transpiler->genLeftCurley();

    symboltable->enterScope();
    auto returnType = parseBlock();

    symboltable->exitScope();

    transpiler->genFunctionEnd();

    checkType(returnType, parseFunctionReturnType(type));
}

/**
 * FunctionInOut := FunctionInputs* FunctionOutputs
 * FunctionInputs :=  '(' Variable [',' Variable]* ')'
 *
 * Semantic Checks:
 *  - check for more than one argument after a declaration of void or that void
 * is first declaration as part of formals
 */
std::string
sailfishc::parseFunctionInOut(const std::string& name)
{
    // inputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren

    std::string types = "(";
    bool seenVoid = false;
    int argCount = 0;
    std::string outputBuffer = "";
    recursiveParse(true, TokenKind::RPAREN,
                   [&outputBuffer, &types, &seenVoid, &argCount, this]() {
                       ++argCount;

                       auto sands = this->parseVariable();
                       auto name = std::get<0>(sands);
                       auto type = std::get<1>(sands);

                       auto outedType = builtinTypesTranslator(type);
                       if (udttable->hasUDT(type))
                           outedType = "struct " + outedType + "*";

                       if (type != "void")
                           if (outputBuffer != "")
                               outputBuffer += ", " + outedType + " " + name;
                           else
                               outputBuffer += outedType + " " + name;
                       else if (!isUdt)
                       {
                           if (outputBuffer != "")
                               outputBuffer += ", " + outedType;
                           else
                               outputBuffer += outedType;
                       }

                       if (type == "void")
                           seenVoid = true;

                       if (argCount > 1 && seenVoid)
                       {
                           semanticerrorhandler->handle(std::make_unique<Error>(
                               Error(currentToken->col, currentToken->line,
                                     "Illegal multi-void definition of formals "
                                     "in function signature",
                                     "", "", "")));
                       }

                       if (type != "void")
                           symboltable->addSymbol(std::get<0>(sands), type);

                       types += "_" + type;
                   });
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    // outputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    auto output = parseType();

    types += ")" + output;

    if (udttable->hasUDT(output))
        output = "struct " + output + "*";
    else
        output = builtinTypesTranslator(output);

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    if (isUdt)
    {
        if (outputBuffer != "")
            outputBuffer += ", struct " + extractUDTName(filename) + "* this";
        else
            outputBuffer = "struct " + extractUDTName(filename) + "* this";
    }

    outputBuffer = output + "\n" + name + "(" + outputBuffer + ")\n";

    transpiler->append(outputBuffer);

    return types;
}

/**
 * Start := 'start' Block
 */
void
sailfishc::parseStart()
{
    advanceAndCheckToken(TokenKind::START);

    transpiler->genMainHeader();

    symboltable->enterScope();
    parseBlock();
    symboltable->exitScope();

    transpiler->genMainFooter();
}

/**
 * Block = '{' Statement* '}'
 *
 * Semantic Checks:
 *  - has exactly one RETURN statement
 *
 * Notes: defaults to void return type
 */
std::string
sailfishc::parseBlock()
{
    transpiler->incrementTabs();
    std::string type = "void";
    bool hasSeenReturn = false;
    advanceAndCheckToken(TokenKind::LCURLEY); // eat '{'

    recursiveParse(true, TokenKind::RCURLEY, [&type, &hasSeenReturn, this]() {
        auto a = this->parseStatement();

        if (std::get<1>(a) == "RETURN")
        {
            if (hasSeenReturn)
            {
                semanticerrorhandler->handle(std::make_unique<Error>(Error(
                    currentToken->col, currentToken->line,
                    "Illegal multiple definitions of return.", "", "", "")));
            }
            else
            {
                type = std::get<0>(a);
                if (symboltable->hasVariable(type))
                    type = symboltable->getSymbolType(type);
                if (type == "U" && udttable->hasUDT(std::get<0>(a)))
                    type = std::get<0>(a);
                hasSeenReturn = true;
            }
        }
    });
    advanceAndCheckToken(TokenKind::RCURLEY); // eat '}'
    transpiler->decrementTabs();
    return type;
}

/**
 * Statement := Tree | Return | Declaration | E0
 */
std::tuple<std::string, std::string>
sailfishc::parseStatement()
{
    std::string type = "";
    std::string val = "";
    transpiler->genStatementHeader();

    switch (currentToken->kind)
    {
    case TokenKind::TREE:
        parseTree();
        type = "tree";
        val = "TREE";
        break;
    case TokenKind::RETURN:
        type = parseReturn();
        val = "RETURN";
        break;
    case TokenKind::DEC:
        type = parseDeclaration();
        val = "DEC";
        break;
    default:
        type = parseE0();
        val = "E0";
    }

    transpiler->genStatementFooter();
    return std::make_tuple(type, val);
}

/**
 * Tree := 'tree' (' Branch* ')'
 */
void
sailfishc::parseTree()
{
    advanceAndCheckToken(TokenKind::TREE);   // eat 'tree'
    advanceAndCheckToken(TokenKind::LPAREN); // eat '('

    bool isFirstBranch = true;

    recursiveParse(true, TokenKind::RPAREN, [&isFirstBranch, this]() {
        if (isFirstBranch)
        {
            transpiler->genIfHeader();
            isFirstBranch = false;
        }
        else
            transpiler->genElseHeader();
        this->parseBranch();
    });

    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
}

/**
 * Branch := '(' Grouping Block')'
 */
void
sailfishc::parseBranch()
{
    advanceAndCheckToken(TokenKind::LPAREN); // eat '('

    parseGrouping();

    transpiler->genBranchHeader();

    symboltable->enterScope();
    parseBlock();
    symboltable->exitScope();

    transpiler->genBranchFooter();

    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
}

/**
 * Grouping := '|' E0 '|'
 *
 * Semantic Check:
 *  - resulting type is a bool
 */
void
sailfishc::parseGrouping()
{
    transpiler->genLeftParen();
    advanceAndCheckToken(TokenKind::PIPE); // eat '|'

    auto type = parseE0();

    checkType("bool", type);

    transpiler->genRightParen();

    advanceAndCheckToken(TokenKind::PIPE); // eat '|'
}

/**
 * Return := 'return' T
 */
std::string
sailfishc::parseReturn()
{
    advanceAndCheckToken(TokenKind::RETURN); // consume 'return'
    transpiler->genReturn();

    return parseE0();
}

/**
 * Declaration :=  'dec' Variable '=' E0
 *
 * Semantic Analysis:
 *  - check that the declared type and the init type are the same
 */
std::string
sailfishc::parseDeclaration()
{
    advanceAndCheckToken(TokenKind::DEC); // consume 'dec'

    auto sands = parseVariable();
    auto name = std::get<0>(sands);
    auto type = std::get<1>(sands);

    auto outtype = type;

    // check to see if it is a udt name first
    if (udttable->hasUDT(outtype))
        outtype = "struct " + outtype + "*";
    else
        outtype = builtinTypesTranslator(outtype);

    transpiler->genTypeAndName(outtype, name);
    transpiler->genOperator("=");

    checkExists(type);

    auto ok = symboltable->addSymbol(name, type);
    if (!ok)
        semanticerrorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Unexpected redeclaration of " + name +
                      ", originally defined as type " +
                      symboltable->getSymbolType(name) + ".",
                  "Received second declaration of type: ", type, ".")));

    transpiler->setDecName(name);

    advanceAndCheckToken(TokenKind::ASSIGNMENT); // consume '='
    auto ta = parseE0();

    if (name.at(0) == '[')
        checkType(name, ta);
    else
        checkType(type, ta);

    return type;
}

/**
 * E0 := T0 E1
 */
std::string
sailfishc::parseE0()
{
    auto type = parseT();
    return parseE1(type);
}

/**
 * E1 := ** E0 | E2
 *
 * Semantic Check
 *  - both are int
 */
std::string
sailfishc::parseE1(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE2(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            this->checkType(T0, T1);
            this->checkType("int", T0);
            this->checkType("int", T1);
            return T1;
        },
        std::make_tuple(TokenKind::EXPONENTIATION, "**"));
}

/**
 * E2 := * E0 | / E0 | % E0 | E3
 *
 * Semantic Check
 *  - both are num (int or flt) and the same type
 */
std::string
sailfishc::parseE2(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE3(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are same
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return T1;
        },
        std::make_tuple(TokenKind::MULTIPLICATION, "*"),
        std::make_tuple(TokenKind::DIVISION, "/"),
        std::make_tuple(TokenKind::MODULO, "%"));
}

/**
 * E3 := + E0 | - E0 | E4
 *
 * Semantic Check
 *  - both are num (int or flt) and the same type
 */
std::string
sailfishc::parseE3(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE4(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are same
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return T1;
        },
        std::make_tuple(TokenKind::ADDITION, "+"),
        std::make_tuple(TokenKind::SUBTRACTION, "-"));
}

/**
 * E4 := < E0 | > E0 | <= E0 | >= E0 | E5
 *
 * Semantic Check
 *  - both are num (int or flt) and the same type
 */
std::string
sailfishc::parseE4(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE5(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are bool
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return "bool";
        },
        std::make_tuple(TokenKind::LESS_THAN, "<"),
        std::make_tuple(TokenKind::LESS_THAN_OR_EQUALS, "<="),
        std::make_tuple(TokenKind::GREATER_THAN, ">"),
        std::make_tuple(TokenKind::GREATER_THAN_OR_EQUALS, ">="));
}

/**
 * E5 := == E0 | != E0 | E6
 *
 * Semantic Check
 *  - both are the same type
 */
std::string
sailfishc::parseE5(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE6(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are bool
            this->checkType(T0, T1);
            return "bool";
        },
        std::make_tuple(TokenKind::EQUIVALENCE, "=="),
        std::make_tuple(TokenKind::NONEQUIVALENCE, "!="));
}

/**
 * E6 := 'and' E0 | 'or' E0 | E7
 *
 * Semantic Check
 *  - both are bool
 */
std::string
sailfishc::parseE6(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE7(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are bool
            this->checkType(T0, T1);
            this->checkType("bool", T0);
            this->checkType("bool", T1);
            return "bool";
        },
        std::make_tuple(TokenKind::OR, "||"),
        std::make_tuple(TokenKind::AND, "&&"));
}

/**
 * E7 := = E0 | E8
 *
 * Semantic Check
 *  - the variable has already been declared
 *  - the lhs matches the rhs
 */
std::string
sailfishc::parseE7(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE8(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // check that the variable has been declared before used (and thus
            // initialized)
            this->checkExists(T0);
            // check that the two types are the same
            auto type = T0;
            if (!isPrimitive(T0))
                type = this->symboltable->getSymbolType(T0);
            if (type == "U")
                type = T0;

            this->checkType(type, T1);
            return T1;
        },

        std::make_tuple(TokenKind::ASSIGNMENT, "="));
}

/**
 * E8 := [!, ++, --] E0 | E9
 *
 * Semantic Check
 *  - check that ! is bool and ++/-- are num
 */
std::string
sailfishc::parseE8(const std::string& T0)
{

    if (currentToken->kind == TokenKind::NEGATION)
    {
        advanceAndCheckToken(TokenKind::NEGATION); // consume '!'
        transpiler->genOperator("!");
        auto type = parseE0();

        checkType("bool", type);

        return "bool";
    }

    if (currentToken->kind == TokenKind::UNARYADD)
    {
        advanceAndCheckToken(TokenKind::UNARYADD); // consume '++'
        transpiler->genOperator("++");
        auto type = parseE0();

        checkType("num", type);

        return type;
    }

    if (currentToken->kind == TokenKind::UNARYMINUS)
    {
        advanceAndCheckToken(TokenKind::UNARYMINUS); // consume '--'
        transpiler->genOperator("--");
        auto type = parseE0();

        checkType("num", type);

        return type;
    }

    return parseE9(T0);
}

/**
 * E9 := [+=, -=, /=, *=] E0 | E10
 *
 * Semantic Check
 *  - both are num and the same type
 */
std::string
sailfishc::parseE9(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> std::string {
            return this->parseE10(T0);
        },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are the same
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return T1;
        },
        std::make_tuple(TokenKind::ADDTO, "+="),
        std::make_tuple(TokenKind::SUBFROM, "-="),
        std::make_tuple(TokenKind::DIVFROM, "/="),
        std::make_tuple(TokenKind::MULTTO, "*="));
}

/**
 * E10 := MemberAccess E0 | E11
 */
std::string
sailfishc::parseE10(const std::string& T0)
{
    if (currentToken->kind == TokenKind::DOT ||
        currentToken->kind == TokenKind::TRIPLE_DOT)
    {
        auto type = parseMemberAccess(T0);

        type = parseE1(type);

        return type;
    }

    return parseE11(T0);
}

/**
 * E11 := New | E12
 */
std::string
sailfishc::parseE11(const std::string& T0)
{
    if (currentToken->kind == TokenKind::NEW)
    {
        auto type = parseNew();
        return type;
    }

    return parseE12(T0);
}

/**
 * E12 := FunctionCall | E13
 */
std::string
sailfishc::parseE12(const std::string& T0)
{
    if (currentToken->kind == TokenKind::LPAREN)
    {
        transpiler->genLeftParen();
        checkExists(T0);

        auto name = T0;

        auto output = checkFunctionCall(name, symboltable);

        transpiler->genRightParen();
        return output;
    }

    return T0;
}

/**
 * MemberAccess := AttributeAccess | MethodAccess
 */
std::string
sailfishc::parseMemberAccess(const std::string& T0)
{
    auto type = T0;
    if (type == "own")
    {
        if (isUdt)
            type = extractUDTName(filename);
        else
            errorhandler->handle(std::make_unique<Error>(Error(
                currentToken->col, currentToken->line,
                "illegal usage of own in a non udt method.", "", "", "")));
    }
    if (!udttable->hasUDT(type))
    {
        type = symboltable->getSymbolType(type);
        checkUDTExists(type);
    }
    switch (currentToken->kind)
    {
    case TokenKind::DOT:
        return parseAttributeAccess(T0, type);
    case TokenKind::TRIPLE_DOT:
        return parseMethodAccess(T0, type);
    default:
        errorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line, "Expected a ... or . token.",
            "Received: ", currentToken->value,
            " of type " + displayKind(currentToken->kind) + ".")));
        return ""; //  unreachable
    }
}

/**
 * AttributeAccess := '.' Identifier
 */
std::string
sailfishc::parseAttributeAccess(const std::string& udtname,
                                const std::string& udtType)
{

    checkExists(udtType);
    checkUDTExists(udtType);
    // get udt's attribute symbol table
    auto st = udttable->getAttributeSymbolTable(udtType);

    advanceAndCheckToken(TokenKind::DOT); // consume '.'
    auto attribute = parseIdentifier();

    transpiler->genAttributeAccess(
        (currentToken->kind == TokenKind::TRIPLE_DOT),
        udttable->hasUDT(udtname), udtname, attribute);

    // check if type exists
    if (!st->hasVariable(attribute))
    {
        semanticerrorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line, "Nonexistent attribute.",
            "Nonexistent attribute named: ", attribute, ".")));
    }

    return st->getSymbolType(attribute);
}

/**
 * MethodAccess := '...' Identifier FunctionCall
 */
std::string
sailfishc::parseMethodAccess(const std::string& udtname,
                             const std::string& udtType)
{
    checkExists(udtType);
    checkUDTExists(udtType);

    // get udt's method symbol table
    auto st = udttable->getMethodSymbolTable(udtType);

    advanceAndCheckToken(TokenKind::TRIPLE_DOT); // consume '...'

    auto methodName = parseIdentifier();

    if (udtname == "own")
        transpiler->pushMethod("this", methodName);
    else
        transpiler->pushMethod(udtname, methodName);

    transpiler->append(methodName);

    transpiler->genLeftParen();

    auto output = checkFunctionCall(methodName, st);

    transpiler->genRightParen();

    transpiler->popMethod();

    return output;
}

/**
 * FunctionCall := '(' [Identifier [',' Identifier]*] ')'
 */
std::string
sailfishc::parseFunctionCall()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren

    std::string types = "(";
    int nonVoidInputs = 0;
    recursiveParse(true, TokenKind::RPAREN, [&nonVoidInputs, &types, this]() {
        if (nonVoidInputs)
            transpiler->genComma();

        auto type = parseE0();

        if (symboltable->hasVariable(type))
            type = symboltable->getSymbolType(type);

        if (type != "void")
            ++nonVoidInputs;

        types += "_" + type;
    });

    transpiler->genFinalFunctionCallArg((nonVoidInputs == 0), isUdt);

    types += ")";

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    return types;
}

/**
 * New := UDTDec
 */
std::string
sailfishc::parseNew()
{
    advanceAndCheckToken(TokenKind::NEW); // consume new
    switch (currentToken->kind)
    {
    case TokenKind::IDENTIFIER:
    {
        return parseUDTDec();
    }
    default:
        errorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Expected a valid new declaration such as a UDT defined "
                  "by '{' '}'.",
                  "Received: ", currentToken->value,
                  " of type " + displayKind(currentToken->kind) + ".")));
        return ""; //  unreachable
    }
}

/**
 * UDTDec := Identifier '{' [UDTDecItem [',' UDTDecItem]*] '}'
 * UDTDecItem := Identifier ':' Primary
 */
std::string
sailfishc::parseUDTDec()
{
    auto udtName = parseIdentifier();

    transpiler->genUDTDecInit(udtName);

    checkExists(udtName);
    checkUDTExists(udtName);

    auto st = udttable->getAttributeSymbolTable(udtName);
    auto attributes = st->getSymbols();

    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    recursiveParse(
        true, TokenKind::RCURLEY, [st, udtName, &attributes, this]() {
            // capture key
            auto attributeName = parseIdentifier();

            transpiler->genUDTDecItem(attributeName);

            advanceAndCheckToken(TokenKind::COLON); // consume ':'

            std::string temp = transpiler->getDecName();
            if (currentToken->value.at(0) == '[')
            {
                transpiler->setDecName(transpiler->getDecName() + "->" +
                                       attributeName);
                transpiler->setDecType(st->getSymbolType(attributeName));
            }

            // capture value
            auto type = parsePrimary();

            transpiler->setDecName(temp);

            if (attributes.size() != 1)
                transpiler->genSemiColonAndNewline();

            // determine if key exists for udt
            std::vector<std::string>::iterator it =
                std::find(attributes.begin(), attributes.end(), attributeName);

            if (it != attributes.end())
            {
                int index = std::distance(attributes.begin(), it);
                checkType(st->getSymbolType(attributeName), type);
                attributes.erase(attributes.begin() + index);
            }
            else
                semanticerrorhandler->handle(std::make_unique<Error>(Error(
                    currentToken->col, currentToken->line,
                    "Unrecognized initialization key for new udt of type: " +
                        udtName,
                    "Unrecognized key: ", attributeName, ".")));
        });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley

    if (attributes.size() != 0)
        semanticerrorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line,
            "Missing keys in udt initialization for type: " + udtName,
            "Expected " + std::to_string(st->getSymbols().size()) +
                " keys and received ",
            std::to_string(st->getSymbols().size() - attributes.size()), ".")));
    return udtName;
}

/**
 * T := Primary | '(' E0 ')'
 */
std::string
sailfishc::parseT()
{
    if (currentToken->kind == TokenKind::LPAREN)
    {
        advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
        transpiler->genLeftParen();
        auto type = parseE0();
        advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
        transpiler->genRightParen();
        return type;
    }

    else if (currentToken->kind == TokenKind::NEGATION ||
             currentToken->kind == TokenKind::UNARYADD ||
             currentToken->kind == TokenKind::UNARYMINUS ||
             currentToken->kind == TokenKind::NEW)
    {
        return parseE1("");
    }

    return parsePrimary();
}

/**
 * Primary := Bool | Integer |  String | Identifier
 */
std::string
sailfishc::parsePrimary()
{
    switch (currentToken->kind)
    {
    case TokenKind::BOOL:
        parseBoolean();
        return "bool";
    case TokenKind::INTEGER:
        parseNumber();
        return "int";
    case TokenKind::FLOAT:
        parseNumber();
        return "flt";
    case TokenKind::STRING:
        parseString();
        return "str";
    case TokenKind::OWN_ACCESSOR:
        parseOwnAccessor();
        return "own";
    case TokenKind::EMPTY:
        parseEmpty();
        return "empty";
    case TokenKind::IDENTIFIER:
    {
        if (currentToken->value.at(0) == '[')
            return parseListType();

        auto type = parseIdentifier();
        transpiler->genPrimary((currentToken->kind != TokenKind::TRIPLE_DOT),
                               (currentToken->kind != TokenKind::DOT),
                               udttable->hasUDT(type), type);

        return type;
    }
    case TokenKind::LIST:
        return parseList();
    default:
        errorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "Expected a valid primary token such as a boolean, "
                  "integer, float, string, identifier, or list.",
                  "Received: ", currentToken->value,
                  " of type " + displayKind(currentToken->kind) + ".")));
        return "bool"; //  unreachable
    }
}

/**
 * Type := Identifier
 */
std::string
sailfishc::parseType()
{
    return currentToken->kind == TokenKind::LISTTYPE ? parseListType()
                                                     : parseIdentifier();
}

/**
 * Variable := Type Identifier
 */
std::tuple<std::string, std::string>
sailfishc::parseVariable()
{
    auto type = parseType();

    // deal with void types
    if (type == "void")
        return std::make_tuple(type, type);

    auto name = parseIdentifier();
    return std::make_tuple(name, type);
}

/**
 * Number := Integer | Float
 */
std::string
sailfishc::parseNumber()
{
    auto v = currentToken->value;
    auto k = currentToken->kind;

    if (k == TokenKind::INTEGER)
    {
        advanceAndCheckToken(TokenKind::INTEGER); // eat integer
        transpiler->genLiteral(v);
        return v;
    }

    advanceAndCheckToken(TokenKind::FLOAT); // eat float
    transpiler->genLiteral(v);
    return v;
}

/**
 * Identifier := lexvalue
 */
std::string
sailfishc::parseIdentifier()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::IDENTIFIER); // eat identifier
    return v;
}

/**
 * Bool := lexvalue
 */
std::string
sailfishc::parseBoolean()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::BOOL); // eat identifier
    transpiler->genLiteral(v == "true" ? "1" : "0");
    return v;
}

/**
 * String := lexvalue
 */
std::string
sailfishc::parseString()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // true eat string
    transpiler->genLiteral(v);
    return v;
}

/**
 * Own Accessor:= lexvalue
 */
std::string
sailfishc::parseOwnAccessor()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::OWN_ACCESSOR); // eat own accessor

    // transpiler->genLiteral("this");

    if (isUdt)
        return extractUDTName(filename);
    else
        errorhandler->handle(std::make_unique<Error>(
            Error(currentToken->col, currentToken->line,
                  "illegal usage of own in a non udt method.", "", "", "")));
    return v; // will not ever reach here
}

/**
 * Empty:= lexvalue
 */
std::string
sailfishc::parseEmpty()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::EMPTY); // eat own accessor

    transpiler->genLiteral("NULL");

    return v; // will not ever reach here
}

/**
 * ListType := lexvalue
 */
std::string
sailfishc::parseListType()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LISTTYPE); // eat list type
    return v;
}

// --------       Some helpers for parsing a list       -------- //

std::vector<std::unique_ptr<Token>>
determineTypes(const std::string& s)
{
    auto lexar = std::make_unique<Lexar>(s, false);
    std::vector<std::unique_ptr<Token>> vals;
    auto token = lexar->getNextToken();
    while (token->kind != TokenKind::EOF_)
    {
        vals.push_back(std::move(token));
        token = lexar->getNextToken();
    }

    return vals;
}

std::string
sailfishc::tokenToType(const TokenKind& tk, const std::string& val)
{
    switch (tk)
    {
    case TokenKind::IDENTIFIER:
        if (symboltable->hasVariable(val))
            return symboltable->getSymbolType(val);
        return val;
    case TokenKind::INTEGER:
        return "int";
    case TokenKind::FLOAT:
        return "flt";
    case TokenKind::STRING:
        return "str";
    case TokenKind::BOOL:
        return "bool";
    default:
        errorhandler->handle(std::make_unique<Error>(Error(
            currentToken->col, currentToken->line, "Unexpected type in a list.",
            "Type: ", displayKind(tk), "")));
        return ""; // unreachable
    }
}

/**
 * List := lexvalue
 */
std::string
sailfishc::parseList()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LIST); // eat list
    auto listVals = determineTypes(parseListValues(v));

    std::string type = transpiler->getDecType();

    std::deque<std::string> vals;
    if (listVals.size() != 0)
    {
        for (int i = listVals.size() - 1; i >= 0; i--)
        {
            auto v = std::move(listVals.at(i));
            auto ty = tokenToType(v->kind, v->value);

            vals.emplace_front(v->value);

            if (i == listVals.size() - 1)
                type = ty;

            else
                checkType(type, ty);
        }
    }

    transpiler->genListInit(type, std::to_string(listVals.size()));

    for (int i = 0; i < vals.size(); i++)
    {
        transpiler->genListItem(std::to_string(i), vals.at(i));
    }

    return type;
}

std::vector<std::shared_ptr<Error>>
sailfishc::getErrors()
{
    return semanticerrorhandler->getErrors();
}
