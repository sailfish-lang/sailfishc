/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Parser.h"
#include "../ast/Ast.h"
#include <iostream>
#include <vector>

// Parser takes in no args, we use a parse function to take in the filename
Parser::Parser()
{
    errorHandler = new ParserErrorHandler();
}

// helper function to get next non comment token
Token*
Parser::getNextUsefulToken()
{
    currentToken = lexar->getNextToken();

    // catch errors from the lexar
    if (currentToken->getKind() == Kind::ERROR_TOKEN)
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(), currentToken->getValue()));
    }

    while (currentToken->getKind() == COMMENT_TOKEN ||
           currentToken->getKind() == COMMA_TOKEN)
    {
        currentToken = lexar->getNextToken();

        // catch errors from the lexar
        if (currentToken->getKind() == Kind::ERROR_TOKEN)
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           currentToken->getValue()));
        }
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
    return new ast::Start(parseSource(), currentToken->getLineNum());
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

    return new ast::Source(srcParts, currentToken->getLineNum());
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
        else
        {
            errorHandler->handle(new Error(
                currentToken->getLineNum(),
                "Expected an export statement (exp), a function (fun), a "
                "decleration (dec), or a UDT (Cat). Those are the only "
                "valid source parts at global source level. Received: " +
                    currentToken->getValue() + "."));
        }
        break;
    case Kind::START_TOKEN:
        // consume 'start'
        getNextUsefulToken();
        return (ast::SourcePart*)parseInitialExecutionBody();
    default:
        errorHandler->handle(
            new Error(currentToken->getLineNum(),
                      "Expected an export statement (exp), a function (fun), a "
                      "decleration (dec), or a UDT (Cat). Those are the only "
                      "valid source parts at global source level. Received: " +
                          currentToken->getValue() + "."));
    }
}

/**
 * ExportDefinition := 'exp' Exportable
 */
ast::ExportDefinition*
Parser::parseExportDefinition()
{
    return new ast::ExportDefinition(parseExportable(),
                                     currentToken->getLineNum());
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
        errorHandler->handle(new Error(
            currentToken->getLineNum(),
            "Expected a function (fun) or a decleration (dec). Received: " +
                currentToken->getValue() + "."));
    }
}

/**
 * FunctionDefinition := FunctionName FunctionInput FunctionOutput FunctionBody
 * FunctionName := 'fun' Identifier
 * FunctionInput := '<-' InputList
 * FunctionOutput := '->' Output
 * FunctionBody := Block
 */
ast::FunctionDefinition*
Parser::parseFunctionDefintion()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // consume function name
    getNextUsefulToken();

    // check for '<-'
    if (currentToken->getKind() != Kind::ARROW_TOKEN)
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(),
                      "Expected a left arrow token. Received: " +
                          currentToken->getValue() + "."));
    }
    // consume '<-'
    getNextUsefulToken();

    // error if one of the inputs is void and there are multiple inputs
    bool hasVoid = false;
    std::vector<ast::Input*> inputs;
    while (currentToken->getKind() != Kind::ARROW_TOKEN)
    {
        ast::Input* input = parseInput();
        inputs.push_back(input);

        if (input->getInput()->getType()->getType() == "void")
            hasVoid = true;

        if (hasVoid && inputs.size() > 1)
        {
            errorHandler->handle(
                new Error(currentToken->getLineNum(),
                          "Cannot have multiple inputs if one is void"));
        }

        // if arrow forgotten, will parse until EOF
        if (currentToken->isEOF())
        {
            errorHandler->handle(
                new Error(currentToken->getLineNum(),
                          "Missing a right arrow token or incorrect format for "
                          "input. Correct: [TYPE] [NAME]."));
        }
    }

    // consume '->'
    getNextUsefulToken();

    // error if one of the outputs is void and there are multiple outputs
    // hasVoid = false;
    // std::vector<ast::Output*> outputs;

    ast::Output* output = parseOutput();

    if (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(), "Expected left curley."));
    }

    ast::Block* body = parseBlock();

    return new ast::FunctionDefinition(name, inputs, output, body,
                                       currentToken->getLineNum());
}

/**
 * Input := Variable
 */
ast::Input*
Parser::parseInput()
{
    return new ast::Input(parseVariable(), currentToken->getLineNum());
}

