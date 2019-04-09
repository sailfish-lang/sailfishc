#include "Parser2.h"

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

// constructor
Parser2::Parser2(const std::string& filename)
{
    lexar = std::make_unique<Lexar2>(filename);
    currentToken = lexar->getNextToken();
    errorhandler = std::make_unique<Parser2ErrorHandler>(Parser2ErrorHandler());
}

// public interface method
NodePtr
Parser2::parse()
{
    return parseProgram();
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
    auto attributes = parseAttributes();
    auto methods = parseMethods();
    return makeNode(OP::UDT, std::move(attributes), std::move(methods));
}

/**
 * Attributes := 'Uat' [Identifier Identifier]*
 */
NodePtr
Parser2::parseAttributes()
{
    advanceAndCheckToken(TokenKind::UAT);     // consume uat
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    auto topattribute =
        getChain(true, TokenKind::RCURLEY, OP::ATTRIBUTE,
                 [this]() -> NodePtr { return this->parseVariable(); });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return makeNode(OP::ATTRIBUTE, std::move(topattribute));
}

/**
 * Methods := 'Ufn' [FunctionDefinition]*
 */
NodePtr
Parser2::parseMethods()
{
    advanceAndCheckToken(TokenKind::UFN);     // consume ufn
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    auto topmethod =
        getChain(true, TokenKind::RCURLEY, OP::METHOD, [this]() -> NodePtr {
            return this->parseFunctionDefinition();
        });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return makeNode(OP::METHOD, std::move(topmethod));
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
    advanceAndCheckToken(TokenKind::FUN);    // consume fun
    auto id = parseIdentifier();
    auto info = parseFunctionInfo();
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION, std::move(id), std::move(info));
}

/**
 * FunctionInfo := FunctionInOut Block
 */
NodePtr
Parser2::parseFunctionInfo()
{
    auto inout = parseFunctionInOut();
    auto block = parseBlock();
    return makeNode(OP::FUNCTION_INFO, std::move(inout), std::move(block));
}

/**
 * FunctionInOut := FunctionInputs* FunctionOutputs
 *  FunctionInputs :=  '(' Variable [',' Variable]* ')'
 */
NodePtr
Parser2::parseFunctionInOut()
{
    // inputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
                                             // TODO: check for multiple voids
    auto topinput =
        getChain(true, TokenKind::RPAREN, OP::FUNCTION_INPUT,
                 [this]() -> NodePtr { return this->parseVariable(); });
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren

    // outputs
    advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
    auto output = parseType();
    advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION_IN_OUT, std::move(topinput),
                    std::move(output));
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
    auto topstatement =
        getChain(true, TokenKind::RCURLEY, OP::STATEMENT,
                 [this]() -> Lexeme { return this->parseStatement(); });
    NodePtr parseBlockRecurse();

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
        return parseE0();
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
    auto condition = parseE0();
    advanceAndCheckToken(TokenKind::PIPE); // eat '|'
    return condition;
}

/**
 * Return := 'return' T
 */
NodePtr
Parser2::parseReturn()
{
    advanceAndCheckToken(TokenKind::RETURN); // consume 'return'
    auto t = parseT();
    return makeNode(OP::RETURN, std::move(t));
}

/**
 * Declaration :=  'dec' Variable '=' E0
 */
NodePtr
Parser2::parseDeclaration()
{
    advanceAndCheckToken(TokenKind::DEC); // consume 'dec'
    auto var = parseVariable();
    advanceAndCheckToken(TokenKind::ASSIGNMENT); // consume '='
    auto e0 = parseE0();
    return makeNode(OP::DECLARATION, std::move(var), std::move(e0));
}

/**
 * E0 := T0 E1
 */
Lexeme
Parser2::parseE0()
{
    auto t0 = parseT();
    return parseE1(std::move(t0));
}

/**
 * E1 := ** E0 | E2
 */
Lexeme
Parser2::parseE1(Lexeme T0)
{
    if (currentToken->kind == TokenKind::EXPONENTIATION)
    {
        advanceAndCheckToken(TokenKind::EXPONENTIATION); // consume "**"
        auto e0 = parseE0();
        return makeNode(OP::EXPONENT, std::move(T0), std::move(e0));
    }
    return parseE2(std::move(T0));
}

