#include "Parser2.h"

// -------- Parser Helper Code --------- //
void
Parser2::advanceAndCheckToken(const Tokenn::Kind& k)
{
    // first check value and kind
    if (currentToken->kind != k)
    {
        // error
    }
    currentToken = lexar->getNextToken();

    // catch errors from the lexar
    if (currentToken->kind == Tokenn::Kind::ERROR)
    {
        // error
    }

    while (currentToken->kind == Tokenn::Kind::COMMENT ||
           currentToken->kind == Tokenn::Kind::COMMA)
    {
        currentToken = lexar->getNextToken();

        // catch errors from the lexar
        if (currentToken->kind == Tokenn::Kind::ERROR)
        {
            // error
        }
    }
}

Parser2::Parser2(const std::string& filename)
{
    lexar = std::make_unique<Lexar2>(filename);
    currentToken = lexar->getNextToken();
}

std::shared_ptr<NodeLexeme>
Parser2::parse()
{
    return parseProgram();
}

// -------- Parser Parse Methods --------- //
/**
 * Program := Source
 */
std::shared_ptr<NodeLexeme>
Parser2::parseProgram()
{
    return parseSource();
}
/**
 * Source := Import Source | SourcePart
 */
std::shared_ptr<NodeLexeme>
Parser2::parseSource()
{
    auto imports = getChain(false, Tokenn::Kind::IMPORT, OP::IMPORT,
                            [this]() -> std::shared_ptr<NodeLexeme> {
                                return this->parseImportInfo();
                            });
    auto sourcepart = parseSourcePart();
    return makeNode(OP::SOURCE, imports, sourcepart);
}

/**
 * ImportInfo := UDName Location
 */
std::shared_ptr<NodeLexeme>
Parser2::parseImportInfo()
{
    advanceAndCheckToken(Tokenn::Kind::IMPORT); // eat 'Import'
    auto name = parseUDName();
    advanceAndCheckToken(Tokenn::Kind::COLON); // eat colon
    auto loc = parseLocation();
    return makeNode(OP::IMPORT, name, loc);
}

/**
 * UDName := Identifier
 */
std::shared_ptr<LeafLexeme>
Parser2::parseUDName()
{
    return parseIdentifier();
}

/**
 * Location := String
 */
std::shared_ptr<LeafLexeme>
Parser2::parseLocation()
{
    return parseString();
}

/**
 * SourcePart := UDT | UDA | Script
 */
std::shared_ptr<NodeLexeme>
Parser2::parseSourcePart()
{
    switch (currentToken->kind)
    {
    case Tokenn::Kind::UAT:
        return parseUDT();
    default:
        return parseScript();
    }
}

/**
 * UDT := UserDefinedType
 */
std::shared_ptr<NodeLexeme>
Parser2::parseUDT()
{
    return parseUserDefinedType();
}

/**
 * UserDefinedType := Attributes Methods
 */
std::shared_ptr<NodeLexeme>
Parser2::parseUserDefinedType()
{
    auto attributes = parseAttributes();
    auto methods = parseMethods();
    return makeNode(OP::UDT, attributes, methods);
}

/**
 * Attributes := [Identifier Identifier]*
 */
std::shared_ptr<NodeLexeme>
Parser2::parseAttributes()
{
    advanceAndCheckToken(Tokenn::Kind::UAT);     // consume uat
    advanceAndCheckToken(Tokenn::Kind::LCURLEY); // consume l curley
    auto topattribute = getChain(true, Tokenn::Kind::RCURLEY, OP::ATTRIBUTE,
                                 [this]() -> std::shared_ptr<NodeLexeme> {
                                     return this->parseVariable();
                                 });
    advanceAndCheckToken(Tokenn::Kind::RCURLEY); // consume r curley
    return makeNode(OP::ATTRIBUTE, topattribute);
}

/**
 * Methods := [FunctionDefinition]*
 */