/**
 * Output := Typename
 */
ast::Output*
Parser::parseOutput()
{
    ast::Typename* type =
        new ast::Typename(currentToken->getValue(), currentToken->getLineNum());

    // consume typename
    getNextUsefulToken();

    return new ast::Output(type, currentToken->getLineNum());
}

/**
 * GeneralDecleration := 'dec' GeneralDefinition
 */
ast::GeneralDecleration*
Parser::parseGeneralDecleration()
{
    return new ast::GeneralDecleration(parseGeneralDefinition(),
                                       currentToken->getLineNum());
}

/**
 * GeneralDefinition := ListDefinition |
 *                      DictionaryDefinition |
 *                      NewUDTDefinition |
 *                      PrimitiveDefinition
 */
ast::GeneralDefinition*
Parser::parseGeneralDefinition()
{
    std::string val = currentToken->getValue();

    // list
    if (val == "list")
    {
        // consume 'list'
        getNextUsefulToken();

        return (ast::GeneralDefinition*)parseListDefinition();
    }

    // dictionary
    else if (val == "dictionary")
    {
        // consume 'dictionary'
        getNextUsefulToken();

        return (ast::GeneralDefinition*)parseDictionaryDefinition();
    }

    // primitives
    else if (val == "bool" || val == "str" || val == "int" || val == "flt" ||
             val == "void")
    {
        return (ast::GeneralDefinition*)parsePrimitiveDefinition();
    }

    // user defined types
    else
    {
        return (ast::GeneralDefinition*)parseNewUDTDefinition();
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

    if (attributes->getName()->getValue() != methods->getName()->getValue())
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(),
                      "Expected UDT method declerations for " +
                          methods->getName()->getValue() +
                          " to follow UDT attribute declerations for " +
                          attributes->getName()->getValue() + "."));
    }

    return new ast::UserDefinedTypeDefinition(attributes, methods,
                                              currentToken->getLineNum());
}

/**
 * UserDefinedTypeAttributes := Identifier '{' Variable* '}'
 */
ast::UserDefinedTypeAttributes*
Parser::UserDefinedTypeAttributes()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // consume identifier
    getNextUsefulToken();

    // check for '{'
    if (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected a left curley. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume '{'
    getNextUsefulToken();

    std::vector<ast::Variable*> attributes;

    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        attributes.push_back(parseVariable());

        // if right curley forgotten, will parse until EOF
        if (currentToken->isEOF())
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           "Missing a right curley."));
        }
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::UserDefinedTypeAttributes(name, attributes,
                                              currentToken->getLineNum());
}

/**
 * UserDefinedTypeMethods := 'Cfn' Identifier '{' FunctionDefinition* '}'
 */
ast::UserDefinedTypeMethods*
Parser::UserDefinedTypeMethods()
{
    // consume 'Cfn'
    getNextUsefulToken();

    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // consume identifier
    getNextUsefulToken();

    // check for '{'
    if (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected a left curley. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume '{'
    getNextUsefulToken();

    std::vector<ast::FunctionDefinition*> methods;
    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        // consume 'fun'
        getNextUsefulToken();

        methods.push_back(parseFunctionDefintion());

        if (currentToken->isEOF())
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           "Missing a right curley."));
        }
    }

    // skip '}'
    getNextUsefulToken();

    return new ast::UserDefinedTypeMethods(name, methods,
                                           currentToken->getLineNum());
}

/**
 * UserDefinedType := identifier '{' [DictionaryItem (',' DictionaryItem)*] '}'
 */
ast::UserDefinedType*
Parser::parseUserDefinedType()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // consume identifier
    getNextUsefulToken();

    // check for '{'
    if (currentToken->getKind() != Kind::LCURLEY_TOKEN)
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected a left curley. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume '{'
    getNextUsefulToken();

    std::vector<ast::DictionaryItem*> attributes;

    while (currentToken->getKind() != Kind::RCURLEY_TOKEN)
    {
        attributes.push_back(parseDictionaryItem());

        if (currentToken->isEOF())
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           "Missing a right curley."));
        }
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::UserDefinedType(name, attributes,
                                    currentToken->getLineNum());
}

/**
 * InitialExecutionBody := 'start' Block
 */
