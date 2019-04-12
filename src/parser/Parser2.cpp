#include "Parser2.h"

UdtAndFlag
parseFile(const std::string& filename)
{
    try
    {
        Parser2* p = new Parser2(filename);
        auto n = p->parse();
        return std::make_tuple(std::move(p->getUDTTable()), p->getIsUDTFlag());
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
    if (t0 == "num")
    {
        if ("int" != t1 && "flt" != t1)
        {
            semanticerrorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Mismatched types. Expected/LeftHand is: int or flt.",
                       "Received/Right Hand is: ", t1, ".")));
        }
    }
    else if (t0 != t1)
    {
        semanticerrorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Mismatched types. Expected/LeftHand is: " + t0 + ".",
                   "Received/Right Hand is: ", t1, ".")));
    }
}

// constructor
Parser2::Parser2(const std::string& file)
{
    filename = file;
    lexar = std::make_unique<Lexar2>(file);
    currentToken = lexar->getNextToken();
    errorhandler = std::make_unique<Parser2ErrorHandler>(Parser2ErrorHandler());
    semanticerrorhandler = std::make_unique<SemanticAnalyzerErrorHandler>(
        SemanticAnalyzerErrorHandler());
    symboltable = std::make_unique<SymbolTable>(SymbolTable());
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
        auto udtAndFlag = parseFile(file);

        auto table = std::move(std::get<0>(udtAndFlag));
        auto flag = std::get<1>(udtAndFlag);

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
    return parseString();
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
        return parseScript();
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
    auto atlandst = this->parseAttributes();
    auto topattribute = std::move(std::get<0>(atlandst));
    std::shared_ptr<SymbolTable> attributest = std::move(std::get<1>(atlandst));

    auto mlandst = this->parseMethods();
    auto topmethod = std::move(std::get<0>(mlandst));
    std::shared_ptr<SymbolTable> methodst = std::move(std::get<1>(mlandst));

    auto udtname = extractUDTName(filename);

    udttable->addUDT(udtname, attributest, methodst);

    return makeNode(OP::UDT, std::move(topattribute), std::move(topmethod));
}

/**
 * Attributes := 'Uat' [Identifier Identifier]*
 */
LandSt
Parser2::parseAttributes()
{
    advanceAndCheckToken(TokenKind::UAT);     // consume uat
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley

    std::unique_ptr<SymbolTable> st =
        std::make_unique<SymbolTable>(SymbolTable());

    auto topattribute = getChain(
        true, TokenKind::RCURLEY, OP::ATTRIBUTE, [&st, this]() -> Lexeme {
            auto lands = this->parseVariable();

            auto attribute = std::move(std::get<0>(lands));
            auto name = std::get<1>(lands);
            auto type = std::get<2>(lands);

            st->addSymbol(name, type);

            return attribute;
        });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return std::make_tuple(makeNode(OP::ATTRIBUTE, std::move(topattribute)),
                           std::move(st));
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
    std::unique_ptr<SymbolTable> st;
    std::unique_ptr<SymbolTable> temp = std::move(symboltable);
    symboltable = std::make_unique<SymbolTable>(SymbolTable());

    auto topmethod =
        getChain(true, TokenKind::RCURLEY, OP::METHOD, [this]() -> NodePtr {
            return this->parseFunctionDefinition();
        });

    // set back the normal symbol table and capture temp table
    st = std::move(symboltable);
    symboltable = std::move(temp);

    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return std::make_tuple(makeNode(OP::METHOD, std::move(topmethod)),
                           std::move(st));
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
 */
NodePtr
Parser2::parseFunctionDefinition()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    advanceAndCheckToken(TokenKind::FUN);    // consume funenterScope

    // parse left child
    auto id = parseIdentifier();

    // parse right child
    auto lands = parseFunctionInfo();
    auto lexeme = std::move(std::get<0>(lands));
    auto type = std::get<1>(lands);
    type = "F" + type;

    // add to symbol table
    auto ok = symboltable->addSymbol(id->value, type);
    if (!ok)
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Unexpected redeclaration of " + id->value +
                       ", originally defined as type " +
                       symboltable->getSymbolType(id->value) + ".",
                   "Received second declaration of type: ", type, ".")));

    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION, std::move(id), std::move(lexeme));
}

