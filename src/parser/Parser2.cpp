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
        auto n = p->parse();
        return std::make_tuple(std::move(p->getUDTTable()), p->getIsUDTFlag(),
                               p->getUDTBuffer());
    }
    catch (const std::string msg)
    {
        std::cerr << msg;
    }
    catch (char const* msg)
    {
        std::cerr << msg;
    }
}

std::string
extractUDTName(const std::string& s)
{
    auto front = s.find_last_of('/') + 1;
    auto back = s.find_last_of('.') - front;
    return s.substr(front, back);
}

LandS
Parser2::extractType(Lexeme l)
{
    if (l.index() == 0)
    {
        auto leaf = std::get<LeafPtr>(std::move(l));
        return std::make_tuple(std::move(leaf), disp(leaf->lit));
    }

    if (l.index() == 1)
    {
        auto node = std::get<NodePtr>(std::move(l));
        return std::make_tuple(std::move(node), disp(node->op));
    }
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
    {
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Expected a token of type: " + displayKind(k),
                   "Received: ", currentToken->value,
                   " of type " + displayKind(currentToken->kind) + ".")));
    }
    currentToken = lexar->getNextToken();

    // catch errors from the lexar
    if (currentToken->kind == TokenKind::ERROR)
    {
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line, "Lexar Error.",
                   "Error: ", currentToken->value, "")));
    }

    while (currentToken->kind == TokenKind::COMMENT ||
           currentToken->kind == TokenKind::COMMA)
    {
        currentToken = lexar->getNextToken();

        // catch errors from the lexar
        if (currentToken->kind == TokenKind::ERROR)
        {
            errorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line, "Lexar Error.",
                       "Error: ", currentToken->value, "")));
        }
    }
}

// -------- Semantic Analysis Helper Code --------- //
void
Parser2::checkType(const std::string& t0, const std::string& t1)
{
    auto left = t0;
    auto right = t1;

    if (symboltable->hasVariable(left))
        left = symboltable->getSymbolType(left);

    if (symboltable->hasVariable(right))
        right = symboltable->getSymbolType(right);

    // should actually check types here
    if (left.at(0) == '[')
    {
        left = extractListType(left);

        if (right.at(0) == '[')
        {
            right = extractListType(right);
        }

        if (t1 != "none" && left != right)
        {
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Mismatched list types. Expected is: " + left + ".",
                       "Received is: ", "[" + right + "]", ".")));
        }
    }
    else if (right.at(0) == '[')
    {
        right = extractListType(right);

        if (left.at(0) == '[')
        {
            left = extractListType(left);
        }

        if (t1 != "none" && right != left)
        {
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Mismatched list types. Expected is: " + left + ".",
                       "Received is: ", "[" + right + "]", ".")));
        }
    }
    else if (left == "num")
    {
        if ("int" != right && "flt" != right)
        {
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Mismatched types. Expected/LeftHand is: int or flt.",
                       "Received/Right Hand is: ", right, ".")));
        }
    }

    else if (left != right && right != "empty")
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Mismatched types. Expected/LeftHand is: " + t0 + ".",
                   "Received/Right Hand is: ", right, ".")));
    }
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
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Illegal redeclaration.",
            "Redeclared variable named: ", s, ".")));
    }
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
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Unknown variable or type.",
            "Unknown variable/type named: ", type, ".")));
    }
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
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line, "Unknown udt type.",
                   "Unknown type named: ", udtname, ".")));
    }
}

LandS
Parser2::checkFunctionCall(const std::string& name,
                           std::shared_ptr<SymbolTable> st)
{
    auto a = parseFunctionCall();
    auto fc = std::move(std::get<0>(a));
    auto fcInputs = parseFunctionInputTypes(std::get<1>(a));

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

    return std::make_tuple(std::move(fc), output);
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
    {
        inputs.push_back(segment);
    }

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
    {
        inputs += " " + segment;
    }

    return inputs + " "; // so that we don't get EOF before done parsing
}

// constructor
Parser2::Parser2(const std::string& file)
{
    filename = file;
    lexar = std::make_unique<Lexar2>(file, true);
    currentToken = lexar->getNextToken();
    errorhandler = std::make_unique<Parser2ErrorHandler>(Parser2ErrorHandler());
    semanticerrorhandler = std::make_unique<SemanticAnalyzerErrorHandler>(
        SemanticAnalyzerErrorHandler(file));
    symboltable = std::make_shared<SymbolTable>(SymbolTable());
    udttable = std::make_unique<UDTTable>(UDTTable());
    isUdt = false;
}

// public interface method
NodePtr
Parser2::parse()
{
    auto program = parseProgram();
    return program;
}

// -------- Parser Parse Methods --------- //
/**
 * Program := Source
 */
NodePtr
Parser2::parseProgram()
{
    return parseSource();
}
/**
 * Source := Import Source | SourcePart
 */
NodePtr
Parser2::parseSource()
{
    auto imports =
        getChain(false, TokenKind::IMPORT, OP::IMPORT,
                 [this]() -> NodePtr { return this->parseImportInfo(); });
    auto sourcepart = parseSourcePart();
    return makeNode(OP::SOURCE, std::move(imports), std::move(sourcepart));
}

/**
 * ImportInfo := UDName Location
 *
 * Semantic Checks:
 *      - import file exists
 *      - import is a udt
 */
