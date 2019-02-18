/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Parser.h"
#include "../ast/Node.h"
#include <iostream>
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

        if (srcPart != nullptr)
        {
            srcParts.push_back(srcPart);
        }

        getNextUsefulToken();
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
        return (ast::SourcePart*)parseInitialExecutionBody();
    }
    return nullptr;
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
        getNextUsefulToken();
        return (ast::Exportable*)parseFunctionDefintion();
    }
    else if (kwd == "dec")
    {
        getNextUsefulToken();
        return (ast::Exportable*)parseGeneralDecleration();
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

    // skip function name
    getNextUsefulToken();
    // skip '<-'
    getNextUsefulToken();

    std::vector<ast::Input*> inputs;
    while (currentToken->getKind() != Kind::ARROW_TOKEN)
    {
        inputs.push_back(parseInput());

        if (currentToken->getKind() == Kind::COMMA_TOKEN)
        {
            // getNextUsefulToken();
        }
        getNextUsefulToken();
    }

    // skip '<-'
    getNextUsefulToken();

    std::vector<ast::Output*> outputs;
    while (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {
        outputs.push_back(parseOutput());
        if (currentToken->getKind() == Kind::COMMA_TOKEN)
        {
            // getNextUsefulToken();
        }
        getNextUsefulToken();
    }

    getNextUsefulToken();

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
        getNextUsefulToken();
        return (ast::GeneralDefinition*)parseListDefinition();
    }
    else if (val == "dictionary")
    {
        getNextUsefulToken();
        return (ast::GeneralDefinition*)parseDictionaryDefinition();
    }
    else if (val == "bool" || val == "str" || val == "int" || val == "flt" ||
             val == "void")
    {
        return (ast::GeneralDefinition*)parseNewVariableDefinition();
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

    // move past identifier
    getNextUsefulToken();
    // skip '{'
    getNextUsefulToken();

    std::vector<ast::Variable*> attributes;

    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        attributes.push_back(parseVariable());
    }

    // skip '}'
    getNextUsefulToken();

    return new ast::UserDefinedTypeAttributes(name, attributes);
}

/**
 * UserDefinedTypeMethods := 'Cfn' Identifier '{' FunctionDefinition* '}'
 */
ast::UserDefinedTypeMethods*
Parser::UserDefinedTypeMethods()
{
    // skip 'Cfn'
    getNextUsefulToken();

    ast::Identifier* name = new ast::Identifier(currentToken->getValue());

    getNextUsefulToken();
    getNextUsefulToken();

    std::vector<ast::FunctionDefinition*> methods;

    return new ast::UserDefinedTypeMethods(name, methods);
}

/**
 * InitialExecutionBody := 'start' Block
 */
ast::InitialExecutionBody*
Parser::parseInitialExecutionBody()
{
    return new ast::InitialExecutionBody();
}

/**
 * ListDefinition := 'list' Identifier ['=' Expression]
 */
ast::ListDefinition*
Parser::parseListDefinition()
{
    return new ast::ListDefinition();
}

/**
 * DictionaryDefinition := ''dictionary' Identifier ['=' Expression]
 */
