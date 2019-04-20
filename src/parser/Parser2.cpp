#include "Parser2.h"

std::string
Parser2::getTabs()
{
    std::string s = "";
    for (int i = 0; i < currentTabs; i++)
        s += "    ";
    return s;
}

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

    else
        return type;
}

UdtFlagAndBufer
parseFile(const std::string& filename)
{
    try
    {
        Parser2* p = new Parser2(filename);
        p->parse();
        return std::make_tuple(std::move(p->getUDTTable()), p->getIsUDTFlag(),
                               p->getTargetBuffer());
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
Parser2::advanceAndCheckToken(const TokenKind& k)
{
    // first check value and kind
    if (currentToken->kind != k)
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Expected a token of type: " + displayKind(k),
                   "Received: ", currentToken->value,
                   " of type " + displayKind(currentToken->kind) + ".")));

    advanceToken();
}

void
Parser2::advanceToken()
{
    currentToken = lexar->getNextToken();

    // catch errors from the lexar
    if (currentToken->kind == TokenKind::ERROR)
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line, "Lexar Error.",
                   "Error: ", currentToken->value, "")));

    while (currentToken->kind == TokenKind::COMMENT ||
           currentToken->kind == TokenKind::COMMA)
    {
        currentToken = lexar->getNextToken();

        // catch errors from the lexar
        if (currentToken->kind == TokenKind::ERROR)
            errorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line, "Lexar Error.",
                       "Error: ", currentToken->value, "")));
    }
}

// -------- Semantic Analysis Helper Code --------- //
void
Parser2::checkType(const std::string& t0, const std::string& t1)
{
    // convert left and right to type of var if they are vars
    auto left =
        symboltable->hasVariable(t0) ? symboltable->getSymbolType(t0) : t0;
    auto right =
        symboltable->hasVariable(t1) ? symboltable->getSymbolType(t1) : t1;

    // should actually check types here

    // edge case lists
    if (left.at(0) == '[' || right.at(0) == '[')
    {
        // if one is a list, both must be lists
        if (left.at(0) == '[')
            left = extractListType(left);

        if (right.at(0) == '[')
            right = extractListType(right);

        if (t0 != "none" && t1 != "none" && left != right)
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Mismatched list types. Expected is: [" + left + "].",
                       "Received is: ", "[" + right + "]", ".")));
    }

    // edge case left is num and thus right can be either int or flt type
    else if (left == "num")
    {
        if ("int" != right && "flt" != right)
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Mismatched types. Expected/LeftHand is: int or flt.",
                       "Received/Right Hand is: ", right, ".")));
    }

    // normal error check, with edge case that right is an empty
    else if (left != right && right != "empty")
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Mismatched types. Expected/LeftHand is: " + t0 + ".",
                   "Received/Right Hand is: ", right, ".")));
}

bool
isPrimitive(const std::string& s)
{
    return s == "int" || s == "flt" || s == "void" || s == "bool" || s == "str";
}

void
Parser2::checkUnique(const std::string& s)
{
    if (symboltable->hasVariable(s))
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Illegal redeclaration.",
            "Redeclared variable named: ", s, ".")));
}

void
Parser2::checkExists(const std::string& s)
{

    auto type = s;
    if (type == "own")
    {
        if (isUdt)
            type = extractUDTName(filename);
        else
            errorhandler->handle(std::make_unique<Error2>(Error2(
                currentToken->col, currentToken->line,
                "illegal usage of own in a non udt method.", "", "", "")));
    }

    if (s.at(0) == '[')
        type = extractListType(type);

    if (!symboltable->hasVariable(type) && !isPrimitive(type))
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Unknown variable or type.",
            "Unknown variable/type named: ", type, ".")));
}

void
Parser2::checkUDTExists(const std::string& s)
{
    auto udtname = s;
    if (udtname == "own")
    {
        if (isUdt)
            udtname = extractUDTName(filename);
        else
            errorhandler->handle(std::make_unique<Error2>(Error2(
                currentToken->col, currentToken->line,
                "illegal usage of own in a non udt method.", "", "", "")));
    }

    if (!udttable->hasUDT(udtname) &&
        (!symboltable->hasVariable(udtname) &&
         !udttable->hasUDT(symboltable->getSymbolType(udtname))))
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line, "Unknown udt type.",
                   "Unknown type named: ", udtname, ".")));
}