NodePtr
Parser2::parseImportInfo()
{
    advanceAndCheckToken(TokenKind::IMPORT); // eat 'Import'
    auto name = parseUDName();
    advanceAndCheckToken(TokenKind::COLON); // eat colon
    auto loc = parseLocation();

    auto file = loc->value.substr(1, loc->value.size() - 2);

    try
    {
        auto udtFlagAndBufer = parseFile(file);

        auto table = std::move(std::get<0>(udtFlagAndBufer));
        auto flag = std::get<1>(udtFlagAndBufer);
        auto buf = std::get<2>(udtFlagAndBufer);

        if (!flag)
            errorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Expected imported file of type UDT",
                       "Received: ", "\"" + file + "\"", " of type script")));

        // add to own udt table under imported name, throwing an error if the
        // name already exists
        if (!udttable->hasUDT(name->value))
            udttable->addUDT(
                name->value,
                table->getAttributeSymbolTable(extractUDTName(file)),
                table->getMethodSymbolTable(extractUDTName(file)));

        symboltable->addSymbol(name->value, "U");

        // aggregate udt buffers
        udtBuffer += buf;
    }
    catch (char const* msg)
    {
        throw msg;
    }

    return makeNode(OP::IMPORT, std::move(name), std::move(loc));
}

/**
 * UDName := Identifier
 */
LeafPtr
Parser2::parseUDName()
{
    return parseIdentifier();
}

/**
 * Location := String
 */
LeafPtr
Parser2::parseLocation()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // true eat string
    return makeLeaf(LIT::STRING, v);
}

/**
 * SourcePart := UDT | Script
 */
NodePtr
Parser2::parseSourcePart()
{
    switch (currentToken->kind)
    {
    case TokenKind::UAT:
        isUdt = true;
        return parseUDT();
    default:
    {
        // capture and open file only if not a udt so that imports do not
        // override
        output.clear();
        output.open("out.c");

        // set the file header here
        output << OUTPUT_HEADER;

        // add the buffers from parsed udt files
        output << udtBuffer;
        return parseScript();
    }
    }
}

/**
 * UDT := UserDefinedType
 */
NodePtr
Parser2::parseUDT()
{
    return parseUserDefinedType();
}

/**
 * UserDefinedType := Attributes Methods
 */
NodePtr
Parser2::parseUserDefinedType()
{
    auto udtname = extractUDTName(filename);
    udttable->addUDT(udtname);

    udtBuffer +=
        "\n//___________BEGIN_" + udtname + "_UDT_DEFINITION__________/_//\n\n";

    udtBuffer += "struct " + udtname + "\n{\n";
    auto atlandst = this->parseAttributes();
    auto topattribute = std::move(std::get<0>(atlandst));
    std::shared_ptr<SymbolTable> attributest = std::move(std::get<1>(atlandst));
    udtBuffer += "};\n";

    udttable->updateUDT(udtname, attributest,
                        std::make_shared<SymbolTable>(SymbolTable()));

    auto mlandst = this->parseMethods();
    auto topmethod = std::move(std::get<0>(mlandst));
    std::shared_ptr<SymbolTable> methodst = std::move(std::get<1>(mlandst));
    udttable->updateUDT(udtname, attributest, methodst);

    udtBuffer +=
        "//___________END_" + udtname + "_UDT_DEFINITION__________/_//\n\n";

    return makeNode(OP::UDT, std::move(topattribute), std::move(topmethod));
}

/**
 * Attributes := 'Uat' [Identifier Identifier]*
 *
 * Semantic Checks:
 *      - type exists
 *      - unique attribute
 */
LandSt
Parser2::parseAttributes()
{
    advanceAndCheckToken(TokenKind::UAT);     // consume uat
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley

    std::shared_ptr<SymbolTable> st =
        std::make_shared<SymbolTable>(SymbolTable());

    st->clear();
    st->addSymbol(extractUDTName(filename), "U");

    auto topattribute = getChain(
        true, TokenKind::RCURLEY, OP::ATTRIBUTE, [&st, this]() -> Lexeme {
            auto lands = this->parseVariable();

            auto attribute = std::move(std::get<0>(lands));
            auto name = std::get<1>(lands);
            auto type = std::get<2>(lands);

            auto outtype = type;
            if (st->hasVariable(outtype) && st->getSymbolType(outtype) == "U")
                outtype = "struct " + outtype + "*";
            else
                outtype = builtinTypesTranslator(outtype);

            udtBuffer += "\t" + outtype + " " + name + ";\n";

            // check if unique name
            if (st->hasVariable(name))
            {
                semanticerrorhandler->handle(std::make_unique<Error2>(
                    Error2(currentToken->col, currentToken->line,
                           "Illegal redeclaration.",
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
                    errorhandler->handle(std::make_unique<Error2>(
                        Error2(currentToken->col, currentToken->line,
                               "illegal usage of own in a non udt method.", "",
                               "", "")));
            }

            if (!st->hasVariable(type) && !isPrimitive(type))
            {
                semanticerrorhandler->handle(std::make_unique<Error2>(
                    Error2(currentToken->col, currentToken->line,
                           "Unknown variable or type.",
                           "Unknown variable/type named: ", type, ".")));
            }

            auto ok = st->addSymbol(name, type);
            if (!ok)
                semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "Unexpected redeclaration of " + name +
                        ", originally defined as type " +
                        st->getSymbolType(name) + ".",
                    "Received second declaration of type: ", type, ".")));

            return attribute;
        });

    st->removeSymbol(extractUDTName(filename));

    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return std::make_tuple(makeNode(OP::ATTRIBUTE, std::move(topattribute)),
                           st);
}