/**
 * E2 := * E0 | / E0 | % E0 | E3
 */
Lexeme
Parser2::parseE2(Lexeme T1)
{
    if (currentToken->kind == TokenKind::MULTIPLICATION ||
        currentToken->kind == TokenKind::DIVISION ||
        currentToken->kind == TokenKind::MODULO)
    {
        switch (currentToken->kind)
        {
        case TokenKind::MULTIPLICATION:
        {
            advanceAndCheckToken(TokenKind::MULTIPLICATION); // consume "*"
            auto e0 = parseE0();
            return makeNode(OP::MULTIPLICATION, std::move(T1), std::move(e0));
        }
        case TokenKind::DIVISION:
        {
            advanceAndCheckToken(TokenKind::DIVISION); // consume "/"
            auto e0 = parseE0();
            return makeNode(OP::DIVISION, std::move(T1), std::move(e0));
        }
        case TokenKind::MODULO:
        {
            advanceAndCheckToken(TokenKind::MODULO); // consume "%"
            auto e0 = parseE0();
            return makeNode(OP::MODULO, std::move(T1), std::move(e0));
        }
        }
    }
    return parseE3(std::move(T1));
}

/**
 * E3 := + E0 | - E0 | E4
 */
Lexeme
Parser2::parseE3(Lexeme T2)
{
    if (currentToken->kind == TokenKind::ADDITION ||
        currentToken->kind == TokenKind::SUBTRACTION)
    {

        switch (currentToken->kind)
        {
        case TokenKind::ADDITION:
        {
            advanceAndCheckToken(TokenKind::ADDITION); // consume "+"
            auto e0 = parseE0();
            return makeNode(OP::ADDITION, std::move(T2), std::move(e0));
        }
        case TokenKind::SUBTRACTION:
        {
            advanceAndCheckToken(TokenKind::SUBTRACTION); // consume "-"
            auto e0 = parseE0();
            return makeNode(OP::SUBTRACTION, std::move(T2), std::move(e0));
        }
        }
    }
    return parseE4(std::move(T2));
}

/**
 * E4 := < E0 | > E0 | <= E0 | >= E0 | E5
 */
Lexeme
Parser2::parseE4(Lexeme T3)
{
    if (currentToken->kind == TokenKind::LESS_THAN ||
        currentToken->kind == TokenKind::LESS_THAN_OR_EQUALS ||
        currentToken->kind == TokenKind::GREATER_THAN ||
        currentToken->kind == TokenKind::GREATER_THAN_OR_EQUALS)
    {
        switch (currentToken->kind)
        {
        case TokenKind::LESS_THAN:
        {
            advanceAndCheckToken(TokenKind::LESS_THAN); // consume "<"
            auto e0 = parseE0();
            return makeNode(OP::LESS_THAN, std::move(T3), std::move(e0));
        }
        case TokenKind::LESS_THAN_OR_EQUALS:
        {
            advanceAndCheckToken(TokenKind::LESS_THAN_OR_EQUALS); // consume "<"
            auto e0 = parseE0();
            return makeNode(OP::LESS_THAN_OR_EQUALS, std::move(T3),
                            std::move(e0));
        }
        case TokenKind::GREATER_THAN:
        {
            advanceAndCheckToken(TokenKind::GREATER_THAN); // consume ">"
            auto e0 = parseE0();
            return makeNode(OP::GREATER_THAN, std::move(T3), std::move(e0));
        }
        case TokenKind::GREATER_THAN_OR_EQUALS:
        {
            advanceAndCheckToken(
                TokenKind::GREATER_THAN_OR_EQUALS); // consume ">="
            auto e0 = parseE0();
            return makeNode(OP::GREATER_THAN_OR_EQUALS, std::move(T3),
                            std::move(e0));
        }
        }
    }
    return parseE5(std::move(T3));
}

/**
 * E5 := == E0 | != E0 | E6
 */