std::string
Parser2::checkFunctionCall(const std::string& name,
                           std::shared_ptr<SymbolTable> st)
{
    auto fcInputs = parseFunctionInputTypes(parseFunctionCall());

    // check if function exists
    if (!st->hasVariable(name))
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Nonexistent member function.",
                   "Nonexistent member function named: ", name, ".")));

    // get method signature
    auto functionSig = st->getSymbolType(name);
    auto inputs = parseFunctionInputTypes(functionSig);
    auto output = parseFunctionReturnType(functionSig);

    if (fcInputs.size() > inputs.size())
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line,
            "Too many inputs in function call " + name,
            "Expected " + std::to_string(inputs.size()) + " and received: ",
            std::to_string(fcInputs.size()), ".")));
    else if (fcInputs.size() < inputs.size())
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line,
            "Too few inputs in function call " + name,
            "Expected " + std::to_string(inputs.size()) + " and received: ",
            std::to_string(fcInputs.size()), ".")));
    else
    {
        bool flag = false;
        for (int i = 0; i < inputs.size(); i++)
        {
            if (inputs[i].at(0) == '[')
                inputs[i] = extractListType(inputs[i]);

            if (fcInputs[i].at(0) == '[')
                fcInputs[i] = extractListType(fcInputs[i]);

            if (inputs[i] != fcInputs[i])
                semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "Function input parameter type mismatch in function call " +
                        name,
                    "Expected " + inputs[i] + " and received: ", fcInputs[i],
                    ".")));
        }
    }

    return output;
}

std::string
Parser2::parseFunctionReturnType(const std::string& s)
{
    return s.substr(s.find_last_of(")") + 1, s.size());
}

std::vector<std::string>
Parser2::parseFunctionInputTypes(const std::string& s)
{
    std::vector<std::string> inputs;

    if (s.find_first_of("(") + 1 == s.find_last_of(")"))
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
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

// constructor
Parser2::Parser2(const std::string& file)
{
    filename = file;
    lexar = std::make_unique<Lexar2>(file, true);
    advanceToken();
    errorhandler = std::make_unique<Parser2ErrorHandler>(Parser2ErrorHandler());
    semanticerrorhandler = std::make_unique<SemanticAnalyzerErrorHandler>(
        SemanticAnalyzerErrorHandler(file));
    symboltable = std::make_shared<SymbolTable>(SymbolTable());
    udttable = std::make_unique<UDTTable>(UDTTable());
    isUdt = false;
}

// public interface method
void
Parser2::parse()
{
    parseProgram();
}

// -------- Parser Parse Methods --------- //
/**
 * Program := Source
 */
void
Parser2::parseProgram()
{
    parseSource();
}
/**
 * Source := Import Source | SourcePart
 */
void
Parser2::parseSource()
{
    recursiveParse(false, TokenKind::IMPORT,
                   [this]() { this->parseImportInfo(); });
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
Parser2::parseImportInfo()
{
    advanceAndCheckToken(TokenKind::IMPORT); // eat 'Import'
    auto name = parseUDName();
    advanceAndCheckToken(TokenKind::COLON); // eat colon
    auto loc = parseLocation();

    auto file = loc.substr(1, loc.size() - 2);

    try
    {
        std::cout << "Compiling import: " + file + "\n";
        auto udtFlagAndBufer = parseFile(file);

        auto table = std::move(std::get<0>(udtFlagAndBufer));
        auto flag = std::get<1>(udtFlagAndBufer);
        auto buf = std::get<2>(udtFlagAndBufer);

        if (!flag)
            errorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Expected imported file of type UDT",
                       "Received: ", "\"" + file + "\"", " of type script")));

        if (name != extractUDTName(file))
            errorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Expected imported udt filename to match corresponding "
                       "variable name.",
                       "Received: ", name,
                       " and expected: " + extractUDTName(file) + ".")));

        // add to own udt table under imported name, throwing an error if the
        // name already exists
        if (!udttable->hasUDT(name))
            udttable->addUDT(
                name, table->getAttributeSymbolTable(extractUDTName(file)),
                table->getMethodSymbolTable(extractUDTName(file)));

        symboltable->addSymbol(name, "U");

        // aggregate udt buffers
        targetBuffer += buf;

        std::cout << "Successfully compiled import: " + file + "\n";
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
Parser2::parseUDName()
{
    return parseIdentifier();
}