/**
 * Methods := 'Ufn' [FunctionDefinition]*
 */
LandSt
Parser2::parseMethods()
{
    advanceAndCheckToken(TokenKind::UFN);     // consume ufn
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley

    // temporarily capture methods into symbol table for udt
    std::shared_ptr<SymbolTable> st;
    std::shared_ptr<SymbolTable> temp = symboltable;
    symboltable = std::make_shared<SymbolTable>(SymbolTable());

    // add udt so it can reference self (or own in Sailfish lingo)
    symboltable->addSymbol(extractUDTName(filename), "U");

    auto topmethod =
        getChain(true, TokenKind::RCURLEY, OP::METHOD, [this]() -> NodePtr {
            return this->parseFunctionDefinition();
        });

    // set back the normal symbol table and capture temp table
    st = symboltable;
    symboltable = temp;

    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return std::make_tuple(makeNode(OP::METHOD, std::move(topmethod)), st);
}

NodePtr
Parser2::parseScript()
{
    return parseScript_();
}

NodePtr
Parser2::parseScript_()
{
    auto func =
        getChain(true, TokenKind::START, OP::FUNCTION, [this]() -> NodePtr {
            return this->parseFunctionDefinition();
        });
    auto start = parseStart();
    return makeNode(OP::SCRIPT, std::move(func), std::move(start));
}

/**
 * FunctionDefinition := '(' 'fun' identifier FunctionInfo ')'
 *
 *  * Semantic Checks:
 *      - function is a unique declaration
 */
NodePtr
Parser2::parseFunctionDefinition()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    advanceAndCheckToken(TokenKind::FUN);    // consume funenterScope

    // parse left child
    auto id = parseIdentifier();

    // parse right child
    auto lands = parseFunctionInfo(id->value);
    auto lexeme = std::move(std::get<0>(lands));
    // auto type = std::get<1>(lands);

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION, std::move(id), std::move(lexeme));
}

/**
 * FunctionInfo := FunctionInOut Block
 *
 * Semantic Check:
 *  - actual return type matches expected return type
 */
LandS
Parser2::parseFunctionInfo(const std::string& name)
{
    auto lands = parseFunctionInOut(name);
    auto fintout = std::move(std::get<0>(lands));
    auto type = std::get<1>(lands);

    type = "F" + type;

    // add to symbol table
    auto ok = symboltable->addSymbol(name, type);
    if (!ok)
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Unexpected redeclaration of " + name +
                       ", originally defined as type " +
                       symboltable->getSymbolType(name) + ".",
                   "Received second declaration of type: ", type, ".")));

    output << "{";
    udtBuffer += "{";
    auto a = parseBlock();
    auto block = std::move(std::get<0>(a));
    auto returnType = std::get<1>(a);
    output << "\n}\n\n";
    udtBuffer += "\n}\n\n";

    checkType(returnType, parseFunctionReturnType(type));

    return std::make_tuple(
        makeNode(OP::FUNCTION_INFO, std::move(fintout), std::move(block)),
        type);
}

/**
 * FunctionInOut := FunctionInputs* FunctionOutputs
 * FunctionInputs :=  '(' Variable [',' Variable]* ')'
 *
 * Semantic Checks:
 *  - check for more than one argument after a declaration of void or that void
 * is first declaration as part of formals
 */
LandS
Parser2::parseFunctionInOut(const std::string& name)
{
    // inputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
                                             // TODO: check for multiple voids

    std::string types = "(";
    bool seenVoid = false;
    int argCount = 0;
    std::string outputBuffer = "";
    auto topinput = getChain(
        true, TokenKind::RPAREN, OP::FUNCTION_INPUT,
        [&outputBuffer, &types, &seenVoid, &argCount, this]() -> Lexeme {
            ++argCount;

            auto lands = this->parseVariable();
            auto inp = std::move(std::get<0>(lands));
            auto name = std::get<1>(lands);
            auto type = std::get<2>(lands);

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

            auto ok = symboltable->addSymbol(std::get<1>(lands), type);
            if (!ok)
                semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "Unexpected redeclaration of " + name +
                        ", originally defined as type " +
                        symboltable->getSymbolType(name) + ".",
                    "Received second declaration of type: ", type, ".")));

            types += "_" + type;
            return inp;
        });
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    // outputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    auto output = parseType();
    types += ")" + output.get()->value;
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    if (isUdt)
        if (outputBuffer != "")
            outputBuffer += ", struct " + extractUDTName(filename) + "* this";
        else
            outputBuffer = "struct " + extractUDTName(filename) + "* this";
    outputBuffer = output->value + "\n" + name + "(" + outputBuffer + ")\n";
    udtBuffer += outputBuffer;
    this->output << outputBuffer;

    return std::make_tuple(
        makeNode(OP::FUNCTION_IN_OUT, std::move(topinput), std::move(output)),
        types);
}

/**
 * Start := 'start' Block
 */
NodePtr
Parser2::parseStart()
{
    advanceAndCheckToken(TokenKind::START);

    output << "int\nmain()\n{";

    auto a = parseBlock();

    output << "\n    return 1;\n}";

    auto block = std::move(std::get<0>(a));
    // auto type = std::get<1>(a); -- ignore type

    return makeNode(OP::START, makeLeaf(LIT::IDENTIFIER, "start"),
                    std::move(block));
}

