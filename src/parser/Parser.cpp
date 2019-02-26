/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Parser.h"
#include "../ast/Ast.h"
#include <vector>

Parser::Parser()
{
}

// helper function to get next non comment token
Token*
Parser::getNextUsefulToken()
{
    currentToken = lexar->getNextToken();
    while (currentToken->getKind() == COMMENT_TOKEN ||
           currentToken->getKind() == COMMA_TOKEN)
    {
        currentToken = lexar->getNextToken();
    }
}

// public method utilized by external classes to parse a given file
ast::Start*
Parser::parse(const std::string filename)
{
    // (re)intialize the lexar to a new object with the correct file
    lexar = new Lexar(filename);

    // start from the beginning, haha how punny :)
    return parseStart();
}

/**
 * Start  := Source
 */
ast::Start*
Parser::parseStart()
{
    ast::Source* src = parseSource();

    return new ast::Start(src);
}

/**
 * Source := SourcePart*
 */
ast::Source*
Parser::parseSource()
{
    std::vector<ast::SourcePart*> srcParts;

    getNextUsefulToken();
    while (!currentToken->isEOF())
    {
        ast::SourcePart* srcPart = parseSourcePart();
        srcParts.push_back(srcPart);
    }

    return new ast::Source(srcParts);
}

/**
 * SourcePart := ExportDefinition |
 *               GeneralDecleration |
 *               FuctionDefinition |
 *               UserDefinedTypeDefinition |
 *               InitialExecutionBody
 */
ast::SourcePart*
Parser::parseSourcePart()
{
    std::string kwd;
    switch (currentToken->getKind())
    {
    case Kind::KEYWORD_TOKEN:
        kwd = currentToken->getValue();

        // increment token since this one's time of importance is over
        getNextUsefulToken();

        if (kwd == "exp")
        {
            return (ast::SourcePart*)parseExportDefinition();
        }
        else if (kwd == "fun")
        {
            return (ast::SourcePart*)parseFunctionDefintion();
        }
        else if (kwd == "dec")
        {
            return (ast::SourcePart*)parseGeneralDecleration();
        }
        else if (kwd == "Cat")
        {
            return (ast::SourcePart*)parseUserDefinedTypeDefinition();
        }
        break;
    case Kind::START_TOKEN:
        // consume 'start'
        getNextUsefulToken();
        return (ast::SourcePart*)parseInitialExecutionBody();
    default:
        throw std::invalid_argument(
            "unexpected source level code. Expected function, var decleration, "
            "structure definition, or initial execution statement.");
    }
}

/**
 * ExportDefinition := 'exp' Exportable
 */
ast::ExportDefinition*
Parser::parseExportDefinition()
{
    ast::Exportable* exprt = parseExportable();

    return new ast::ExportDefinition(exprt);
}

/**
 * Exportable := FunctionDefinition | GeneralDecleration
 */
ast::Exportable*
Parser::parseExportable()
{
    std::string kwd = currentToken->getValue();
    if (kwd == "fun")
    {
        // consume 'fun'
        getNextUsefulToken();

        return (ast::Exportable*)parseFunctionDefintion();
    }
    else if (kwd == "dec")
    {
        // consume 'dec'
        getNextUsefulToken();

        return (ast::Exportable*)parseGeneralDecleration();
    }
    else
    {
        throw std::invalid_argument(
            "expected a function or variable decleration");
    }
}

/**
 * FunctionDefinition := FunctionName FunctionInput FunctionOutput FunctionBody
 * FunctionName := 'fun' Identifier
 * FunctionInput := '<-' InputList
 * FunctionOutput := '->' OutputList
 * FunctionBody := Block
 */
ast::FunctionDefinition*
Parser::parseFunctionDefintion()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // consume function name
    getNextUsefulToken();
    // consume '<-'
    getNextUsefulToken();

    std::vector<ast::Input*> inputs;
    while (currentToken->getKind() != Kind::ARROW_TOKEN)
    {
        inputs.push_back(parseInput());

        // consume leftover token from parseInput()
        getNextUsefulToken();
    }

    // consume '->'
    getNextUsefulToken();

    std::vector<ast::Output*> outputs;
    while (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {
        outputs.push_back(parseOutput());

        // consume leftover token from parseOutput()
        getNextUsefulToken();
    }

    ast::Block* body = parseBlock();

    return new ast::FunctionDefinition(name, inputs, outputs, body);
}

/**
 * Input := Variable
 */