ast::InitialExecutionBody*
Parser::parseInitialExecutionBody()
{
    return new ast::InitialExecutionBody(parseBlock(),
                                         currentToken->getLineNum());
}

/**
 * ListDefinition := 'list' Identifier '<[' Typename ']>' NewExpression
 */
ast::ListDefinition*
Parser::parseListDefinition()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // move to next token
    getNextUsefulToken();

    // check for '<['
    if (currentToken->getKind() != Kind::LFISH_TAIL_TOKEN)
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected a left fish tail. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume fishtail
    getNextUsefulToken();

    ast::Typename* type =
        new ast::Typename(currentToken->getValue(), currentToken->getLineNum());

    // consume typename
    getNextUsefulToken();

    // check for ']>'
    if (currentToken->getKind() != Kind::RFISH_TAIL_TOKEN)
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(),
                      "Expected a right fish tail. Received: " +
                          currentToken->getValue() + "."));
    }
    // consume fishtail
    getNextUsefulToken();

    ast::NewExpression* expr = parseNewExpression();

    return new ast::ListDefinition(name, type, expr,
                                   currentToken->getLineNum());
}

/**
 * DictionaryDefinition := ''dictionary' Identifier '<[' Typename : Typename
 * ']>' NewExpression
 */
ast::DictionaryDefinition*
Parser::parseDictionaryDefinition()
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // consume identifier
    getNextUsefulToken();

    // check for '<['
    if (currentToken->getKind() != Kind::LFISH_TAIL_TOKEN)
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected a left fish tail. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume fishtail
    getNextUsefulToken();

    ast::Typename* keyType =
        new ast::Typename(currentToken->getValue(), currentToken->getLineNum());

    // consume typename
    getNextUsefulToken();

    // check for ':'
    if (currentToken->getKind() != Kind::COLON_TOKEN)
    {
        errorHandler->handle(new Error(
            currentToken->getLineNum(),
            "Expected a colon. Received: " + currentToken->getValue() + "."));
    }
    // consume colon
    getNextUsefulToken();

    ast::Typename* valueType =
        new ast::Typename(currentToken->getValue(), currentToken->getLineNum());

    // consume typename
    getNextUsefulToken();

    // check for ']>'
    if (currentToken->getKind() != Kind::RFISH_TAIL_TOKEN)
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(),
                      "Expected a right fish tail. Received: " +
                          currentToken->getValue() + "."));
    }
    // consume fishtail
    getNextUsefulToken();

    ast::NewExpression* expr = parseNewExpression();

    return new ast::DictionaryDefinition(name, keyType, valueType, expr,
                                         currentToken->getLineNum());
}

/**
 * NewUDTDefinition := Variable '=' NewExpression
 */
ast::NewUDTDefinition*
Parser::parseNewUDTDefinition()
{
    ast::Variable* var = parseVariable();

    // check for '='
    if (currentToken->getValue() != "=")
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected an equals sign. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume '='
    getNextUsefulToken();

    ast::NewExpression* expr = parseNewExpression();

    return new ast::NewUDTDefinition(var, expr, currentToken->getLineNum());
}

/**
 * PrimitiveDefinition := Variable '=' BinaryExpression
 */
ast::PrimitiveDefition*
Parser::parsePrimitiveDefinition()
{
    ast::Variable* var = parseVariable();

    // check for '='
    if (currentToken->getValue() != "=")
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Expected an equals sign. Received: " +
                                           currentToken->getValue() + "."));
    }
    // consume '='
    getNextUsefulToken();

    ast::BinaryExpression* expr = parseBinaryExpression();

    return new ast::PrimitiveDefition(var, expr, currentToken->getLineNum());
}

/**
 * Expression := NewExpression |
 *               GroupingExpression |
 *               UnaryExpression |
 *               PrimaryExpression
 */
ast::Expression*
Parser::parseExpression()
{
    std::string tk = currentToken->getValue();

    if (tk == "new")
    {
        return (ast::Expression*)parseNewExpression();
    }
    else if (tk == "|")
    {
        return (ast::Expression*)parseGroupingExpression();
    }
    else if (tk == "!")
    {
        // only one case of unary right now
        return (ast::Expression*)parseUnaryExpression();
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

    return new ast::NewExpression(newVal, currentToken->getLineNum());
}

/**
 * NewExpression := ListLiteral | DictionaryLiteral | UserDefinedType
 */
ast::New*
Parser::parseNew()
{
    // consume 'new'
    getNextUsefulToken();

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
        return (ast::New*)parseUserDefinedType();
    }
}