/**
 * FunctionInfo := FunctionInOut Block
 */
LandS
Parser2::parseFunctionInfo()
{
    auto lands = parseFunctionInOut();
    auto fintout = std::move(std::get<0>(lands));
    auto type = std::get<1>(lands);

    auto block = parseBlock();

    return std::make_tuple(
        makeNode(OP::FUNCTION_INFO, std::move(fintout), std::move(block)),
        type);
}

/**
 * FunctionInOut := FunctionInputs* FunctionOutputs
 *  FunctionInputs :=  '(' Variable [',' Variable]* ')'
 */
LandS
Parser2::parseFunctionInOut()
{
    // inputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
                                             // TODO: check for multiple voids

    std::string types = "(";
    auto topinput = getChain(true, TokenKind::RPAREN, OP::FUNCTION_INPUT,
                             [&types, this]() -> Lexeme {
                                 auto lands = this->parseVariable();
                                 auto inp = std::move(std::get<0>(lands));
                                 auto type = std::get<2>(lands);
                                 types += "_" + type;
                                 return inp;
                             });
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    // outputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    auto output = parseType();
    types += ")" + output.get()->value;
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
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
    auto block = parseBlock();
    return makeNode(OP::START, makeLeaf(LIT::IDENTIFIER, "start"),
                    std::move(block));
}

/**
 * Block = '{' Statement* '}'
 */
NodePtr
Parser2::parseBlock()
{
    advanceAndCheckToken(TokenKind::LCURLEY); // eat '{'

    symboltable->enterScope();
    auto topstatement =
        getChain(true, TokenKind::RCURLEY, OP::STATEMENT,
                 [this]() -> Lexeme { return this->parseStatement(); });
    symboltable->exitScope();

    advanceAndCheckToken(TokenKind::RCURLEY); // eat '}'
    return makeNode(OP::BLOCK, std::move(topstatement));
}

/**
 * Statement := Tree | Return | Declaration | E0
 */