Lexeme
Parser2::parseE5(Lexeme T4)
{
    if (currentToken->kind == TokenKind::EQUIVALENCE ||
        currentToken->kind == TokenKind::NONEQUIVALENCE)
    {

        switch (currentToken->kind)
        {
        case TokenKind::EQUIVALENCE:
        {
            advanceAndCheckToken(TokenKind::EQUIVALENCE); // consume "=="
            auto e0 = parseE0();
            return makeNode(OP::EQUIVALENCE, std::move(T4), std::move(e0));
        }
        case TokenKind::NONEQUIVALENCE:
        {
            advanceAndCheckToken(TokenKind::NONEQUIVALENCE); // consume "!="
            auto e0 = parseE0();
            return makeNode(OP::NONEQUIVALENCE, std::move(T4), std::move(e0));
        }
        }
    }
    return parseE6(std::move(T4));
}

/**
 * E6 := 'and' E0 | 'or' E0 | E7
 */
Lexeme
Parser2::parseE6(Lexeme T5)
{
    if (currentToken->kind == TokenKind::AND ||
        currentToken->kind == TokenKind::OR)
    {
        switch (currentToken->kind)
        {
        case TokenKind::AND:
        {
            advanceAndCheckToken(TokenKind::AND); // consume 'and'
            auto e0 = parseE0();
            return makeNode(OP::AND, std::move(T5), std::move(e0));
        }
        case TokenKind::OR:
        {
            advanceAndCheckToken(TokenKind::OR); // consume 'or'
            auto e0 = parseE0();
            return makeNode(OP::OR, std::move(T5), std::move(e0));
        }
        }
    }
    return parseE7(std::move(T5));
}

/**
 * E7 := = E0 | E8
 */
Lexeme
Parser2::parseE7(Lexeme T6)
{
    if (currentToken->kind == TokenKind::ASSIGNMENT)
    {
        advanceAndCheckToken(TokenKind::ASSIGNMENT); // consume "="
        auto e0 = parseE0();
        return makeNode(OP::ASSIGNMENT, std::move(T6), std::move(e0));
    }
    return parseE8(std::move(T6));
}

/**
 * E8 := [!, ++, --] E0 | E9
 */
Lexeme
Parser2::parseE8(Lexeme T7)
{
    if (currentToken->kind == TokenKind::NEGATION ||
        currentToken->kind == TokenKind::UNARYADD ||
        currentToken->kind == TokenKind::UNARYMINUS)
    {
        switch (currentToken->kind)
        {
        case TokenKind::NEGATION:
        {
            advanceAndCheckToken(TokenKind::NEGATION); // consume "!"
            auto e0 = parseE0();
            return makeNode(OP::NEGATION, std::move(T7), std::move(e0));
        }
        case TokenKind::UNARYADD:
        {
            advanceAndCheckToken(TokenKind::UNARYADD); // consume "++"
            auto e0 = parseE0();
            return makeNode(OP::UNARYADD, std::move(T7), std::move(e0));
        }
        case TokenKind::UNARYMINUS:
        {
            advanceAndCheckToken(TokenKind::UNARYMINUS); // consume "--"
            auto e0 = parseE0();
            return makeNode(OP::UNARYMINUS, std::move(T7), std::move(e0));
        }
        }
    }
    return parseE9(std::move(T7));
}

/**
 * E9 := [+=, -=, /=, *=] E0 | E10
 */
Lexeme
Parser2::parseE9(Lexeme T8)
{
    if (currentToken->kind == TokenKind::ADDTO ||
        currentToken->kind == TokenKind::SUBFROM ||
        currentToken->kind == TokenKind::DIVFROM ||
        currentToken->kind == TokenKind::MULTTO)
    {
        switch (currentToken->kind)
        {
        case TokenKind::ADDTO:
        {
            advanceAndCheckToken(TokenKind::ADDTO); // consume "+="
            auto e0 = parseE0();
            return makeNode(OP::ADDTO, std::move(T8), std::move(e0));
        }
        case TokenKind::SUBFROM:
        {
            advanceAndCheckToken(TokenKind::SUBFROM); // consume "-="
            auto e0 = parseE0();
            return makeNode(OP::SUBFROM, std::move(T8), std::move(e0));
        }
        case TokenKind::DIVFROM:
        {
            advanceAndCheckToken(TokenKind::DIVFROM); // consume "/="
            auto e0 = parseE0();
            return makeNode(OP::DIVFROM, std::move(T8), std::move(e0));
        }
        case TokenKind::MULTTO:
        {
            advanceAndCheckToken(TokenKind::MULTTO); // consume "*="
            auto e0 = parseE0();
            return makeNode(OP::MULTTO, std::move(T8), std::move(e0));
        }
        }
    }
    return parseE10(std::move(T8));
}