std::shared_ptr<NodeLexeme>
Parser2::parseMethods()
{
    advanceAndCheckToken(Tokenn::Kind::UFN);     // consume ufn
    advanceAndCheckToken(Tokenn::Kind::LCURLEY); // consume l curley
    auto topmethod = getChain(true, Tokenn::Kind::RCURLEY, OP::METHOD,
                              [this]() -> std::shared_ptr<NodeLexeme> {
                                  return this->parseFunctionDefinition();
                              });
    advanceAndCheckToken(Tokenn::Kind::RCURLEY); // consume r curley
    return makeNode(OP::METHOD, topmethod);
}

std::shared_ptr<NodeLexeme>
Parser2::parseScript()
{
    return parseScript_();
}

std::shared_ptr<NodeLexeme>
Parser2::parseScript_()
{
    auto func = getChain(true, Tokenn::Kind::START, OP::FUNCTION,
                         [this]() -> std::shared_ptr<NodeLexeme> {
                             return this->parseFunctionDefinition();
                         });
    auto start = parseStart();
    return makeNode(OP::SCRIPT, func, start);
}

/**
 * FunctionDefinition := '(' 'fun' identifier FunctionInfo ')'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseFunctionDefinition()
{
    advanceAndCheckToken(Tokenn::Kind::LPAREN); // consume l paren
    advanceAndCheckToken(Tokenn::Kind::FUN);    // consume fun
    auto id = parseIdentifier();
    auto info = parseFunctionInfo();
    advanceAndCheckToken(Tokenn::Kind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION, id, info);
}

/**
 * FunctionInfo := FunctionInOut Block
 */
std::shared_ptr<NodeLexeme>
Parser2::parseFunctionInfo()
{
    auto inout = parseFunctionInOut();
    auto block = parseBlock();
    return makeNode(OP::FUNCTION_INFO, inout, block);
}

/**
 * FunctionInOut := FunctionInputs* FunctionOutputs
 */
std::shared_ptr<NodeLexeme>
Parser2::parseFunctionInOut()
{
    auto inputs = parseFunctionInputs();
    auto output = parseFunctionOutput();
    return makeNode(OP::FUNCTION_IN_OUT, inputs, output);
}

/**
 * FunctionInputs :=  '(' Variable [',' Variable]* ')'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseFunctionInputs()
{
    advanceAndCheckToken(
        Tokenn::Kind::LPAREN); // consume l paren
                               // TODO: check for multiple voids
    auto topinput = getChain(true, Tokenn::Kind::RPAREN, OP::FUNCTION_INPUT,
                             [this]() -> std::shared_ptr<NodeLexeme> {
                                 return this->parseVariable();
                             });
    advanceAndCheckToken(Tokenn::Kind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION_INPUT, topinput);
}

/**
 * FunctionOutput := '(' Type ')'
 */
std::shared_ptr<LeafLexeme>
Parser2::parseFunctionOutput()
{
    advanceAndCheckToken(Tokenn::Kind::LPAREN); // consume l paren
    auto type = parseType();
    advanceAndCheckToken(Tokenn::Kind::RPAREN); // consume r paren
    return type;
}

/**
 * Start := 'start' Block
 */
std::shared_ptr<NodeLexeme>
Parser2::parseStart()
{
    advanceAndCheckToken(Tokenn::Kind::START);
    auto block = parseBlock();
    return makeNode(OP::START, makeLeaf(LIT::IDENTIFIER, "start"), block);
}

/**
 * Block = '{' Statement* '}'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseBlock()
{
    advanceAndCheckToken(Tokenn::Kind::LCURLEY); // eat '{'
    auto topstatement = getChain(true, Tokenn::Kind::RCURLEY, OP::STATEMENT,
                                 [this]() -> std::shared_ptr<NodeLexeme> {
                                     return this->parseStatement();
                                 });
    std::shared_ptr<NodeLexeme> parseBlockRecurse();

    advanceAndCheckToken(Tokenn::Kind::RCURLEY); // eat '}'
    return makeNode(OP::BLOCK, topstatement);
}

/**
 * Statement := Tree | Return | Declaration | E0
 */