/**
 * Location := String
 */
std::string
Parser2::parseLocation()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // true eat string
    return v;
}

/**
 * SourcePart := UDT | Script
 */
void
Parser2::parseSourcePart()
{
    switch (currentToken->kind)
    {
    case TokenKind::UAT:
        isUdt = true;
        parseUDT();
        break;
    default:
    {
        // capture and open file only if not a udt so that imports do not
        // override
        output.clear();
        output.open("out.c");

        // set the file header here
        output << OUTPUT_HEADER << getStdLibC();

        // add the buffers from parsed udt files
        output << targetBuffer;
        targetBuffer = "";
        parseScript();

        output << targetBuffer;
    }
    }
}

/**
 * UDT := UserDefinedType
 */
void
Parser2::parseUDT()
{
    parseUserDefinedType();
}

/**
 * UserDefinedType := Attributes Methods
 */
void
Parser2::parseUserDefinedType()
{
    auto udtname = extractUDTName(filename);

    auto a_st = std::make_shared<SymbolTable>(SymbolTable());
    auto m_st = std::make_shared<SymbolTable>(SymbolTable());

    udttable->addUDT(udtname, a_st, m_st);

    targetBuffer +=
        "\n//___________BEGIN_" + udtname + "_UDT_DEFINITION__________/_//\n\n";

    targetBuffer += "struct " + udtname + "\n{\n";
    a_st = this->parseAttributes(a_st);
    targetBuffer += "};\n";

    std::shared_ptr<SymbolTable> methodst = this->parseMethods(m_st);

    targetBuffer +=
        "//___________END_" + udtname + "_UDT_DEFINITION__________/_//\n\n";
}

/**
 * Attributes := 'Uat' [Identifier Identifier]*
 *
 * Semantic Checks:
 *      - type exists
 *      - unique attribute
 */
std::shared_ptr<SymbolTable>
Parser2::parseAttributes(std::shared_ptr<SymbolTable> st)
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
            udttable->hasUDT(outtype))
            outtype = "struct " + outtype + "*";
        else
            outtype = builtinTypesTranslator(outtype);

        targetBuffer += "\t" + outtype + " " + name + ";\n";

        // check if unique name
        if (st->hasVariable(name))
        {
            semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
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
                errorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "illegal usage of own in a non udt method.", "", "", "")));
        }

        if (!st->hasVariable(type) && !isPrimitive(type) &&
            !udttable->hasUDT(type))
        {
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Unknown variable or type.",
                       "Unknown variable/type named: ", type, ".")));
        }

        auto ok = st->addSymbol(name, type);
        if (!ok)
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Unexpected redeclaration of " + name +
                           ", originally defined as type " +
                           st->getSymbolType(name) + ".",
                       "Received second declaration of type: ", type, ".")));
    });

    st->removeSymbol(extractUDTName(filename));

    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley

    return st;
}

/**
 * Methods := 'Ufn' [FunctionDefinition]*
 */
std::shared_ptr<SymbolTable>
Parser2::parseMethods(std::shared_ptr<SymbolTable> st)
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
    return st;
}