Lexeme
Parser2::parseStatement()
{
    switch (currentToken->kind)
    {
    case TokenKind::TREE:
        return parseTree();
    case TokenKind::RETURN:
        return parseReturn();
    case TokenKind::DEC:
        return parseDeclaration();
    default:
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return e0;
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
    std::vector<NodePtr> branches;

    auto topbranch =
        getChain(true, TokenKind::RPAREN, OP::BRANCH,
                 [this]() -> NodePtr { return this->parseBranch(); });
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
    auto block = parseBlock();
    advanceAndCheckToken(TokenKind::RPAREN); // eat ')'
    return makeNode(OP::BRANCH, std::move(grouping), std::move(block));
}

/**
 * Grouping := '|' E0 '|'
 */
Lexeme
Parser2::parseGrouping()
{
    advanceAndCheckToken(TokenKind::PIPE); // eat '|'

    auto a = parseE0();
    auto e0 = std::move(std::get<0>(a));
    auto type = std::get<1>(a);

    // checkType("bool", type);

    advanceAndCheckToken(TokenKind::PIPE); // eat '|'
    return e0;
}

/**
 * Return := 'return' T
 */
NodePtr
Parser2::parseReturn()
{
    advanceAndCheckToken(TokenKind::RETURN); // consume 'return'
    auto a = parseT();
    auto t = std::move(std::get<0>(a));
    auto type = std::get<1>(a);
    return makeNode(OP::RETURN, std::move(t));
}

/**
 * Declaration :=  'dec' Variable '=' E0
 */
NodePtr
Parser2::parseDeclaration()
{
    advanceAndCheckToken(TokenKind::DEC); // consume 'dec'

    auto lsands = parseVariable();
    auto var = std::move(std::get<0>(lsands));
    auto name = std::get<1>(lsands);
    auto type = std::get<2>(lsands);

    symboltable->addSymbol(name, type);

    advanceAndCheckToken(TokenKind::ASSIGNMENT); // consume '='
    auto a = parseE0();
    auto e0 = std::move(std::get<0>(a));
    auto ta = std::get<1>(a);

    checkType(type, ta);

    return makeNode(OP::DECLARATION, std::move(var), std::move(e0));
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
 */
LandS
Parser2::parseE1(const std::string& T0)
{

    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE2(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are int
            this->checkType(T0, T1);
            this->checkType("int", T0);
            this->checkType("int", T1);
            return T1;
        },
        std::make_tuple(TokenKind::EXPONENTIATION, OP::EXPONENT));
}

/**
 * E2 := * E0 | / E0 | % E0 | E3
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
        std::make_tuple(TokenKind::MULTIPLICATION, OP::MULTIPLICATION),
        std::make_tuple(TokenKind::DIVISION, OP::DIVISION),
        std::make_tuple(TokenKind::MODULO, OP::MODULO));
}

/**
 * E3 := + E0 | - E0 | E4
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
        std::make_tuple(TokenKind::ADDITION, OP::ADDITION),
        std::make_tuple(TokenKind::SUBTRACTION, OP::SUBTRACTION));
}

/**
 * E4 := < E0 | > E0 | <= E0 | >= E0 | E5
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
        std::make_tuple(TokenKind::LESS_THAN, OP::LESS_THAN),
        std::make_tuple(TokenKind::LESS_THAN_OR_EQUALS,
                        OP::LESS_THAN_OR_EQUALS),
        std::make_tuple(TokenKind::GREATER_THAN, OP::GREATER_THAN),
        std::make_tuple(TokenKind::GREATER_THAN_OR_EQUALS,
                        OP::GREATER_THAN_OR_EQUALS));
}

/**
 * E5 := == E0 | != E0 | E6
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
        std::make_tuple(TokenKind::EQUIVALENCE, OP::EQUIVALENCE),
        std::make_tuple(TokenKind::NONEQUIVALENCE, OP::NONEQUIVALENCE));
}

/**
 * E6 := 'and' E0 | 'or' E0 | E7
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
        std::make_tuple(TokenKind::OR, OP::OR),
        std::make_tuple(TokenKind::AND, OP::AND));
}

/**
 * E7 := = E0 | E8
 */
LandS
Parser2::parseE7(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE8(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are the same
            this->checkType(T0, T1);
            return T1;
        },

        std::make_tuple(TokenKind::ASSIGNMENT, OP::ASSIGNMENT));
}

/**
 * E8 := [!, ++, --] E0 | E9
 */
LandS
Parser2::parseE8(const std::string& T0)
{
    return parseExpr(
        T0,
        [this](const std::string& T0) -> LandS { return this->parseE9(T0); },
        [this](const std::string& T0, const std::string& T1) -> std::string {
            // checking that both are the same
            // this->checkType(T0, T1);
            // TODO: check this
            return T1;
        },
        std::make_tuple(TokenKind::NEGATION, OP::NEGATION),
        std::make_tuple(TokenKind::UNARYADD, OP::UNARYADD),
        std::make_tuple(TokenKind::UNARYMINUS, OP::UNARYMINUS));
}

/**
 * E9 := [+=, -=, /=, *=] E0 | E10
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
        std::make_tuple(TokenKind::ADDTO, OP::ADDTO),
        std::make_tuple(TokenKind::SUBFROM, OP::SUBFROM),
        std::make_tuple(TokenKind::DIVFROM, OP::DIVFROM),
        std::make_tuple(TokenKind::MULTTO, OP::MULTTO));
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
        auto memberAccess = parseMemberAccess();
        auto a = parseE1(""); // like calling E0 where T0 does not exist
        auto e1 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        return std::make_tuple(
            makeNode(OP::MEMBER, std::move(memberAccess), std::move(e1)), type);
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
 * E12 := T12 | ε
 */