/**
 * Block = '{' Statement* '}'
 *
 * Semantic Checks:
 *  - has exactly one RETURN statement
 *
 * Notes: defaults to void return type
 */
LandS
Parser2::parseBlock()
{
    ++currentTabs;
    std::string type = "void";
    bool hasSeenReturn = false;
    advanceAndCheckToken(TokenKind::LCURLEY); // eat '{'

    symboltable->enterScope();

    auto topstatement = getChain(
        true, TokenKind::RCURLEY, OP::STATEMENT,
        [&type, &hasSeenReturn, this]() -> Lexeme {
            auto a = this->parseStatement();
            auto statement = std::move(std::get<0>(a));

            if (std::get<2>(a) == "RETURN")
            {
                if (hasSeenReturn)
                {
                    semanticerrorhandler->handle(std::make_unique<Error2>(
                        Error2(currentToken->col, currentToken->line,
                               "Illegal multiple definitions of return.", "",
                               "", "")));
                }
                else
                {
                    type = std::get<1>(a);
                    if (symboltable->hasVariable(type))
                        type = symboltable->getSymbolType(type);
                    hasSeenReturn = true;
                }
            }

            return std::move(statement);
        });
    symboltable->exitScope();

    advanceAndCheckToken(TokenKind::RCURLEY); // eat '}'
    --currentTabs;
    return std::make_tuple(makeNode(OP::BLOCK, std::move(topstatement)), type);
}

/**
 * Statement := Tree | Return | Declaration | E0
 */
LSandS
Parser2::parseStatement()
{
    output << "\n" + getTabs();
    udtBuffer += "\n" + getTabs();
    switch (currentToken->kind)
    {
    case TokenKind::TREE:
        return std::make_tuple(std::move(parseTree()), "tree", "TREE");
    case TokenKind::RETURN:
    {
        auto a = parseReturn();
        auto ret = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(std::move(ret), type, "RETURN");
    }
    case TokenKind::DEC:
    {
        auto a = parseDeclaration();
        auto dec = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(std::move(dec), type, "DEC");
    }
    default:
    {
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(std::move(e0), type, "E0");
    }
    }
}

/**
 * Tree := 'tree' (' Branch* ')'
 */
NodePtr
Parser2::parseTree()
{

    advanceAndCheckToken(TokenKind::TREE);   // eat 'tree'
    advanceAndCheckToken(TokenKind::LPAREN); // eat '('

    bool isFirstBranch = true;

    auto topbranch = getChain(true, TokenKind::RPAREN, OP::BRANCH,
                              [&isFirstBranch, this]() -> NodePtr {
                                  if (isFirstBranch)
                                  {
                                      output << "if";
                                      udtBuffer += "if";
                                      isFirstBranch = false;
                                  }
                                  else
                                  {
                                      output << "\n" + getTabs() + "else if";
                                      udtBuffer +=
                                          "\n " + getTabs() + " else if ";
                                  }
                                  return this->parseBranch();
                              });

    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
    return makeNode(OP::TREE, std::move(topbranch));
}

/**
 * Branch := '(' Grouping Block')'
 */
NodePtr
Parser2::parseBranch()
{
    advanceAndCheckToken(TokenKind::LPAREN); // eat '('

    auto grouping = parseGrouping();

    output << "\n" + getTabs() + "{";
    udtBuffer += "\n" + getTabs() + "{";
    auto a = parseBlock();
    output << "\n" + getTabs() + "}";
    udtBuffer += "\n" + getTabs() + "}";

    auto block = std::move(std::get<0>(a));
    // auto type = std::get<1>(a); -- ignore type

    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
    return makeNode(OP::BRANCH, std::move(grouping), std::move(block));
}

/**
 * Grouping := '|' E0 '|'
 *
 * Semantic Check:
 *  - resulting type is a bool
 */
Lexeme
Parser2::parseGrouping()
{
    inGrouping = true;
    output << " (";
    udtBuffer += " (";
    advanceAndCheckToken(TokenKind::PIPE); // eat '|'

    auto a = parseE0();
    auto e0 = std::move(std::get<0>(a));
    auto type = std::get<1>(a);

    checkType("bool", type);

    output << ") ";
    udtBuffer += ") ";

    advanceAndCheckToken(TokenKind::PIPE); // eat '|'
    inGrouping = false;
    return e0;
}

/**
 * Return := 'return' T
 */
LandS
Parser2::parseReturn()
{
    advanceAndCheckToken(TokenKind::RETURN); // consume 'return'
    output << "return ";
    udtBuffer += "return ";

    auto a = parseE0();
    auto e0 = std::move(std::get<0>(a));
    auto type = std::get<1>(a);
    return std::make_tuple(makeNode(OP::RETURN, std::move(e0)), type);
}

/**
 * Declaration :=  'dec' Variable '=' E0
 *
 * Semantic Analysis:
 *  - check that the declared type and the init type are the same
 */