void
Parser2::parseScript()
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
Parser2::parseFunctionDefinition()
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
Parser2::parseFunctionInfo(const std::string& name)
{

    auto type = parseFunctionInOut(name);

    type = "F" + type;

    // add function itself to symbol table, one scope level back
    auto ok = symboltable->addSymbol(name, type);
    if (!ok)
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Unexpected redeclaration of " + name +
                       ", originally defined as type " +
                       symboltable->getSymbolType(name) + ".",
                   "Received second declaration of type: ", type, ".")));

    targetBuffer += "{";

    symboltable->enterScope();
    auto returnType = parseBlock();

    symboltable->exitScope();

    targetBuffer += "\n}\n\n";

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
Parser2::parseFunctionInOut(const std::string& name)
{
    // inputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren

    std::string types = "(";
    bool seenVoid = false;
    int argCount = 0;
    std::string outputBuffer = "";
    recursiveParse(
        true, TokenKind::RPAREN,
        [&outputBuffer, &types, &seenVoid, &argCount, this]() {
            ++argCount;

            auto sands = this->parseVariable();
            auto name = std::get<0>(sands);
            auto type = std::get<1>(sands);

            if (type != "void")
                if (outputBuffer != "")
                    outputBuffer +=
                        ", " + builtinTypesTranslator(type) + " " + name;
                else
                    outputBuffer += type + " " + name;
            else if (!isUdt)
            {
                if (outputBuffer != "")
                    outputBuffer += ", " + builtinTypesTranslator(type);
                else
                    outputBuffer += type;
            }

            if (type == "void")
                seenVoid = true;

            if (argCount > 1 && seenVoid)
            {
                semanticerrorhandler->handle(std::make_unique<Error2>(
                    Error2(currentToken->col, currentToken->line,
                           "Illegal multi-void definition of formals "
                           "in function signature",
                           "", "", "")));
            }

            if (type != "void")
            {
                auto ok = symboltable->addSymbol(std::get<0>(sands), type);
                // if (!ok)
                // semanticerrorhandler->handle(std::make_unique<Error2>(
                //     Error2(currentToken->col, currentToken->line,
                //            "Unexpected redeclaration of " + name +
                //                ", originally defined as type " +
                //                symboltable->getSymbolType(name) + ".",
                //            "Received second declaration of type: ", type,
                //            ".")));
            }
            types += "_" + type;
        });
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    // outputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    auto output = parseType();
    types += ")" + output;
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    if (isUdt)
    {
        if (outputBuffer != "")
            outputBuffer += ", struct " + extractUDTName(filename) + "* this";
        else
            outputBuffer = "struct " + extractUDTName(filename) + "* this";
    }
    outputBuffer = output + "\n" + name + "(" + outputBuffer + ")\n";

    targetBuffer += outputBuffer;

    return types;
}

/**
 * Start := 'start' Block
 */
void
Parser2::parseStart()
{
    advanceAndCheckToken(TokenKind::START);

    targetBuffer += "int\nmain()\n{";

    symboltable->enterScope();
    parseBlock();
    symboltable->exitScope();

    targetBuffer += "\n    return 1;\n}";
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
Parser2::parseBlock()
{
    ++currentTabs;
    std::string type = "void";
    bool hasSeenReturn = false;
    advanceAndCheckToken(TokenKind::LCURLEY); // eat '{'

    recursiveParse(true, TokenKind::RCURLEY, [&type, &hasSeenReturn, this]() {
        auto a = this->parseStatement();

        if (std::get<1>(a) == "RETURN")
        {
            if (hasSeenReturn)
            {
                semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "Illegal multiple definitions of return.", "", "", "")));
            }
            else
            {
                type = std::get<0>(a);
                if (symboltable->hasVariable(type))
                    type = symboltable->getSymbolType(type);
                hasSeenReturn = true;
            }
        }
    });
    advanceAndCheckToken(TokenKind::RCURLEY); // eat '}'
    --currentTabs;
    return type;
}

/**
 * Statement := Tree | Return | Declaration | E0
 */
std::tuple<std::string, std::string>
Parser2::parseStatement()
{
    targetBuffer += "\n" + getTabs();
    switch (currentToken->kind)
    {
    case TokenKind::TREE:
        parseTree();
        return std::make_tuple("tree", "TREE");
    case TokenKind::RETURN:
    {
        auto type = parseReturn();
        targetBuffer += ";";
        return std::make_tuple(type, "RETURN");
    }
    case TokenKind::DEC:
    {
        auto type = parseDeclaration();
        targetBuffer += ";";
        return std::make_tuple(type, "DEC");
    }
    default:
    {
        auto type = parseE0();
        targetBuffer += ";";
        return std::make_tuple(type, "E0");
    }
    }
}