/**
 * GroupingExpression := '|' BinaryExpression '|'
 */
ast::GroupingExpression*
Parser::parseGroupingExpression()
{
    // consume '|'
    getNextUsefulToken();

    if (currentToken->getKind() == Kind::PIPE_TOKEN)
    {
        errorHandler->handle(new Error(currentToken->getLineNum(),
                                       "Cannot have empty if condition."));
    }

    ast::BinaryExpression* expr = parseBinaryExpression();

    if (currentToken->getKind() != Kind::PIPE_TOKEN)
    {
        errorHandler->handle(
            new Error(currentToken->getLineNum(), "Missing a pipe."));
    }

    // consume '|'
    getNextUsefulToken();

    return new ast::GroupingExpression(expr, currentToken->getLineNum());
}

/**
 * AttributeAccess := Identifier '.' Identifier
 */
ast::AttributeAccess*
Parser::parseAttributeAccess(ast::Identifier* udtName)
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // consume identifier
    getNextUsefulToken();

    return new ast::AttributeAccess(name, udtName, currentToken->getLineNum());
}

/**
 * MethodAccess := Identifier '...' Identifier FunctionCall
 */
ast::MethodAccess*
Parser::parseMethodAccess(ast::Identifier* udtName)
{
    ast::Identifier* name = new ast::Identifier(currentToken->getValue(),
                                                currentToken->getLineNum());

    // DONT consume identifier
    // getNextUsefulToken();

    ast::BinaryExpression* es = parseBinaryExpression();

    return new ast::MethodAccess(name, es, udtName, currentToken->getLineNum());
}
/**
 * UnaryExpression := Negation
 */
ast::UnaryExpression*
Parser::parseUnaryExpression()
{
    std::string tk = currentToken->getValue();
    if (tk == "!")
    {
        // consume '!'
        getNextUsefulToken();

        return (ast::UnaryExpression*)new ast::Negation(
            parseBinaryExpression(), currentToken->getLineNum());
    }
}

/**
 * BinaryExpression := Exponentiation |
 *                     MultDivMod |
 *                     Arithmetic |
 *                     Comparison |
 *                     Equivalence |
 *                     LogicalComparison |
 *                     Assignment |
 *                     ExpressionOnlyStatement
 */