LandS
Parser2::parseDeclaration()
{
    advanceAndCheckToken(TokenKind::DEC); // consume 'dec'

    auto lsands = parseVariable();
    auto var = std::move(std::get<0>(lsands));
    auto name = std::get<1>(lsands);
    auto type = std::get<2>(lsands);

    auto outtype = type;

    // check to see if it is a udt name first
    if (udttable->hasUDT(outtype))
        outtype = "struct " + outtype + "*";
    else
        outtype = builtinTypesTranslator(outtype);

    output << outtype + " " + name + " = ";
    udtBuffer += outtype + " " + name + " = ";

    checkExists(type);

    decName = name;

    auto ok = symboltable->addSymbol(name, type);
    if (!ok)
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Unexpected redeclaration of " + name +
                       ", originally defined as type " +
                       symboltable->getSymbolType(name) + ".",
                   "Received second declaration of type: ", type, ".")));

    advanceAndCheckToken(TokenKind::ASSIGNMENT); // consume '='
    auto a = parseE0();
    auto e0 = std::move(std::get<0>(a));
    auto ta = std::get<1>(a);

    if (name.at(0) == '[')
        checkType(name, ta);
    else
        checkType(type, ta);

    return std::make_tuple(
        makeNode(OP::DECLARATION, std::move(var), std::move(e0)), type);
}

/**
 * E0 := T0 E1
 */
LandS
Parser2::parseE0()
{
    auto a = parseT();
    auto t0 = std::move(std::get<0>(a));
    auto typea = std::get<1>(a);

    auto b = parseE1(typea);
    auto e1 = std::move(std::get<0>(b));
    auto typeb = std::get<1>(b);

    return std::make_tuple(std::move(e1), typeb);
}

/**
 * E1 := ** E0 | E2
 *
 * Semantic Check
 *  - both are int
 */
LandS
Parser2::parseE1(const std::string& T0)
{

    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE2(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            this->checkType(T0, T1);
            this->checkType("int", T0);
            this->checkType("int", T1);
            return T1;
        },
        std::make_tuple(TokenKind::EXPONENTIATION, OP::EXPONENT, "**"));
}

/**
 * E2 := * E0 | / E0 | % E0 | E3
 *
 * Semantic Check
 *  - both are num (int or flt) and the same type
 */
LandS
Parser2::parseE2(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE3(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are same
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return T1;
        },
        std::make_tuple(TokenKind::MULTIPLICATION, OP::MULTIPLICATION, "*"),
        std::make_tuple(TokenKind::DIVISION, OP::DIVISION, "/"),
        std::make_tuple(TokenKind::MODULO, OP::MODULO, "%"));
}

/**
 * E3 := + E0 | - E0 | E4
 *
 * Semantic Check
 *  - both are num (int or flt) and the same type
 */
LandS
Parser2::parseE3(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE4(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are same
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return T1;
        },
        std::make_tuple(TokenKind::ADDITION, OP::ADDITION, "+"),
        std::make_tuple(TokenKind::SUBTRACTION, OP::SUBTRACTION, "-"));
}

/**
 * E4 := < E0 | > E0 | <= E0 | >= E0 | E5
 *
 * Semantic Check
 *  - both are num (int or flt) and the same type
 */
LandS
Parser2::parseE4(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE5(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are bool
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return "bool";
        },
        std::make_tuple(TokenKind::LESS_THAN, OP::LESS_THAN, "<"),
        std::make_tuple(TokenKind::LESS_THAN_OR_EQUALS, OP::LESS_THAN_OR_EQUALS,
                        "<="),
        std::make_tuple(TokenKind::GREATER_THAN, OP::GREATER_THAN, ">"),
        std::make_tuple(TokenKind::GREATER_THAN_OR_EQUALS,
                        OP::GREATER_THAN_OR_EQUALS, ">="));
}

/**
 * E5 := == E0 | != E0 | E6
 *
 * Semantic Check
 *  - both are the same type
 */
LandS
Parser2::parseE5(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE6(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are bool
            this->checkType(T0, T1);
            return "bool";
        },
        std::make_tuple(TokenKind::EQUIVALENCE, OP::EQUIVALENCE, "=="),
        std::make_tuple(TokenKind::NONEQUIVALENCE, OP::NONEQUIVALENCE, "!="));
}

/**
 * E6 := 'and' E0 | 'or' E0 | E7
 *
 * Semantic Check
 *  - both are bool
 */
LandS
Parser2::parseE6(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE7(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are bool
            this->checkType(T0, T1);
            this->checkType("bool", T0);
            this->checkType("bool", T1);
            return "bool";
        },
        std::make_tuple(TokenKind::OR, OP::OR, "||"),
        std::make_tuple(TokenKind::AND, OP::AND, "&&"));
}

/**
 * E7 := = E0 | E8
 *
 * Semantic Check
 *  - the variable has already been declared
 *  - the lhs matches the rhs
 */
LandS
Parser2::parseE7(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE8(T0); },
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

        std::make_tuple(TokenKind::ASSIGNMENT, OP::ASSIGNMENT, "="));
}

/**
 * E8 := [!, ++, --] E0 | E9
 *
 * Semantic Check
 *  - check that ! is bool and ++/-- are num
 */
LandS
Parser2::parseE8(const std::string& T0)
{

    if (currentToken->kind == TokenKind::NEGATION)
    {
        advanceAndCheckToken(TokenKind::NEGATION); // consume '!'
        output << "!";
        udtBuffer += "!";
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);

        checkType("bool", type);

        return std::make_tuple(makeNode(OP::NEGATION, T0, std::move(e0)),
                               "bool");
    }

    if (currentToken->kind == TokenKind::UNARYADD)
    {
        advanceAndCheckToken(TokenKind::UNARYADD); // consume '++'
        output << "++";
        udtBuffer += "++";
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);

        checkType("num", type);

        return std::make_tuple(makeNode(OP::UNARYADD, T0, std::move(e0)), type);
    }

    if (currentToken->kind == TokenKind::UNARYMINUS)
    {
        advanceAndCheckToken(TokenKind::UNARYMINUS); // consume '--'
        output << "--";
        udtBuffer += "--";
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);

        checkType("num", type);

        return std::make_tuple(makeNode(OP::UNARYMINUS, T0, std::move(e0)),
                               type);
    }

    auto b = parseE9(std::move(T0));
    auto t0 = std::move(std::get<0>(b));
    auto type = std::get<1>(b);

    return std::make_tuple(std::move(t0), type);
}