/**
 * Tree := 'tree' (' Branch* ')'
 */
void
Parser2::parseTree()
{
    advanceAndCheckToken(TokenKind::TREE);   // eat 'tree'
    advanceAndCheckToken(TokenKind::LPAREN); // eat '('

    bool isFirstBranch = true;

    recursiveParse(true, TokenKind::RPAREN, [&isFirstBranch, this]() {
        if (isFirstBranch)
        {
            targetBuffer += "if";
            isFirstBranch = false;
        }
        else
            targetBuffer += "\n " + getTabs() + " else if ";
        this->parseBranch();
    });

    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
}

/**
 * Branch := '(' Grouping Block')'
 */
void
Parser2::parseBranch()
{
    advanceAndCheckToken(TokenKind::LPAREN); // eat '('

    parseGrouping();

    targetBuffer += "\n" + getTabs() + "{";

    symboltable->enterScope();
    parseBlock();
    symboltable->exitScope();

    targetBuffer += "\n" + getTabs() + "}";

    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
}

/**
 * Grouping := '|' E0 '|'
 *
 * Semantic Check:
 *  - resulting type is a bool
 */
void
Parser2::parseGrouping()
{
    inGrouping = true;
    targetBuffer += " (";
    advanceAndCheckToken(TokenKind::PIPE); // eat '|'

    auto type = parseE0();

    checkType("bool", type);

    targetBuffer += ") ";

    advanceAndCheckToken(TokenKind::PIPE); // eat '|'
    inGrouping = false;
}

/**
 * Return := 'return' T
 */
std::string
Parser2::parseReturn()
{
    advanceAndCheckToken(TokenKind::RETURN); // consume 'return'
    targetBuffer += "return ";

    return parseE0();
}

/**
 * Declaration :=  'dec' Variable '=' E0
 *
 * Semantic Analysis:
 *  - check that the declared type and the init type are the same
 */