ast::Input*
Parser::parseInput()
{
    ast::Variable* var = parseVariable();

    return new ast::Input(var);
}

/**
 * Output := Typename
 */
ast::Output*
Parser::parseOutput()
{
    ast::Typename* type = new ast::Typename(currentToken->getValue());

    return new ast::Output(type);
}

/**
 * GeneralDecleration := 'dec' GeneralDefinition
 */
ast::GeneralDecleration*
Parser::parseGeneralDecleration()
{
    ast::GeneralDefinition* gd = parseGeneralDefinition();

    return new ast::GeneralDecleration(gd);
}

/**
 * GeneralDefinition := ListDefinition |
 *                      DictionaryDefinition |
 *                      NewVariableDefinition
 */
ast::GeneralDefinition*
Parser::parseGeneralDefinition()
{
    std::string val = currentToken->getValue();
    if (val == "list")
    {
        // consume 'list'
        getNextUsefulToken();

        return (ast::GeneralDefinition*)parseListDefinition();
    }
    else if (val == "dictionary")
    {
        // consume 'dictionary'
        getNextUsefulToken();

        return (ast::GeneralDefinition*)parseDictionaryDefinition();
    }
    else if (val == "bool" || val == "str" || val == "int" || val == "flt" ||
             val == "void")
    {
        return (ast::GeneralDefinition*)parseNewVariableDefinition();
    }
    else
    {
        throw std::invalid_argument("expected a list, dictionary, bool, str, "
                                    "int, flt, or void definition");
    }
}

/**
 * UserDefinedTypeDefinition := 'Cat' UserDefinedTypeAttributes
 * UserDefinedTypeMethods
 */
ast::UserDefinedTypeDefinition*
Parser::parseUserDefinedTypeDefinition()
{
    ast::UserDefinedTypeAttributes* attributes = UserDefinedTypeAttributes();
    ast::UserDefinedTypeMethods* methods = UserDefinedTypeMethods();

    return new ast::UserDefinedTypeDefinition(attributes, methods);
}

/**
 * UserDefinedTypeAttributes := Identifier '{' Variable* '}'
 */
ast::UserDefinedTypeAttributes*
Parser::UserDefinedTypeAttributes()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // consume identifier
    getNextUsefulToken();
    // consume '{'
    getNextUsefulToken();

    std::vector<ast::Variable*> attributes;

    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        attributes.push_back(parseVariable());
        getNextUsefulToken();
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::UserDefinedTypeAttributes(name, attributes);
}

/**
 * UserDefinedTypeMethods := 'Cfn' Identifier '{' FunctionDefinition* '}'
 */
ast::UserDefinedTypeMethods*
Parser::UserDefinedTypeMethods()
{
    // consume 'Cfn'
    getNextUsefulToken();

    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // consume identifier
    getNextUsefulToken();
    // consume '{'
    getNextUsefulToken();

    std::vector<ast::FunctionDefinition*> methods;
    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        // consume 'fun'
        getNextUsefulToken();

        methods.push_back(parseFunctionDefintion());

        // consume last token leftover from parseFunctionDefinition()
        getNextUsefulToken();
    }

    // skip '}'
    getNextUsefulToken();

    return new ast::UserDefinedTypeMethods(name, methods);
}

/**
 * InitialExecutionBody := 'start' Block
 */
ast::InitialExecutionBody*
Parser::parseInitialExecutionBody()
{
    ast::Block* body = parseBlock();
    return new ast::InitialExecutionBody(body);
}

/**
 * ListDefinition := 'list' Identifier = Expression
 */
ast::ListDefinition*
Parser::parseListDefinition()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // move to next token
    getNextUsefulToken();

    ast::Expression* expr = parseExpression();

    return new ast::ListDefinition(name, expr);
}

/**
 * DictionaryDefinition := ''dictionary' Identifier = Expression
 */
ast::DictionaryDefinition*
Parser::parseDictionaryDefinition()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // consume identifier
    getNextUsefulToken();

    ast::Expression* expr = parseExpression();

    return new ast::DictionaryDefinition(name, expr);
}

/**
 * NewVariableDefinition := Variable '=' Expression
 */
ast::NewVariableDefinition*
Parser::parseNewVariableDefinition()
{
    ast::Variable* var = parseVariable();

    ast::Expression* expr = (ast::Expression*)parseExpression();

    return new ast::NewVariableDefinition(var, expr);
}