std::shared_ptr<NodeLexeme>
Parser2::parseStatement()
{
    switch (currentToken->kind)
    {
    case Tokenn::Kind::TREE:
        return parseTree();
    case Tokenn::Kind::RETURN:
        return parseReturn();
    case Tokenn::Kind::DEC:
        return parseDeclaration();
    default:
        return parseE0();
    }
}

/**
 * Tree := 'tree' (' Branch* ')'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseTree()
{

    advanceAndCheckToken(Tokenn::Kind::TREE);   // eat 'tree'
    advanceAndCheckToken(Tokenn::Kind::LPAREN); // eat '('
    std::vector<std::shared_ptr<NodeLexeme>> branches;

    auto topbranch = getChain(true, Tokenn::Kind::RPAREN, OP::BRANCH,
                              [this]() -> std::shared_ptr<NodeLexeme> {
                                  return this->parseBranch();
                              });
    advanceAndCheckToken(Tokenn::Kind::RPAREN); // eat ')'
    return makeNode(OP::TREE, topbranch);
}

/**
 * Branch := '(' Grouping Block')'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseBranch()
{
    advanceAndCheckToken(Tokenn::Kind::LPAREN); // eat '('
    auto grouping = parseGrouping();
    auto block = parseBlock();
    advanceAndCheckToken(Tokenn::Kind::RPAREN); // eat ')'
    return makeNode(OP::BRANCH, grouping, block);
}

/**
 * Grouping := '|' E0 '|'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseGrouping()
{
    advanceAndCheckToken(Tokenn::Kind::PIPE); // eat '|'
    auto condition = parseE0();
    advanceAndCheckToken(Tokenn::Kind::PIPE); // eat '|'
    return condition;
}

/**
 * Return := 'return' T
 */
std::shared_ptr<NodeLexeme>
Parser2::parseReturn()
{
    advanceAndCheckToken(Tokenn::Kind::RETURN); // consume 'return'
    auto t = parseT();
    return makeNode(OP::RETURN, t);
}

/**
 * Declaration :=  'dec' Variable '=' E0
 */
std::shared_ptr<NodeLexeme>
Parser2::parseDeclaration()
{
    advanceAndCheckToken(Tokenn::Kind::DEC); // consume 'dec'
    auto var = parseVariable();
    advanceAndCheckToken(Tokenn::Kind::ASSIGNMENT); // consume '='
    auto e0 = parseE0();
    return makeNode(OP::DECLARATION, var, e0);
}