ast::BinaryExpression*
Parser::parseBinaryExpression()
{
    ast::Expression* leftExpr = parseExpression();

    std::string tk = currentToken->getValue();
    int lineNum = currentToken->getLineNum();

    // if at the end of the file, just return a PrimaryExpression
    if (currentToken->isEOF())
    {
        return (ast::BinaryExpression*)new ast::ExpressionOnlyStatement(
            leftExpr, lineNum);
    }

    // Expression ** Expression
    if (tk == "**")
    {
        // consume '**'
        getNextUsefulToken();

        ast::BinaryExpression* rightExpr = parseBinaryExpression();

        return (ast::BinaryExpression*)new ast::Exponentiation(
            leftExpr, rightExpr, lineNum);
    }

    // Expression ['*' | '/' | '%'] Expression
    if (tk == "*" || tk == "/" || tk == "%")
    {
        if (tk == "*")
        {
            // consume '*'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::Multiplication(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "/")
        {
            // consume '/'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::Division(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "%")
        {
            // consume '%'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::Modulo(leftExpr, rightExpr,
                                                           lineNum);
        }
    }

    // Expression ['+' | '-'] Expression
    if (tk == "+" || tk == "-")
    {
        if (tk == "+")
        {
            // consume '+'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::Addition(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "-")
        {
            // consume '-'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::Subtraction(
                leftExpr, rightExpr, lineNum);
        }
    }

    // Expression ['>', '>=', '<', '<='] Expression
    if (tk == ">" || tk == ">=" || tk == "<" || tk == "<=")
    {
        if (tk == ">")
        {
            // consume '>'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::BinaryGreaterThan(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == ">=")
        {
            // consume '>='
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::BinaryGreaterThanOrEqual(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "<")
        {
            // consume '<'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::BinaryLessThan(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "<=")
        {
            // consume '<='
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::BinaryLessThanOrEqual(
                leftExpr, rightExpr, lineNum);
        }
    }

    // Expression ['==' | '!='] Expression
    if (tk == "==" || tk == "!=")
    {
        if (tk == "==")
        {
            // consume '=='
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::EquivalenceComparison(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "!=")
        {
            // consume '!='
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::NonEquivalenceComparison(
                leftExpr, rightExpr, lineNum);
        }
    }

    // Expression ['and' | 'or'] Expression
    if (tk == "and" || tk == "or")
    {
        if (tk == "and")
        {
            // consume 'and'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::AndComparison(
                leftExpr, rightExpr, lineNum);
        }

        if (tk == "or")
        {
            // consume 'or'
            getNextUsefulToken();

            ast::BinaryExpression* rightExpr = parseBinaryExpression();

            return (ast::BinaryExpression*)new ast::OrComparison(
                leftExpr, rightExpr, lineNum);
        }
    }

    // Expression '=' Expression
    if (tk == "=")
    {
        // consume '='
        getNextUsefulToken();

        ast::BinaryExpression* rightExpr = parseBinaryExpression();

        return (ast::BinaryExpression*)new ast::Assignment(leftExpr, rightExpr,
                                                           lineNum);
    }

    // PrimaryExpression
    return (ast::BinaryExpression*)new ast::ExpressionOnlyStatement(leftExpr,
                                                                    lineNum);
}

/**
 * PrimaryExpression := Primary
 */
ast::PrimaryExpression*
Parser::parsePrimaryExpression()
{
    return new ast::PrimaryExpression(parsePrimary(),
                                      currentToken->getLineNum());
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

    if (kind == Kind::BOOL_TOKEN)
    {
        // consume bool
        getNextUsefulToken();

        return (ast::Primary*)new ast::BooleanLiteral(
            tk, currentToken->getLineNum());
    }
    else if (kind == Kind::INTEGER_TOKEN)
    {
        // consume integer
        getNextUsefulToken();

        return (ast::Primary*)new ast::IntegerLiteral(
            tk, currentToken->getLineNum());
    }
    else if (kind == Kind::FLOAT_TOKEN)
    {
        // consume float
        getNextUsefulToken();

        return (ast::Primary*)new ast::FloatLiteral(tk,
                                                    currentToken->getLineNum());
    }
    else if (kind == Kind::STRING_TOKEN)
    {
        // consume string
        getNextUsefulToken();

        return (ast::Primary*)new ast::StringLiteral(
            tk, currentToken->getLineNum());
    }
    else if (kind == Kind::LCURLEY_TOKEN)
    {
        // consume '{'
        getNextUsefulToken();

        return (ast::Primary*)parseDictionaryLiteral();
    }
    else if (kind == Kind::LBRACKET_TOKEN)
    {
        // consume '['
        getNextUsefulToken();

        return (ast::Primary*)parseListLiteral();
    }
    else if (kind == Kind::IDENTIFIER_TOKEN)
    {
        ast::Identifier* ident =
            new ast::Identifier(tk, currentToken->getLineNum());

        // consume identifier
        getNextUsefulToken();

        if (currentToken->getKind() == Kind::DOT_TOKEN)
        {
            // consume '.'
            getNextUsefulToken();

            return (ast::Primary*)parseAttributeAccess(ident);
        }
        else if (currentToken->getKind() == Kind::TRIPLE_DOT_TOKEN)
        {
            // consume '...'
            getNextUsefulToken();

            return (ast::Primary*)parseMethodAccess(ident);
        }
        else if (currentToken->getKind() == Kind::LPAREN_TOKEN)
        {
            // consume '('
            getNextUsefulToken();

            std::vector<ast::Primary*> idents;

            while (currentToken->getKind() != Kind::RPAREN_TOKEN)
            {
                idents.push_back(parsePrimary());

                if (currentToken->isEOF())
                {
                    errorHandler->handle(
                        new Error(currentToken->getLineNum(),
                                  "Missing a right parenthesis."));
                }
            }

            // consume ')'
            getNextUsefulToken();

            return (ast::Primary*)new ast::FunctionCall(
                ident, idents, currentToken->getLineNum());
        }
        else
        {
            return (ast::Primary*)ident;
        }
    }
    else
    {
        errorHandler->handle(new Error(
            currentToken->getLineNum(),
            "Expected a literal of type bool, int, flt, str, list, dictionary, "
            "or a UDT. Instead received: " +
                tk + "."));
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

        if (currentToken->isEOF())
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           "Missing a right curley."));
        }
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::DictionaryLiteral(dictionaryItems,
                                      currentToken->getLineNum());
}

/**
 * DictionaryItem := Identifier ':' Identifier
 */
ast::DictionaryItem*
Parser::parseDictionaryItem()
{
    ast::Primary* key = parsePrimary();

    // check for ':'
    if (currentToken->getKind() != Kind::COLON_TOKEN)
    {
        errorHandler->handle(new Error(
            currentToken->getLineNum(),
            "Expected a colon. Received: " + currentToken->getValue() + "."));
    }
    // consume ':'
    getNextUsefulToken();

    ast::Primary* value = parsePrimary();

    return new ast::DictionaryItem(key, value, currentToken->getLineNum());
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

        if (currentToken->isEOF())
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           "Missing a right bracket."));
        }
    }

    // consume ']'
    getNextUsefulToken();

    return new ast::ListLiteral(listItems, currentToken->getLineNum());
}