std::string
Parser2::parseDeclaration()
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

    targetBuffer += outtype + " " + name + " = ";

    checkExists(type);

    auto ok = symboltable->addSymbol(name, type);
    if (!ok)
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Unexpected redeclaration of " + name +
                       ", originally defined as type " +
                       symboltable->getSymbolType(name) + ".",
                   "Received second declaration of type: ", type, ".")));

    decName = name;

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
Parser2::parseE0()
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
Parser2::parseE1(const std::string& T0)
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
Parser2::parseE2(const std::string& T0)
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
Parser2::parseE3(const std::string& T0)
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
Parser2::parseE4(const std::string& T0)
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
Parser2::parseE5(const std::string& T0)
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
Parser2::parseE6(const std::string& T0)
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
Parser2::parseE7(const std::string& T0)
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
Parser2::parseE8(const std::string& T0)
{

    if (currentToken->kind == TokenKind::NEGATION)
    {
        advanceAndCheckToken(TokenKind::NEGATION); // consume '!'
        targetBuffer += "!";
        auto type = parseE0();

        checkType("bool", type);

        return "bool";
    }

    if (currentToken->kind == TokenKind::UNARYADD)
    {
        advanceAndCheckToken(TokenKind::UNARYADD); // consume '++'
        targetBuffer += "++";
        auto type = parseE0();

        checkType("num", type);

        return type;
    }

    if (currentToken->kind == TokenKind::UNARYMINUS)
    {
        advanceAndCheckToken(TokenKind::UNARYMINUS); // consume '--'
        targetBuffer += "--";
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
Parser2::parseE9(const std::string& T0)
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
Parser2::parseE10(const std::string& T0)
{
    if (currentToken->kind == TokenKind::DOT ||
        currentToken->kind == TokenKind::TRIPLE_DOT)
    {
        auto type = parseMemberAccess(T0);

        return parseE1(type);
    }

    return parseE11(T0);
}

/**
 * E11 := New | E12
 */
std::string
Parser2::parseE11(const std::string& T0)
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
Parser2::parseE12(const std::string& T0)
{
    if (currentToken->kind == TokenKind::LPAREN)
    {
        targetBuffer += "(";
        inGrouping = true;
        checkExists(T0);

        auto name = T0;

        auto output = checkFunctionCall(name, symboltable);

        targetBuffer += ")";
        inGrouping = false;

        return output;
    }

    return parseE13(T0);
}

/**
 * E13 := T13 | ε
 */
std::string
Parser2::parseE13(const std::string& T11)
{
    return T11;
}

/**
 * MemberAccess := AttributeAccess | MethodAccess
 */
std::string
Parser2::parseMemberAccess(const std::string& T0)
{
    auto type = T0;
    if (type == "own")
    {
        if (isUdt)
            type = extractUDTName(filename);
        else
            errorhandler->handle(std::make_unique<Error2>(Error2(
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
        errorhandler->handle(std::make_unique<Error2>(Error2(
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
Parser2::parseAttributeAccess(const std::string& udtname,
                              const std::string& udtType)
{

    checkExists(udtType);
    checkUDTExists(udtType);
    // get udt's attribute symbol table
    auto st = udttable->getAttributeSymbolTable(udtType);

    advanceAndCheckToken(TokenKind::DOT); // consume '.'
    auto attribute = parseIdentifier();

    if (udtname != "own")
        targetBuffer += udtname;
    else
        targetBuffer += "this";

    targetBuffer += "->" + builtinTypesTranslator(attribute);

    // check if type exists
    if (!st->hasVariable(attribute))
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Nonexistent attribute.",
            "Nonexistent attribute named: ", attribute, ".")));
    }

    return st->getSymbolType(attribute);
}

/**
 * MethodAccess := '...' Identifier FunctionCall
 */
std::string
Parser2::parseMethodAccess(const std::string& udtname,
                           const std::string& udtType)
{
    methodAccessName = udtname;

    if (methodAccessName == "own")
        methodAccessName = "this";

    checkExists(udtType);
    checkUDTExists(udtType);

    // get udt's method symbol table
    auto st = udttable->getMethodSymbolTable(udtType);

    advanceAndCheckToken(TokenKind::TRIPLE_DOT); // consume '...'

    auto methodName = parseIdentifier();

    targetBuffer += methodName + "(";

    inGrouping = true;

    auto output = checkFunctionCall(methodName, st);

    this->targetBuffer += ")";

    inGrouping = false;

    methodAccessName = "";

    return output;
}

/**
 * FunctionCall := '(' [Identifier [',' Identifier]*] ')'
 */
std::string
Parser2::parseFunctionCall()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren

    std::string types = "(";
    int nonVoidInputs = 0;
    recursiveParse(true, TokenKind::RPAREN, [&nonVoidInputs, &types, this]() {
        if (nonVoidInputs)
            targetBuffer += ", ";

        auto type = parseE0();

        if (symboltable->hasVariable(type))
            type = symboltable->getSymbolType(type);

        if (type != "void")
            ++nonVoidInputs;

        types += "_" + type;
    });

    if (methodAccessName != "")
        if (nonVoidInputs == 0)
            targetBuffer += methodAccessName;
        else
            targetBuffer += ", " + methodAccessName;

    types += ")";

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    return types;
}

/**
 * New := UDTDec
 */
std::string
Parser2::parseNew()
{
    advanceAndCheckToken(TokenKind::NEW); // consume new
    switch (currentToken->kind)
    {
    case TokenKind::IDENTIFIER:
    {
        return parseUDTDec();
    }
    default:
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
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
Parser2::parseUDTDec()
{

    auto udtName = parseIdentifier();
    targetBuffer +=
        "(struct " + udtName + "*)malloc(sizeof(struct " + udtName + "));\n";

    checkExists(udtName);
    checkUDTExists(udtName);

    auto st = udttable->getAttributeSymbolTable(udtName);
    auto attributes = st->getSymbols();

    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    recursiveParse(
        true, TokenKind::RCURLEY, [st, udtName, &attributes, this]() {
            // capture key
            auto attributeName = parseIdentifier();

            this->targetBuffer +=
                getTabs() + decName + "->" + attributeName + " = ";

            advanceAndCheckToken(TokenKind::COLON); // consume ':'

            // capture value
            auto type = parsePrimary();

            if (attributes.size() != 1)
                this->targetBuffer += ";\n";

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
                semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "Unrecognized initialization key for new udt of type: " +
                        udtName,
                    "Unrecognized key: ", attributeName, ".")));
        });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley

    if (attributes.size() != 0)
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
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
Parser2::parseT()
{
    if (currentToken->kind == TokenKind::LPAREN)
    {
        advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
        targetBuffer += "(";
        auto type = parseE0();
        advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
        targetBuffer += ")";
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
Parser2::parsePrimary()
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

        if (methodAccessName == "" ||
            (methodAccessName != "" && type != "void"))
            if (!udttable->hasUDT(symboltable->getSymbolType(type)))
                targetBuffer += builtinTypesTranslator(type);

        return type;
    }
    case TokenKind::LIST:
        return parseList();
    default:
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
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
Parser2::parseType()
{
    return currentToken->kind == TokenKind::LISTTYPE ? parseListType()
                                                     : parseIdentifier();
}

/**
 * Variable := Type Identifier
 */
std::tuple<std::string, std::string>
Parser2::parseVariable()
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
Parser2::parseNumber()
{
    auto v = currentToken->value;
    auto k = currentToken->kind;

    if (k == TokenKind::INTEGER)
    {
        advanceAndCheckToken(TokenKind::INTEGER); // eat integer
        targetBuffer += v;
        return v;
    }

    advanceAndCheckToken(TokenKind::FLOAT); // eat float
    targetBuffer += v;
    return v;
}

/**
 * Identifier := lexvalue
 */
std::string
Parser2::parseIdentifier()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::IDENTIFIER); // eat identifier
    return v;
}

/**
 * Bool := lexvalue
 */
std::string
Parser2::parseBoolean()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::BOOL); // eat identifier
    targetBuffer += (v == "true" ? "1" : "0");

    return v;
}

/**
 * String := lexvalue
 */
std::string
Parser2::parseString()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // true eat string
    targetBuffer += v;
    return v;
}

/**
 * Own Accessor:= lexvalue
 */
std::string
Parser2::parseOwnAccessor()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::OWN_ACCESSOR); // eat own accessor

    if (isUdt)
        return extractUDTName(filename);
    else
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "illegal usage of own in a non udt method.", "", "", "")));
    return v; // will not ever reach here
}