/**
 * E0 := T0 E1
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE0()
{
    auto t0 = parseT();
    return parseE1(t0);
}

/**
 * E1 := ** T1 E0 | E2
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE1(std::shared_ptr<NodeLexeme> T0)
{
    if (currentToken->kind == Tokenn::Kind::EXPONENTIATION)
    {
        advanceAndCheckToken(Tokenn::Kind::EXPONENTIATION); // consume "**"
        auto e0 = parseE0();
        return makeNode(OP::EXPONENT, T0, e0);
    }
    return parseE2(T0);
}

/**
 * E2 := * T2 E0 | / T2 E0 | % T2 E0 | E3
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE2(std::shared_ptr<NodeLexeme> T1)
{
    if (currentToken->kind == Tokenn::Kind::MULTIPLICATION ||
        currentToken->kind == Tokenn::Kind::DIVISION ||
        currentToken->kind == Tokenn::Kind::MODULO)
    {
        switch (currentToken->kind)
        {
        case Tokenn::Kind::MULTIPLICATION:
        {
            advanceAndCheckToken(Tokenn::Kind::MULTIPLICATION); // consume "*"
            auto e0 = parseE0();
            return makeNode(OP::MULTIPLICATION, T1, e0);
        }
        case Tokenn::Kind::DIVISION:
        {
            advanceAndCheckToken(Tokenn::Kind::DIVISION); // consume "/"
            auto e0 = parseE0();
            return makeNode(OP::DIVISION, T1, e0);
        }
        case Tokenn::Kind::MODULO:
        {
            advanceAndCheckToken(Tokenn::Kind::MODULO); // consume "%"
            auto e0 = parseE0();
            return makeNode(OP::MODULO, T1, e0);
        }
        }
    }
    return parseE3(T1);
}

/**
 * E3 := + T3 E0 | - T3 E0 | E4
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE3(std::shared_ptr<NodeLexeme> T2)
{
    if (currentToken->kind == Tokenn::Kind::ADDITION ||
        currentToken->kind == Tokenn::Kind::SUBTRACTION)
    {

        switch (currentToken->kind)
        {
        case Tokenn::Kind::ADDITION:
        {
            advanceAndCheckToken(Tokenn::Kind::ADDITION); // consume "+"
            auto e0 = parseE0();
            return makeNode(OP::ADDITION, T2, e0);
        }
        case Tokenn::Kind::SUBTRACTION:
        {
            advanceAndCheckToken(Tokenn::Kind::SUBTRACTION); // consume "-"
            auto e0 = parseE0();
            return makeNode(OP::SUBTRACTION, T2, e0);
        }
        }
    }
    return parseE4(T2);
}

/**
 * E4 := < T4 E0 | > T4 E0 | <= T4 E0 | >= T4 E0 | E5
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE4(std::shared_ptr<NodeLexeme> T3)
{
    if (currentToken->kind == Tokenn::Kind::LESS_THAN ||
        currentToken->kind == Tokenn::Kind::LESS_THAN_OR_EQUALS ||
        currentToken->kind == Tokenn::Kind::GREATER_THAN ||
        currentToken->kind == Tokenn::Kind::GREATER_THAN_OR_EQUALS)
    {
        auto t4 = parseType();
        auto e0 = parseE0();
        switch (currentToken->kind)
        {
        case Tokenn::Kind::LESS_THAN:
        {
            advanceAndCheckToken(Tokenn::Kind::LESS_THAN); // consume "<"
            auto e0 = parseE0();
            return makeNode(OP::LESS_THAN, T3, e0);
        }
        case Tokenn::Kind::LESS_THAN_OR_EQUALS:
        {
            advanceAndCheckToken(
                Tokenn::Kind::LESS_THAN_OR_EQUALS); // consume "<"
            auto e0 = parseE0();
            return makeNode(OP::LESS_THAN_OR_EQUALS, T3, e0);
        }
        case Tokenn::Kind::GREATER_THAN:
        {
            advanceAndCheckToken(Tokenn::Kind::GREATER_THAN); // consume ">"
            auto e0 = parseE0();
            return makeNode(OP::GREATER_THAN, T3, e0);
        }
        case Tokenn::Kind::GREATER_THAN_OR_EQUALS:
        {
            advanceAndCheckToken(
                Tokenn::Kind::GREATER_THAN_OR_EQUALS); // consume ">="
            auto e0 = parseE0();
            return makeNode(OP::GREATER_THAN_OR_EQUALS, T3, e0);
        }
        }
    }
    return parseE5(T3);
}

/**
 * E5 := == T5 E0 | != T5 E0 | E6
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE5(std::shared_ptr<NodeLexeme> T4)
{
    if (currentToken->kind == Tokenn::Kind::EQUIVALENCE ||
        currentToken->kind == Tokenn::Kind::NONEQUIVALENCE)
    {

        switch (currentToken->kind)
        {
        case Tokenn::Kind::EQUIVALENCE:
        {
            advanceAndCheckToken(Tokenn::Kind::EQUIVALENCE); // consume "=="
            auto e0 = parseE0();
            return makeNode(OP::EQUIVALENCE, T4, e0);
        }
        case Tokenn::Kind::NONEQUIVALENCE:
        {
            advanceAndCheckToken(Tokenn::Kind::NONEQUIVALENCE); // consume "!="
            auto e0 = parseE0();
            return makeNode(OP::NONEQUIVALENCE, T4, e0);
        }
        }
    }
    return parseE6(T4);
}

/**
 * E6 := 'and' T6 E0 | 'or' T6 E0 | E7
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE6(std::shared_ptr<NodeLexeme> T5)
{
    if (currentToken->kind == Tokenn::Kind::AND ||
        currentToken->kind == Tokenn::Kind::OR)
    {
        switch (currentToken->kind)
        {
        case Tokenn::Kind::AND:
        {
            advanceAndCheckToken(Tokenn::Kind::AND); // consume 'and'
            auto e0 = parseE0();
            return makeNode(OP::AND, T5, e0);
        }
        case Tokenn::Kind::OR:
        {
            advanceAndCheckToken(Tokenn::Kind::OR); // consume 'or'
            auto e0 = parseE0();
            return makeNode(OP::OR, T5, e0);
        }
        }
    }
    return parseE7(T5);
}

/**
 * E7 := = T7 E0 | E8
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE7(std::shared_ptr<NodeLexeme> T6)
{
    if (currentToken->kind == Tokenn::Kind::ASSIGNMENT)
    {
        advanceAndCheckToken(Tokenn::Kind::ASSIGNMENT); // consume "="
        auto e0 = parseE0();
        return makeNode(OP::ASSIGNMENT, T6, e0);
    }
    return parseE8(T6);
}

/**
 * E8 := ! T8 E0 | E9
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE8(std::shared_ptr<NodeLexeme> T7)
{
    if (currentToken->kind == Tokenn::Kind::NEGATION)
    {
        advanceAndCheckToken(Tokenn::Kind::NEGATION); // consume "!"
        auto e0 = parseE0();
        return makeNode(OP::NEGATION, T7, e0);
    }
    return parseE9(T7);
}

/**
 * E9 := MemberAccess E0 | E10
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE9(std::shared_ptr<NodeLexeme> T8)
{
    if (currentToken->kind == Tokenn::Kind::DOT ||
        currentToken->kind == Tokenn::Kind::TRIPLE_DOT)
    {
        auto memberAccess = parseMemberAccess();
        auto e1 =
            parseE1(makeNullNode()); // like calling E0 where T0 does not exist
        return makeNode(OP::MEMBER, memberAccess, e1);
    }
    return parseE10(T8);
}

/**
 * E10 := New | E11
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE10(std::shared_ptr<NodeLexeme> T9)
{
    if (currentToken->kind == Tokenn::Kind::NEW)
    {
        auto New = parseNew();
        return makeNode(OP::MEMBER, New);
    }
    return parseE11(T9);
}

/**
 * E11 := T11 | ε
 */