/**
 * Expression := ArrayExpression |
 *               MemberAccess |
 *               FunctionCall |
 *               '(' ')' |
 *               '!' Expression |
 *               '**' Expression |
 *               ('*' | '/' | '%') Expression |
 *               ('+' | '-') Expression|
 *               ('>' | '<' | '>=' | '<=') Expression |
 *               ('==' | '!=') Expression|
 *               'and' Expression |
 *               'or' Expression |
 *               '=' Expression |
 *               PrimaryExpression
 */
ast::Expression*
Parser::parseExpression()
{
    std::string tk = currentToken->getValue();

    if (tk == "new")
    {
        // consume 'new'
        getNextUsefulToken();

        return (ast::Expression*)parseNewExpression();
    }
    else if (tk == "[")
    {
        // consume '['
        getNextUsefulToken();

        return (ast::Expression*)parseArrayExpression();
    }
    else if (tk == "|")
    {
        // consume '|'
        getNextUsefulToken();

        return (ast::Expression*)parseBinaryExpression();
    }
    else if (tk == "." || tk == "...")
    {
        return (ast::Expression*)parseMemberAccess();
    }
    else if (tk == "(")
    {
        // consume '('
        getNextUsefulToken();

        return (ast::Expression*)parseFunctionCall();
    }
    else if (tk == "!")
    {
        // consume '!'
        getNextUsefulToken();

        return (ast::Expression*)new ast::Negation(parseExpression());
    }
    else if (tk == "**")
    {
        // consume '**'
        getNextUsefulToken();

        return (ast::Expression*)new ast::Exponentiation(parseExpression());
    }
    else if (tk == "*" || tk == "/" || tk == "%")
    {
        if (tk == "*")
        {
            // consume '*'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Multiplication(parseExpression());
        }
        else if (tk == "/")
        {
            // consume '/'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Division(parseExpression());
        }
        else if (tk == "%")
        {
            // consume '%'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Modulo(parseExpression());
        }
    }
    else if (tk == "+" || tk == "-")
    {
        if (tk == "+")
        {
            // consume '+'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Addition(parseExpression());
        }
        else if (tk == "-")
        {
            // consume '-'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Subtraction(parseExpression());
        }
    }
    else if (tk == ">" || tk == "<" || tk == ">=" || tk == "<=")
    {
        if (tk == ">")
        {
            // consume '=='
            getNextUsefulToken();

            return (ast::Expression*)new ast::BinaryGreaterThan(
                parseExpression());
        }
        else if (tk == "<")
        {
            // skip '<
            getNextUsefulToken();

            return (ast::Expression*)new ast::BinaryLessThan(parseExpression());
        }
        else if (tk == ">=")
        {
            // consume '>='
            getNextUsefulToken();

            return (ast::Expression*)new ast::BinaryGreaterThanOrEqual(
                parseExpression());
        }
        else if (tk == "<=")
        {
            // consume '<='
            getNextUsefulToken();

            return (ast::Expression*)new ast::BinaryLessThanOrEqual(
                parseExpression());
        }
    }
    else if (tk == "==" || tk == "!=")
    {
        if (tk == "==")
        {
            // consume '=='
            getNextUsefulToken();

            return (ast::Expression*)new ast::EquivalenceComparison(
                parseExpression());
        }
        else
        {
            // consume '!='
            getNextUsefulToken();

            return (ast::Expression*)new ast::NonEquivalenceComparison(
                parseExpression());
        }
    }
    else if (tk == "and")
    {
        // consume 'and'
        getNextUsefulToken();

        return (ast::Expression*)new ast::AndComparison(parseExpression());
    }
    else if (tk == "or")
    {
        // consume 'or'
        getNextUsefulToken();

        return (ast::Expression*)new ast::OrComparison(parseExpression());
    }
    else if (tk == "=")
    {
        // consume '='
        getNextUsefulToken();

        return (ast::Expression*)new ast::Assignment(parseExpression());
    }
    else
    {
        return (ast::Expression*)parsePrimaryExpression();
    }
}

/**
 * NewExpression := New
 */
ast::NewExpression*
Parser::parseNewExpression()
{
    ast::New* newVal = parseNew();

    return new ast::NewExpression(newVal);
}

/**
 * NewExpression := ListLiteral | DictionaryLiteral
 */
