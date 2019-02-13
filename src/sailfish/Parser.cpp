/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Parser.h"
#include <iostream>
#include <vector>

Parser::Parser()
{
}

// helper function to get next non comment token
Token*
Parser::getNextToken()
{
    currentToken = lexar->getNextToken();
    while (currentToken->getKind() == COMMENT_TOKEN)
    {
        currentToken = lexar->getNextToken();
    }
}

Node*
Parser::parse(const std::string filename)
{
    // initialize the Lexar
    lexar = new Lexar(filename);

    return parseStart();
}

/**
 *  Start := Source
 */
Node*
Parser::parseStart()
{
    // get source
    std::vector<Node*> source{parseSource()};
    return new Node(Start, "", nullptr, source);
}

/**
 *  Source := SourcePart*
 */
Node*
Parser::parseSource()
{
    // get next token
    getNextToken();
    // get source list
    std::vector<Node*> sourcelist{};
    while (!currentToken->isEOF())
    {
        Node* sourcePart = parseSourcePart();
        sourcelist.push_back(sourcePart);
        getNextToken();
    }
    return new Node(Source, "", nullptr, sourcelist);
}

/**
 * SourcePart := ExportDefinition |
 *               GeneralDecleration |
 *               FunctionDefinition |
 *               UserDefinedTypeDefinition |
 *               InitialExecutionBody
 */
Node*
Parser::parseSourcePart()
{
    std::vector<Node*> empty{};
    std::string tokenValue = currentToken->getValue();
    switch (currentToken->getKind())
    {
    case KEYWORD_TOKEN:
        if (tokenValue == "exp")
        {
            // ExportDefinition

            // get next token
            getNextToken();

            std::vector<Node*> list{parseExportDefinition()};

            return new Node(ExportDefinition, "", nullptr, list);
        }
        else if (tokenValue == "dec")
        {
            // GeneralDecleration

            // get next token
            getNextToken();

            return new Node(GeneralDecleration, "", nullptr, empty);
        }
        else if (tokenValue == "fun")
        {
            // FunctionDefinition

            // get next token
            getNextToken();

            return new Node(FunctionDefinition, "", nullptr, empty);
        }
        else if (tokenValue == "Cat")
        {
            // UserDefinedTypeDefinition

            // get next token
            getNextToken();

            return new Node(UserDefinedTypeAttributes, "", nullptr, empty);
        }
        else if (tokenValue == "Cfn")
        {
            // UserDefinedTypeDefinition

            // get next token
            getNextToken();

            return new Node(UserDefinedTypeMethods, "", nullptr, empty);
        }
        break;
    case START_TOKEN:
        // InitialExecutionBody

        // get next token
        getNextToken();

        return new Node(InitialExecutionBody, "", nullptr, empty);
        break;
    }
}

/**
 *  ExportDefinition := 'exp' Exportable
 *  Exportable := FunctionDefinition | GeneralDecleration
 */
Node*
Parser::parseExportDefinition()
{

    std::vector<Node*> empty{};
    std::string tokenValue = currentToken->getValue();
    switch (currentToken->getKind())
    {
    case KEYWORD_TOKEN:
        if (tokenValue == "dec")
        {
            // GeneralDecleration
            return new Node(GeneralDecleration, "", nullptr, empty);
        }
        else if (tokenValue == "fun")
        {
            // FunctionDefinition
            return new Node(FunctionDefinition, "", nullptr, empty);
        }
    }
}