std::shared_ptr<NodeLexeme>
Parser2::parseE11(std::shared_ptr<NodeLexeme> T10)
{
    return makeNode(OP::END, T10);
}

/**
 * MemberAccess := AttributeAccess | MethodAccess
 */
std::shared_ptr<NodeLexeme>
Parser2::parseMemberAccess()
{
    switch (currentToken->kind)
    {
    case Tokenn::Kind::DOT:
        return parseAttributeAccess();
    case Tokenn::Kind::TRIPLE_DOT:
        return parseMethodAccess();

        // error case
    }
}

/**
 * AttributeAccess := '.' Identifier
 */
std::shared_ptr<NodeLexeme>
Parser2::parseAttributeAccess()
{
    advanceAndCheckToken(Tokenn::Kind::DOT); // consume '.'
    auto attribute = parseIdentifier();
    return makeNode(OP::ATTRIBUTE_ACCESS, attribute);
}

/**
 * MethodAccess := '...' Identifier FunctionCall
 */
std::shared_ptr<NodeLexeme>
Parser2::parseMethodAccess()
{
    advanceAndCheckToken(Tokenn::Kind::TRIPLE_DOT); // consume '...'
    auto name = parseIdentifier();
    auto method = parseFunctionCall();
    return makeNode(OP::METHOD_ACCESS, method, name);
}

/**
 * FunctionCall := '(' [Identifier [',' Identifier]*] ')'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseFunctionCall()
{
    advanceAndCheckToken(Tokenn::Kind::LPAREN); // consume l paren
    auto topinput = getChain(true, Tokenn::Kind::RPAREN, OP::INPUT,
                             [this]() -> std::shared_ptr<LeafLexeme> {
                                 return this->parseIdentifier();
                             });
    advanceAndCheckToken(Tokenn::Kind::RPAREN); // consume r paren
    return makeNode(OP::FUNCTION_CALL, topinput);
}

/**
 * New := UDTDec
 */