/**
 * E10 := MemberAccess E0 | E11
 */
Lexeme
Parser2::parseE10(Lexeme T9)
{
    if (currentToken->kind == TokenKind::DOT ||
        currentToken->kind == TokenKind::TRIPLE_DOT)
    {
        auto memberAccess = parseMemberAccess();
        auto e1 =
            parseE1(makeNullNode()); // like calling E0 where T0 does not exist
        return makeNode(OP::MEMBER, std::move(memberAccess), std::move(e1));
    }
    return parseE11(std::move(T9));
}

/**
 * E11 := New | E12
 */
Lexeme
Parser2::parseE11(Lexeme T10)
{
    if (currentToken->kind == TokenKind::NEW)
    {
        auto New = parseNew();
        return makeNode(OP::MEMBER, std::move(New));
    }
    return parseE12(std::move(T10));
}

/**
 * E12 := T12 | ε
 */
Lexeme
Parser2::parseE12(Lexeme T11)
{
    return T11;
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
NodePtr
Parser2::parseNew()
{
    advanceAndCheckToken(TokenKind::NEW); // consume new
    switch (currentToken->kind)
    {
    case TokenKind::IDENTIFIER:
        return parseUDTDec();
    default:
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Expected a valid new declaration such as a UDT defined "
                   "by '{' '}'.",
                   "Received: ", currentToken->value,
                   " of type " + displayKind(currentToken->kind) + ".")));
        return makeNullNode(); //  unreachable
    }
}

/**
 * UDTDec := Identifier '{' [UDTDecItem [',' UDTDecItem]*] '}'
 */
NodePtr
Parser2::parseUDTDec()
{
    auto name = parseIdentifier();
    advanceAndCheckToken(TokenKind::LCURLEY); // consume l curley
    auto topitem =
        getChain(true, TokenKind::RCURLEY, OP::UDTDECITEM,
                 [this]() -> NodePtr { return this->parseUDTDecItem(); });
    advanceAndCheckToken(TokenKind::RCURLEY); // consume r curley
    return makeNode(OP::UDTDEC, std::move(name), std::move(topitem));
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
    return makeNode(OP::UDTDECITEM, std::move(identifier), std::move(primary));
}

/**
 * T := Primary | '(' E0 ')'
 */
Lexeme
Parser2::parseT()
{
    if (currentToken->kind == TokenKind::LPAREN)
    {
        advanceAndCheckToken(TokenKind::LPAREN); // consume l paren
        auto e0 = parseE0();
        advanceAndCheckToken(TokenKind::RPAREN); // consume r paren
        return e0;
    }

    else if (currentToken->kind == TokenKind::NEGATION ||
             currentToken->kind == TokenKind::UNARYADD ||
             currentToken->kind == TokenKind::UNARYMINUS ||
             currentToken->kind == TokenKind::NEW)
    {
        auto e1 = parseE1(makeNullNode()); // type less
        return e1;
    }

    auto primary = parsePrimary();
    return makeNode(OP::PRIMARY, std::move(primary));
}

/**
 * Primary := Bool | Integer |  String | Identifier
 */
LeafPtr
Parser2::parsePrimary()
{
    switch (currentToken->kind)
    {
    case TokenKind::BOOL:
        return parseBoolean();
    case TokenKind::INTEGER:
    case TokenKind::FLOAT:
        return parseNumber();
    case TokenKind::STRING:
        return parseString();
    case TokenKind::IDENTIFIER:
        return parseIdentifier();
    case TokenKind::LIST:
        return parseList();
    default:
        errorhandler->handle(std::make_unique<Error2>(
            Error2(currentToken->col, currentToken->line,
                   "Expected a valid primary token such as a boolean, "
                   "integer, float, string, identifier, or list.",
                   "Received: ", currentToken->value,
                   " of type " + displayKind(currentToken->kind) + ".")));
        return parseIdentifier(); //  unreachable
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
NodePtr
Parser2::parseVariable()
{
    auto type = parseType();

    // deal with void types
    if (type.get()->value == "void")
        return makeNode(OP::VARIABLE, std::move(type), std::move(type));

    auto name = parseIdentifier();
    return makeNode(OP::VARIABLE, std::move(type), std::move(name));
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