/**
 * ListItem := Identifier
 */
ast::ListItem*
Parser::parseListItem()
{
    ast::Primary* value = parsePrimary();

    return new ast::ListItem(value, currentToken->getLineNum());
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
        ast::Typename* type =
            new ast::Typename("void", currentToken->getLineNum());
        ast::Identifier* id =
            new ast::Identifier("void", currentToken->getLineNum());

        // consume void
        getNextUsefulToken();

        return new ast::Variable(type, id, currentToken->getLineNum());
    }

    ast::Typename* type =
        new ast::Typename(currentToken->getValue(), currentToken->getLineNum());

    // consume typename
    getNextUsefulToken();

    ast::Identifier* id = new ast::Identifier(currentToken->getValue(),
                                              currentToken->getLineNum());

    // consume identifier
    getNextUsefulToken();

    return new ast::Variable(type, id, currentToken->getLineNum());
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

        if (currentToken->isEOF())
        {
            errorHandler->handle(new Error(currentToken->getLineNum(),
                                           "Missing a right curley."));
        }
    }

    // consume '}'
    getNextUsefulToken();

    return new ast::Block(statements, currentToken->getLineNum());
}

/**
 * Statement := IfStatement |
 *              Block |
 *              ReturnStatement |
 *              GeneralDecleration |
 *              BinaryExpression
 */
ast::Statement*
Parser::parseStatement()
{
    std::string tk = currentToken->getValue();
    Kind kind = currentToken->getKind();

    switch (kind)
    {

    case Kind::KEYWORD_TOKEN:
    {
        if (tk == "if")
        {
            // consume 'if'
            getNextUsefulToken();

            return (ast::Statement*)parseIfStatement();
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
            errorHandler->handle(
                new Error(currentToken->getLineNum(),
                          "Expected if, return, or dec. Received: " +
                              currentToken->getValue() + "."));
        }
    }

    case Kind::LCURLEY_TOKEN:
        return (ast::Statement*)parseBlock();

    default:
        return (ast::Statement*)parseBinaryExpression();
    }
}

/**
 * IfStatement := 'if' GroupingExpression Block 'else' Block
 */
ast::IfStatement*
Parser::parseIfStatement()
{
    ast::GroupingExpression* ifExpr = parseGroupingExpression();

    ast::Block* ifStatements = parseBlock();

    if (currentToken->getValue() != "else")
    {
        errorHandler->handle(new Error(
            currentToken->getLineNum(),
            "Expected else. Received: " + currentToken->getValue() + "."));
    }
    // skip 'else'
    getNextUsefulToken();

    ast::Block* elseStatements = parseBlock();

    return new ast::IfStatement(ifExpr, ifStatements, elseStatements,
                                currentToken->getLineNum());
}

/**
 * ReturnStatement := 'return' BinaryExpression
 */
ast::ReturnStatement*
Parser::parseReturnStatement()
{
    return new ast::ReturnStatement(parseBinaryExpression(),
                                    currentToken->getLineNum());
}