ast::DictionaryDefinition*
Parser::parseDictionaryDefinition()
{
    return new ast::DictionaryDefinition();
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
 * Expression := IndexAccess |
 *               MemberAccess |
 *               FunctionCall |
 *               '(' ')' |
 *               '!' Expression |
 *               '**' Expression |
 *               ('*' | '/' | '%') Expression |
 *               ('+' | '-') Expression|
 *               ('>' | '<' | '>=' | '<=') Expression |
 *               ('==' | '!=') Expression|
 *               '&&' Expression |
 *               '||' Expression |
 *               '=' Expression |
 *               PrimaryExpression
 */
ast::Expression*
Parser::parseExpression()
{
    std::string tk = currentToken->getValue();
    if (tk == "[")
    {
        // skip '['
        getNextUsefulToken();

        return (ast::Expression*)parseIndexAccess();
    }
    else if (tk == ".")
    {
        // skip '.'
        return (ast::Expression*)parseMemberAccess();
    }
    else if (tk == "(")
    {
        // skip '.'
        return (ast::Expression*)parseFunctionCall();
    }
    else if (tk == "!")
    {
        // skip '!'
        getNextUsefulToken();

        return (ast::Expression*)new ast::Negation(parseExpression());
    }
    else if (tk == "**")
    {
        // skip '**'
        getNextUsefulToken();

        return (ast::Expression*)new ast::Exponentiation(parseExpression());
    }
    else if (tk == "*" || tk == "/" || tk == "%")
    {
        if (tk == "*")
        {
            // skip '*'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Multiplication(parseExpression());
        }
        else if (tk == "/")
        {
            // skip '/'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Division(parseExpression());
        }
        else if (tk == "%")
        {
            // skip '%'
            getNextUsefulToken();

            return (ast::Expression*)new ast::Modulo(parseExpression());
        }
    }
    else if (tk == ">" || tk == "<" || tk == ">=" || tk == "<=")
    {
        if (tk == ">")
        {
            // skip '=='
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
            // skip '>='
            getNextUsefulToken();

            return (ast::Expression*)new ast::BinaryGreaterThanOrEqual(
                parseExpression());
        }
        else if (tk == "<=")
        {
            // skip '<='
            getNextUsefulToken();

            return (ast::Expression*)new ast::BinaryLessThanOrEqual(
                parseExpression());
        }
    }
    else if (tk == "==" || tk == "!=")
    {
        if (tk == "==")
        {
            // skip '=='
            getNextUsefulToken();

            return (ast::Expression*)new ast::EquivalenceComparison(
                parseExpression());
        }
        else
        {
            // skip '!='
            getNextUsefulToken();

            return (ast::Expression*)new ast::NonEquivalenceComparison(
                parseExpression());
        }
    }
    else if (tk == "&&")
    {
        // skip '&&'
        getNextUsefulToken();

        return (ast::Expression*)new ast::AndComparison(parseExpression());
    }
    else if (tk == "||")
    {
        // skip '||'
        getNextUsefulToken();

        return (ast::Expression*)new ast::OrComparison(parseExpression());
    }
    else if (tk == "=")
    {
        // skip '=='
        getNextUsefulToken();

        return (ast::Expression*)new ast::Assignment(parseExpression());
    }
    else
    {
        return (ast::Expression*)new ast::PrimaryExpression();
    }
}

/**
 * IndexAccess := '[' NumberLiteral ']'
 */
ast::IndexAccess*
Parser::parseIndexAccess()
{
    return new ast::IndexAccess();
}

/**
 * MemberAccess := '.' Identifier
 */
ast::MemberAccess*
Parser::parseMemberAccess()
{
    return new ast::MemberAccess();
}

/**
 * FunctionCall := '(' [Expression] (',' Expression)*')'
 */
ast::FunctionCall*
Parser::parseFunctionCall()
{
    return new ast::FunctionCall();
}

/**
 * PrimaryExpression := BooleanLiteral |
 *                   DictionaryLiteral |
 *                   ListLiteral |
 *                   NumberLiteral |
 *                   StringLiteral |
 *                   Identifier |
 *                   TypenameExpression
 */
ast::PrimaryExpression*
Parser::parsePrimaryExpression()
{
    return new ast::PrimaryExpression();
}

/**
 * Variable := TypeName Identifier
 */
ast::Variable*
Parser::parseVariable()
{
    ast::Typename* type = new ast::Typename(currentToken->getValue());

    getNextUsefulToken();

    ast::Identifier* id = new ast::Identifier(currentToken->getValue());

    return new ast::Variable(type, id);
}

/**
 * Block := { Statement* }
 */
ast::Block*
Parser::parseBlock()
{
    std::vector<ast::Statement*> statements;

    // skip '{'
    // getNextUsefulToken();

    while (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {

        ast::Statement* s = parseStatement();
        statements.push_back(s);
    }

    return new ast::Block(statements);
}

/**
 * Statement := IfStatement |
 *              LoopStatement |
 *              Block |
 *              'continue' |
 *              'break' |
 *              ReturnStatement |
 *              SimpleStatement
 */
ast::Statement*
Parser::parseStatement()
{
    std::string tk = currentToken->getValue();

    if (tk == "if")
    {
        getNextUsefulToken();
        return (ast::Statement*)parseIfStatement();
    }
    else if (tk == "loop")
    {
        getNextUsefulToken();
        return (ast::Statement*)parseLoopStatement();
    }
    else if (tk == "{")
    {
        getNextUsefulToken();
        return (ast::Statement*)parseBlock();
    }
    else if (tk == "return")
    {
        getNextUsefulToken();
        return (ast::Statement*)parseReturnStatement();
    }
    else if (tk == "continue")
    {
        getNextUsefulToken();
        return (ast::Statement*)new ast::Continue();
    }
    else if (tk == "break")
    {
        getNextUsefulToken();
        return (ast::Statement*)new ast::Break();
    }
    else
    {
        getNextUsefulToken();
        return (ast::Statement*)parseSimpleStatement();
    }
}

/**
 * IfStatement := 'if' Expression '{' Statement '}' 'else' '{' Statement '}'
 */
ast::IfStatement*
Parser::parseIfStatement()
{
    return new ast::IfStatement();
}

/**
 * LoopStatement := 'loop' RangeVariableDefinition { Body } |
 *                  'loop' Expression{ Body }
 */
ast::LoopStatement*
Parser::parseLoopStatement()
{
    // skip 'loop'
    getNextUsefulToken();

    return new ast::LoopStatement();
}

/**
 * SimpleStatement := GeneralDefinition |
 *                    VariableAssignment |
 *                    ExpressionStatement
 */
ast::SimpleStatement*
Parser::parseSimpleStatement()
{
    return new ast::SimpleStatement();
}

/**
 * ReturnStatement := 'return' Expression
 */
ast::ReturnStatement*
Parser::parseReturnStatement()
{
    // skip 'return'
    getNextUsefulToken();

    ast::Expression* expr = parseExpression();

    return new ast::ReturnStatement(expr);
}