std::shared_ptr<NodeLexeme>
Parser2::parseNew()
{
    advanceAndCheckToken(Tokenn::Kind::NEW); // consume new
    switch (currentToken->kind)
    {
    case Tokenn::Kind::LCURLEY:
        return parseUDTDec();

        // error case
    }
}

/**
 * UDTDec := '{' [UDTDecItem [',' UDTDecItem]*] '}'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseUDTDec()
{
    advanceAndCheckToken(Tokenn::Kind::LCURLEY); // consume l curley
    auto topitem = getChain(true, Tokenn::Kind::RCURLEY, OP::UDTDECITEM,
                            [this]() -> std::shared_ptr<NodeLexeme> {
                                return this->parseUDTDecItem();
                            });
    advanceAndCheckToken(Tokenn::Kind::RCURLEY); // consume r curley
    return makeNode(OP::UDTDEC, topitem);
}

/**
 * UDTDecItem := Identifier ':' Primary
 */
std::shared_ptr<NodeLexeme>
Parser2::parseUDTDecItem()
{
    auto identifier = parseIdentifier();
    advanceAndCheckToken(Tokenn::Kind::COLON); // consume ':'
    auto primary = parsePrimary();
    return makeNode(OP::UDTDECITEM, identifier, primary);
}

/**
 * T := Primary | '(' E0 ')'
 */
std::shared_ptr<NodeLexeme>
Parser2::parseT()
{
    if (currentToken->kind == Tokenn::Kind::LPAREN)
    {
        advanceAndCheckToken(Tokenn::Kind::LPAREN); // consume l paren
        auto e0 = parseE0();
        advanceAndCheckToken(Tokenn::Kind::RPAREN); // consume r paren
        return e0;
    }

    auto primary = parsePrimary();
    return makeNode(OP::PRIMARY, primary);
}

std::shared_ptr<LeafLexeme>
Parser2::parsePrimary()
{
    switch (currentToken->kind)
    {
    case Tokenn::Kind::BOOL:
        return parseBoolean();
    case Tokenn::Kind::INTEGER:
    case Tokenn::Kind::FLOAT:
        return parseNumber();
    case Tokenn::Kind::STRING:
        return parseString();
    case Tokenn::Kind::IDENTIFIER:
        return parseIdentifier();

        // error case
    }
}

std::shared_ptr<LeafLexeme>
Parser2::parseType()
{
    return parseIdentifier();
}

std::shared_ptr<NodeLexeme>
Parser2::parseVariable()
{
    auto type = parseType();

    // deal with void types
    if (type.get()->value == "void")
        return makeNode(OP::VARIABLE, type, type);

    auto name = parseIdentifier();
    return makeNode(OP::VARIABLE, type, name);
}

std::shared_ptr<LeafLexeme>
Parser2::parseNumber()
{
    auto v = currentToken->value;
    auto k = currentToken->kind;
    advanceAndCheckToken(Tokenn::Kind::IDENTIFIER); // eat identifier

    if (k == Tokenn::Kind::INTEGER)
        return makeLeaf(LIT::INTEGER, v);
    return makeLeaf(LIT::FLOAT, v);
}

std::shared_ptr<LeafLexeme>
Parser2::parseIdentifier()
{
    auto v = currentToken->value;
    advanceAndCheckToken(Tokenn::Kind::IDENTIFIER); // eat identifier
    return makeLeaf(LIT::IDENTIFIER, v);
}

std::shared_ptr<LeafLexeme>
Parser2::parseBoolean()
{
    auto v = currentToken->value;
    advanceAndCheckToken(Tokenn::Kind::IDENTIFIER); // eat identifier
    return makeLeaf(LIT::IDENTIFIER, v);
}

std::shared_ptr<LeafLexeme>
Parser2::parseString()
{
    auto v = currentToken->value;
    advanceAndCheckToken(Tokenn::Kind::STRING); // eat string
    return makeLeaf(LIT::STRING, v);
}