ast::New*
Parser::parseNew()
{
    Kind kind = currentToken->getKind();

    if (kind == Kind::LBRACKET_TOKEN)
    {
        // consume '['
        getNextUsefulToken();
        return (ast::New*)parseListLiteral();
    }
    else if (kind == Kind::LCURLEY_TOKEN)
    {
        // consume '{'
        getNextUsefulToken();
        return (ast::New*)parseDictionaryLiteral();
    }
    else
    {
        throw std::invalid_argument("expected a '[' or a '{'");
    }
}

/**
 * ArrayExpression := '[' Expression (',' Expression)* ']'
 */
ast::ArrayExpression*
Parser::parseArrayExpression()
{
    bool canBeIndex = true;

    // if first element is not an integer it can't be an index access
    if (currentToken->getKind() != Kind::INTEGER_TOKEN)
    {
        canBeIndex = false;
    }

    std::vector<ast::Expression*> exprs;

    while (currentToken->getKind() != Kind::RBRACKET_TOKEN)
    {
        exprs.push_back(parseExpression());
    }

    // if more than one expression in list, it cannot be an index access
    if (exprs.size() != 1)
    {
        canBeIndex = false;
    }

    // consume ']'
    getNextUsefulToken();

    return new ast::ArrayExpression(exprs, canBeIndex);
}

/**
 * BinaryExpression := '|' Expression* '|'
 */
ast::BinaryExpression*
Parser::parseBinaryExpression()
{
    std::vector<ast::Expression*> exprs;

    while (currentToken->getKind() != Kind::PIPE_TOKEN)
    {
        exprs.push_back(parseExpression());
    }

    // consume '|'
    getNextUsefulToken();

    return new ast::BinaryExpression(exprs);
}

/**
 * MemberAccess := '.' Identifier
 */
ast::MemberAccess*
Parser::parseMemberAccess()
{
    Kind kind = currentToken->getKind();
    switch (kind)
    {
    case Kind::DOT_TOKEN:
        // consume '.'
        getNextUsefulToken();

        return (ast::MemberAccess*)parseAttributeAccess();
    case Kind::TRIPLE_DOT_TOKEN:
        // consume '...'
        getNextUsefulToken();

        return (ast::MemberAccess*)parseMethodAccess();
    default:
        throw std::invalid_argument("unexpected member accesser");
    }
}

/**
 * AttributeAccess := '...' Identifier
 */
ast::AttributeAccess*
Parser::parseAttributeAccess()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // consume identifier
    getNextUsefulToken();

    return new ast::AttributeAccess(name);
}

/**
 * MethodAccess := '...' Identifier FunctionCall
 */
ast::MethodAccess*
Parser::parseMethodAccess()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    // consume identifier
    getNextUsefulToken();

    ast::FunctionCall* fc = parseFunctionCall();

    return new ast::MethodAccess(name, fc);
}

/**
 * FunctionCall := '(' [Expression] (',' Expression)*')'
 */
ast::FunctionCall*
Parser::parseFunctionCall()
{
    // consume '('
    getNextUsefulToken();

    std::vector<ast::Identifier*> idents;

    while (currentToken->getKind() != Kind::RPAREN_TOKEN)
    {
        idents.push_back(new ast::Identifier(currentToken->getValue()));

        // consume identifier
        getNextUsefulToken();
    }

    // consume ')'
    getNextUsefulToken();

    return new ast::FunctionCall(idents);
}

/**
 * PrimaryExpression := Primary
 */
ast::PrimaryExpression*
Parser::parsePrimaryExpression()
{
    return new ast::PrimaryExpression(parsePrimary());
}

/**
 * Primary := BooleanLiteral |
 *            IntegerLiteral |
 *            FloatLiteral |
 *            StringLiteral |
 *            Identifier
 */
ast::Primary*
Parser::parsePrimary()
{

    std::string tk = currentToken->getValue();
    Kind kind = currentToken->getKind();

    // consume all since they are captured above
    getNextUsefulToken();

    if (kind == Kind::BOOL_TOKEN)
    {
        return (ast::Primary*)new ast::BooleanLiteral(tk);
    }
    else if (kind == Kind::INTEGER_TOKEN)
    {
        return (ast::Primary*)new ast::IntegerLiteral(tk);
    }
    else if (kind == Kind::FLOAT_TOKEN)
    {
        return (ast::Primary*)new ast::FloatLiteral(tk);
    }
    else if (kind == Kind::STRING_TOKEN)
    {
        return (ast::Primary*)new ast::StringLiteral(tk);
    }
    else if (kind == Kind::BYTE_TOKEN)
    {
        return (ast::Primary*)new ast::ByteLiteral(tk);
    }
    else if (kind == Kind::IDENTIFIER_TOKEN)
    {
        return (ast::Primary*)new ast::Identifier(tk);
    }
    else
    {
        throw std::invalid_argument("unexpected literal");
    }
}

