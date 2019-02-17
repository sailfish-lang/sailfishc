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
    while (currentToken->getKind() == COMMENT_TOKEN)
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
    return new ast::Start();
}

/**
 * Source := SourcePart*
 */
ast::Source*
parseSource()
{
}

/**
 * SourcePart := ExportDefinition |
 *               GeneralDecleration |
 *               FuctionDefinition |
 *               UserDefinedTypeDefinition |
 *               InitialExecutionBody
 */
ast::SourcePart*
parseSourcePart()
{
}

/**
 * ExportDefinition := 'exp' Exportable
 */
ast::ExportDefinition*
parseExportDefinition()
{
}

/**
 * Exportable := FunctionDefinition | GeneralDecleration
 */
ast::Exportable*
parseExportable()
{
}

/**
 * FunctionDefinition := FunctionName FunctionInput FunctionOutput FunctionBody
 * FunctionName := 'fun' Identifier
 * FunctionInput := '<-' InputList
 * FunctionOutput := '->' OutputList
 * FunctionBody := '{' Block '}'
 */
ast::FunctionDefinition*
parseFunctionDefinition()
{
}