/**
 * Empty:= lexvalue
 */
std::string
Parser2::parseEmpty()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::EMPTY); // eat own accessor

    targetBuffer += "NULL";

    return v; // will not ever reach here
}

/**
 * ListType := lexvalue
 */
std::string
Parser2::parseListType()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LISTTYPE); // eat list type
    return v;
}

// --------       Some helpers for parsing a list       -------- //

std::vector<std::unique_ptr<Token2>>
determineTypes(const std::string& s)
{
    auto lexar = std::make_unique<Lexar2>(s, false);
    std::vector<std::unique_ptr<Token2>> vals;
    auto token = lexar->getNextToken();
    while (token->kind != TokenKind::EOF_)
    {
        vals.push_back(std::move(token));
        token = lexar->getNextToken();
    }

    return vals;
}

std::string
Parser2::tokenToType(const TokenKind& tk, const std::string& val)
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
        errorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Unexpected type in a list.",
            "Type: ", displayKind(tk), "")));
        return ""; // unreachable
    }
}

/**
 * List := lexvalue
 */
std::string
Parser2::parseList()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LIST); // eat list
    auto listVals = determineTypes(parseListValues(v));

    std::string type = "none";

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

    std::string buf = "(" + builtinTypesTranslator(type) + "*)malloc(sizeof(" +
                      builtinTypesTranslator(type) + ") * " +
                      std::to_string(listVals.size()) + ");\n";

    for (int i = 0; i < vals.size(); i++)
    {
        auto v = vals.at(i);
        buf +=
            getTabs() + decName + "[" + std::to_string(i) + "] = " + v + ";\n";
    }

    buf = buf.substr(0, buf.size() - 2);

    targetBuffer += buf;

    return type;
}

void
Parser2::transpile()
{
    output.close();
}