/**
 * DictionaryLiteral := '{' [DictionaryItem (',' DictionaryItem)] '}'
 */
ast::DictionaryLiteral*
Parser::parseDictionaryLiteral()
{
    std::vector<ast::DictionaryItem*> dictionaryItems;

    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        dictionaryItems.push_back(parseDictionaryItem());
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::DictionaryLiteral(dictionaryItems);
}

/**
 * DictionaryItem := Identifier ':' Identifier
 */
ast::DictionaryItem*
Parser::parseDictionaryItem()
{
    ast::Identifier* key = new ast::Identifier(currentToken->getValue());

    // move on to next token
    getNextUsefulToken();

    // skip ':'
    getNextUsefulToken();

    ast::Identifier* value = new ast::Identifier(currentToken->getValue());

    // move on to next token
    getNextUsefulToken();

    return new ast::DictionaryItem(key, value);
}

/**
 * ListLiteral := '[' [ListItem (',' ListItem)] ']'
 */
ast::ListLiteral*
Parser::parseListLiteral()
{
    std::vector<ast::ListItem*> listItems;

    while (currentToken->getKind() != Kind::RBRACKET_TOKEN)
    {
        listItems.push_back(parseListItem());
    }

    // consume ']'
    getNextUsefulToken();

    return new ast::ListLiteral(listItems);
}

/**
 * ListItem := Identifier
 */
ast::ListItem*
Parser::parseListItem()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue());
    // move on to next token
    getNextUsefulToken();

    return new ast::ListItem(name);
}

/**
 * Variable := TypeName Identifier
 */
ast::Variable*
Parser::parseVariable()
{

    // TODO: fix this hack for void input
    if (std::string("void").compare(currentToken->getValue()) == 0)
    {
        ast::Typename* type = new ast::Typename("void");
        ast::Identifier* id = new ast::Identifier("void");
        return new ast::Variable(type, id);
    }

    ast::Typename* type = new ast::Typename(currentToken->getValue());

    // consume typename
    getNextUsefulToken();

    ast::Identifier* id = new ast::Identifier(currentToken->getValue());

    // consume identifier
    getNextUsefulToken();

    return new ast::Variable(type, id);
}

/**
 * Block := { Statement* }
 */
ast::Block*
Parser::parseBlock()
{
    std::vector<ast::Statement*> statements;

    // consume '{'
    getNextUsefulToken();

    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        ast::Statement* s = parseStatement();
        statements.push_back(s);
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::Block(statements);
}

/**
 * Statement := IfStatement |
 *              Block |
 *              ReturnStatement |
 *              GeneralDecleration |
 *              ExpressionStatement
 */
ast::Statement*
Parser::parseStatement()
{
    std::string tk = currentToken->getValue();

    if (tk == "if")
    {
        // consume 'if'
        getNextUsefulToken();

        return (ast::Statement*)parseIfStatement();
    }
    else if (tk == "{")
    {
        return (ast::Statement*)parseBlock();
    }
    else if (tk == "return")
    {
        // consume 'return'
        getNextUsefulToken();

        return (ast::Statement*)parseReturnStatement();
    }
    else if (tk == "dec")
    {
        // consume 'dec'
        getNextUsefulToken();

        return (ast::Statement*)parseGeneralDecleration();
    }
    else
    {
        return (ast::Statement*)parseExpressionStatement();
    }
}

/**
 * IfStatement := 'if' Expression Block 'else' Block
 */
ast::IfStatement*
Parser::parseIfStatement()
{
    ast::Expression* ifExpr = parseExpression();

    ast::Block* ifStatements = parseBlock();

    // skip 'else'
    getNextUsefulToken();

    ast::Block* elseStatements = parseBlock();

    return new ast::IfStatement(ifExpr, ifStatements, elseStatements);
}

/**
 *
 */
ast::ExpressionStatement*
Parser::parseExpressionStatement()
{
    ast::Expression* expr = parseExpression();
    return new ast::ExpressionStatement(expr);
}

/**
 * ReturnStatement := 'return' Expression
 */
ast::ReturnStatement*
Parser::parseReturnStatement()
{
    ast::Expression* expr = parseExpression();

    return new ast::ReturnStatement(expr);
}