/**
 * E9 := [+=, -=, /=, *=] E0 | E10
 *
 * Semantic Check
 *  - both are num and the same type
 */
LandS
Parser2::parseE9(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE10(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are the same
            this->checkType(T0, T1);
            this->checkType("num", T0);
            this->checkType("num", T1);
            return T1;
        },
        std::make_tuple(TokenKind::ADDTO, OP::ADDTO, "+="),
        std::make_tuple(TokenKind::SUBFROM, OP::SUBFROM, "-="),
        std::make_tuple(TokenKind::DIVFROM, OP::DIVFROM, "/="),
        std::make_tuple(TokenKind::MULTTO, OP::MULTTO, "*="));
}

/**
 * E10 := MemberAccess E0 | E11
 */
LandS
Parser2::parseE10(const std::string& T0)
{
    if (currentToken->kind == TokenKind::DOT ||
        currentToken->kind == TokenKind::TRIPLE_DOT)
    {
        auto a = parseMemberAccess(T0);
        auto member = std::move(std::get<0>(a));
        auto typea = std::get<1>(a);

        auto b = parseE1(typea);
        auto e0 = std::move(std::get<0>(b));
        auto typeb = std::get<1>(b);

        return std::make_tuple(std::move(e0), typeb);
    }

    auto b = parseE11(std::move(T0));
    auto t10 = std::move(std::get<0>(b));
    auto type = std::get<1>(b);

    return std::make_tuple(std::move(t10), type);
}

/**
 * E11 := New | E12
 */
LandS
Parser2::parseE11(const std::string& T0)
{
    if (currentToken->kind == TokenKind::NEW)
    {
        auto a = parseNew();
        auto New = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(makeNode(OP::MEMBER, std::move(New)), type);
    }

    auto b = parseE12(std::move(T0));
    auto t11 = std::move(std::get<0>(b));
    auto type = std::get<1>(b);

    return std::make_tuple(std::move(t11), type);
}

/**
 * E12 := FunctionCall | E13
 */
LandS
Parser2::parseE12(const std::string& T0)
{
    if (currentToken->kind == TokenKind::LPAREN)
    {

        this->output << "(";
        inGrouping = true;
        checkExists(T0);

        auto name = T0;

        auto a = checkFunctionCall(name, symboltable);
        auto fc = std::move(std::get<0>(a));
        auto output = std::get<1>(a);

        this->output << ");";
        inGrouping = false;

        // return std::make_tuple(
        //     makeNode(OP::METHOD_ACCESS, std::move(fc), std::move(name)),
        //     output);

        return std::make_tuple(makeNode(OP::FUNCTION_CALL, std::move(fc)),
                               output);
    }

    auto b = parseE13(T0);
    auto t11 = std::move(std::get<0>(b));
    auto type = std::get<1>(b);

    return std::make_tuple(std::move(t11), type);
}

/**
 * E13 := T13 | ε
 */
LandS
Parser2::parseE13(const std::string& T11)
{
    if (!inGrouping)
    {
        output << ";";
        udtBuffer += ";";
    }
    return std::make_tuple(makeNullNode(), T11);
}

/**
 * MemberAccess := AttributeAccess | MethodAccess
 */
LandS
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
        return parseAttributeAccess(type);
    case TokenKind::TRIPLE_DOT:
        return parseMethodAccess(type);
    default:
        errorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Expected a ... or . token.",
            "Received: ", currentToken->value,
            " of type " + displayKind(currentToken->kind) + ".")));
        return std::make_tuple(std::move(makeNullNode()), ""); //  unreachable
    }
}

/**
 * AttributeAccess := '.' Identifier
 */
LandS
Parser2::parseAttributeAccess(const std::string& udtType)
{
    checkExists(udtType);
    checkUDTExists(udtType);
    // get udt's attribute symbol table
    auto st = udttable->getAttributeSymbolTable(udtType);

    advanceAndCheckToken(TokenKind::DOT); // consume '.'
    auto attribute = parseIdentifier();
    output << "->" + builtinTypesTranslator(attribute->value);
    udtBuffer += "->" + builtinTypesTranslator(attribute->value);

    // check if type exists
    if (!st->hasVariable(attribute->value))
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Nonexistent attribute.",
            "Nonexistent attribute named: ", attribute->value, ".")));
    }

    return std::make_tuple(makeNode(OP::ATTRIBUTE_ACCESS, std::move(attribute)),
                           st->getSymbolType(attribute->value));
}

/**
 * MethodAccess := '...' Identifier FunctionCall
 */
LandS
Parser2::parseMethodAccess(const std::string& udtType)
{
    checkExists(udtType);
    checkUDTExists(udtType);

    // get udt's attribute symbol table
    auto st = udttable->getMethodSymbolTable(udtType);

    advanceAndCheckToken(TokenKind::TRIPLE_DOT); // consume '...'

    auto name = parseIdentifier();

    auto a = checkFunctionCall(name->value, st);
    auto fc = std::move(std::get<0>(a));
    auto output = std::move(std::get<1>(a));

    return std::make_tuple(
        makeNode(OP::METHOD_ACCESS, std::move(fc), std::move(name)), output);
}