LandS
Parser2::parseE12(const std::string& T11)
{
    auto landv = T11;
    // auto t11 = std::move(std::get<0>(landv));
    // auto type = std::get<1>(landv);

    return std::make_tuple(makeNullNode(), T11);
}

/**
 * MemberAccess := AttributeAccess | MethodAccess
 */
NodePtr
Parser2::parseMemberAccess()
{
    switch (currentToken->kind)
    {
    case TokenKind::DOT:
        return parseAttributeAccess();
    case TokenKind::TRIPLE_DOT:
        return parseMethodAccess();
    default:
        errorhandler->handle(std::make_unique<Error2>(Error2(
            currentToken->col, currentToken->line, "Expected a ... or . token.",
            "Received: ", currentToken->value,
            " of type " + displayKind(currentToken->kind) + ".")));
        return makeNullNode(); //  unreachable
    }
}

/**
 * AttributeAccess := '.' Identifier
 */
NodePtr
Parser2::parseAttributeAccess()
{
    advanceAndCheckToken(TokenKind::DOT); // consume '.'
    auto attribute = parseIdentifier();
    return makeNode(OP::ATTRIBUTE_ACCESS, std::move(attribute));
}

/**
 * MethodAccess := '...' Identifier FunctionCall
 */
NodePtr
Parser2::parseMethodAccess()
{
    advanceAndCheckToken(TokenKind::TRIPLE_DOT); // consume '...'
    auto name = parseIdentifier();
    auto method = parseFunctionCall();
    return makeNode(OP::METHOD_ACCESS, std::move(method), std::move(name));
}

/**
 * FunctionCall := '(' [Identifier [',' Identifier]*] ')'
 */
NodePtr
Parser2::parseFunctionCall()
{
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    auto topinput =
        getChain(true, TokenKind::RPAREN, OP::INPUT,
                 [this]() -> LeafPtr { return this->parseIdentifier(); });
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION_CALL, std::move(topinput));
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
 */
LandS
Parser2::parseUDTDec()
{
    auto name = parseIdentifier();
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    auto topitem =
        getChain(true, TokenKind::RCURLEY, OP::UDTDECITEM,
                 [this]() -> NodePtr { return this->parseUDTDecItem(); });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return std::make_tuple(
        makeNode(OP::UDTDEC, std::move(name), std::move(topitem)), name->value);
}

/**
 * UDTDecItem := Identifier ':' Primary
 */
NodePtr
Parser2::parseUDTDecItem()
{
    auto identifier = parseIdentifier();
    advanceAndCheckToken(TokenKind::COLON); // consume ':'
    auto primary = parsePrimary();
    auto prim = std::move(std::get<0>(primary));
    auto type = std::get<1>(primary);
    return makeNode(OP::UDTDECITEM, std::move(identifier), std::move(prim));
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
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);
        advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
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
    case TokenKind::IDENTIFIER:
        return std::make_tuple(parseIdentifier(), "id");
    case TokenKind::LIST:
        return std::make_tuple(parseList(), currentToken->value);
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
        return parseListType();
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
        return makeLeaf(LIT::INTEGER, v);
    }

    advanceAndCheckToken(TokenKind::FLOAT); // eat float
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
    return makeLeaf(LIT::IDENTIFIER, v);
}

/**
 * String := lexvalue
 */
LeafPtr
Parser2::parseString()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::STRING); // eat string
    return makeLeaf(LIT::STRING, v);
}

/**
 * List := lexvalue
 */
LeafPtr
Parser2::parseList()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LIST); // eat list
    return makeLeaf(LIT::LIST, v);
}

/**
 * ListType := lexvalue
 */
LeafPtr
Parser2::parseListType()
{
    auto v = currentToken->value;
    advanceAndCheckToken(TokenKind::LISTTYPE); // eat list type
    return makeLeaf(LIT::LISTTYPE, v);
}