/**
 * FunctionCall := '(' [Identifier [',' Identifier]*] ')'
 */
LandS
Parser2::parseFunctionCall()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren

    std::string types = "(";
    auto topinput = getChain(true, TokenKind::RPAREN, OP::INPUT,
                             [&types, this]() -> Lexeme {
                                 auto a = parseE0();
                                 auto input = std::move(std::get<0>(a));
                                 auto type = std::get<1>(a);

                                 if (symboltable->hasVariable(type))
                                     type = symboltable->getSymbolType(type);

                                 types += "_" + type;
                                 return input;
                             });

    types += ")";

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
    return std::make_tuple(makeNode(OP::FUNCTION_CALL, std::move(topinput)),
                           types);
}

/**
 * New := UDTDec
 */
LandS
Parser2::parseNew()
{
    advanceAndCheckToken(TokenKind::NEW); // consume new
    switch (currentToken->kind)
    {
    case TokenKind::IDENTIFIER:
    {
        auto a = parseUDTDec();
        auto udtdec = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(std::move(udtdec), type);
    }
    default:
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Expected a valid new declaration such as a UDT defined "
                   "by '{' '}'.",
                   "Received: ", currentToken->value,
                   " of type " + displayKind(currentToken->kind) + ".")));
        return std::make_tuple(makeNullNode(), ""); //  unreachable
    }
}

/**
 * UDTDec := Identifier '{' [UDTDecItem [',' UDTDecItem]*] '}'
 * UDTDecItem := Identifier ':' Primary
 */
LandS
Parser2::parseUDTDec()
{

    auto udtType = parseIdentifier();
    auto udtName = udtType->value;
    output << "(struct " + udtName + "*)malloc(sizeof(struct " + udtName +
                  "));\n";
    udtBuffer +=
        "(struct " + udtName + "*)malloc(sizeof(struct " + udtName + "));\n";

    checkExists(udtName);
    checkUDTExists(udtName);

    auto st = udttable->getAttributeSymbolTable(udtName);
    auto attributes = st->getSymbols();

    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    auto topitem = getChain(
        true, TokenKind::RCURLEY, OP::UDTDECITEM,
        [st, udtName, &attributes, this]() -> NodePtr {
            // capture key
            auto attributeName = parseIdentifier();

            this->udtBuffer +=
                getTabs() + decName + "->" + attributeName->value + " = ";
            this->output << getTabs() + decName + "->" + attributeName->value +
                                " = ";

            advanceAndCheckToken(TokenKind::COLON); // consume ':'

            // capture value
            auto primary = parsePrimary();
            auto prim = std::move(std::get<0>(primary));
            auto type = std::get<1>(primary);

            if (attributes.size() != 1)
            {
                this->udtBuffer += ";\n";
                this->output << ";\n";
            }

            // determine if key exists for udt
            std::vector<std::string>::iterator it = std::find(
                attributes.begin(), attributes.end(), attributeName->value);

            if (it != attributes.end())
            {
                int index = std::distance(attributes.begin(), it);
                checkType(st->getSymbolType(attributeName->value), type);
                attributes.erase(attributes.begin() + index);
            }
            else
                semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
                    currentToken->col, currentToken->line,
                    "Unrecognized initialization key for new udt of type: " +
                        udtName,
                    "Unrecognized key: ", attributeName->value, ".")));

            return makeNode(OP::UDTDECITEM, std::move(attributeName),
                            std::move(prim));
        });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley

    if (attributes.size() != 0)
        semanticerrorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line,
            "Missing keys in udt initialization for type: " + udtName,
            "Expected " + std::to_string(st->getSymbols().size()) +
                " keys and received ",
            std::to_string(st->getSymbols().size() - attributes.size()), ".")));

    return std::make_tuple(
        makeNode(OP::UDTDEC, std::move(udtType), std::move(topitem)), udtName);
}

/**
 * T := Primary | '(' E0 ')'
 */
LandS
Parser2::parseT()
{
    if (currentToken->kind == TokenKind::LPAREN)
    {
        advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
        output << "(";
        udtBuffer += "(";
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
        output << ")";
        udtBuffer += ")";
        return std::make_tuple(std::move(e0), type);
    }

    else if (currentToken->kind == TokenKind::NEGATION ||
             currentToken->kind == TokenKind::UNARYADD ||
             currentToken->kind == TokenKind::UNARYMINUS ||
             currentToken->kind == TokenKind::NEW)
    {
        auto a = parseE1(""); // type less
        auto e1 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(std::move(e1), type);
    }

    auto primary = parsePrimary();
    auto prim = std::move(std::get<0>(primary));
    auto type = std::get<1>(primary);

    return std::make_tuple(makeNode(OP::PRIMARY, std::move(prim)), type);
}

/**
 * Primary := Bool | Integer |  String | Identifier
 */
LandS
Parser2::parsePrimary()
{
    switch (currentToken->kind)
    {
    case TokenKind::BOOL:
        return std::make_tuple(parseBoolean(), "bool");
    case TokenKind::INTEGER:
        return std::make_tuple(parseNumber(), "int");
    case TokenKind::FLOAT:
        return std::make_tuple(parseNumber(), "flt");
    case TokenKind::STRING:
        return std::make_tuple(parseString(), "str");
    case TokenKind::OWN_ACCESSOR:
        return std::make_tuple(parseOwnAccessor(), "own");
    case TokenKind::EMPTY:
        return std::make_tuple(parseEmpty(), "empty");
    case TokenKind::IDENTIFIER:
    {
        if (currentToken->value.at(0) == '[')
            return parseListType();

        auto id = parseIdentifier();
        auto type = id->value;
        output << builtinTypesTranslator(id->value);
        udtBuffer += builtinTypesTranslator(id->value);
        return std::make_tuple(std::move(id), type);
    }
    case TokenKind::LIST:
    {
        auto a = parseList();
        auto list = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(std::move(list), type);
    }
    default:
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Expected a valid primary token such as a boolean, "
                   "integer, float, string, identifier, or list.",
                   "Received: ", currentToken->value,
                   " of type " + displayKind(currentToken->kind) + ".")));
        return std::make_tuple(parseIdentifier(), "bool"); //  unreachable
    }
}

/**
 * Type := Identifier
 */
LeafPtr
Parser2::parseType()
{
    if (currentToken->kind == TokenKind::LISTTYPE)
    {
        auto a = parseListType();
        auto list = std::move(std::get<0>(a));
        return std::move(list);
    }
    return parseIdentifier();
}

/**
 * Variable := Type Identifier
 */
LSandS
Parser2::parseVariable()
{
    auto type = parseType();

    // deal with void types
    if (type.get()->value == "void")
        return std::make_tuple(
            makeNode(OP::VARIABLE, std::move(type), std::move(type)),
            type.get()->value, type.get()->value);

    auto name = parseIdentifier();
    return std::make_tuple(
        makeNode(OP::VARIABLE, std::move(type), std::move(name)),
        name.get()->value, type.get()->value);
}

/**
 * Number := Integer | Float
 */
LeafPtr
Parser2::parseNumber()
{
    auto v = currentToken->value;
    auto k = currentToken->kind;

    if (k == TokenKind::INTEGER)
    {
        advanceAndCheckToken(TokenKind::INTEGER); // eat integer
        output << v;
        udtBuffer += v;
        return makeLeaf(LIT::INTEGER, v);
    }

    advanceAndCheckToken(TokenKind::FLOAT); // eat float
    output << v;
    udtBuffer += v;
    return makeLeaf(LIT::FLOAT, v);
}

/**
 * Identifier := lexvalue
 */
LeafPtr
Parser2::parseIdentifier()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::IDENTIFIER); // eat identifier
    return makeLeaf(LIT::IDENTIFIER, v);
}

/**
 * Bool := lexvalue
 */
LeafPtr
Parser2::parseBoolean()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::BOOL); // eat identifier
    output << (v == "true" ? "1" : "0");
    udtBuffer += (v == "true" ? "1" : "0");

    return makeLeaf(LIT::IDENTIFIER, v);
}

/**
 * String := lexvalue
 */
LeafPtr
Parser2::parseString()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // true eat string
    output << v;
    udtBuffer += v;
    return makeLeaf(LIT::STRING, v);
}

/**
 * Own Accessor:= lexvalue
 */
LeafPtr
Parser2::parseOwnAccessor()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::OWN_ACCESSOR); // eat own accessor

    udtBuffer += "this";

    if (isUdt)
        return makeLeaf(LIT::IDENTIFIER, extractUDTName(filename));
    else
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "illegal usage of own in a non udt method.", "", "", "")));
    return makeLeaf(LIT::OWN, v); // will not ever reach here
}

/**
 * Empty:= lexvalue
 */
LeafPtr
Parser2::parseEmpty()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::EMPTY); // eat own accessor

    output << "NULL";
    udtBuffer += "NULL";

    return makeLeaf(LIT::EMPTY, v); // will not ever reach here
}

/**
 * ListType := lexvalue
 */
std::tuple<LeafPtr, std::string>
Parser2::parseListType()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LISTTYPE); // eat list type
    // output << v;
    // udtBuffer += v;
    return std::make_tuple(makeLeaf(LIT::LISTTYPE, v), "[list]");
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

LIT
Parser2::kindToLIT(TokenKind tk)
{
    switch (tk)
    {
    case TokenKind::IDENTIFIER:
        return LIT::IDENTIFIER;
    case TokenKind::INTEGER:
        return LIT::INTEGER;
    case TokenKind::FLOAT:
        return LIT::FLOAT;
    case TokenKind::STRING:
        return LIT::STRING;
    case TokenKind::BOOL:
        return LIT::BOOLEAN;
    default:
        errorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Unexpected type in a list.",
            "Type: ", displayKind(tk), "")));
    }
}

std::string
Parser2::tokenToType(const TokenKind& tk, const std::string& val)
{
    switch (tk)
    {
    case TokenKind::IDENTIFIER:
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
    }
}

/**
 * List := lexvalue
 */
LandS
Parser2::parseList()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LIST); // eat list
    auto listVals = determineTypes(parseListValues(v));

    std::string type = "none";

    auto prev = makeNullNode();
    std::deque<std::string> vals;
    if (listVals.size() == 0)
    {
    }
    else
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

            auto node =
                makeNode(OP::LISTITEM, makeLeaf(kindToLIT(v->kind), v->value),
                         std::move(prev));
            prev = std::move(node);
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

    udtBuffer += buf;
    output << buf;

    return std::make_tuple(makeNode(OP::LIST, std::move(prev)), type);
}

void
Parser2::transpile()
{
    output.close();
}