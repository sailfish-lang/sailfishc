/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Parser.h"
#include <vector>

Parser::Parser()
{
}

Node*
Parser::parse(const std::string filename)
{
    // initialize the Lexar
    lexar = new Lexar(filename);

    // retrieve the first token
    currentToken = lexar->getNextToken();

    return parseStart();
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
              GeneralDecleration |
              FunctionDefinition |
              UserDefinedTypeDefinition |
              InitialExecutionBody
*/
Node*
Parser::parseSourcePart()
{
    // get next token
    getNextToken();

    std::vector<Node*> empty{};
    std::string tokenValue = currentToken->getValue();
    switch (currentToken->getKind())
    {
    case KEYWORD_TOKEN:
        if (tokenValue == "exp")
        {
            // ExportDefinition
            return new Node(ExportDefinition, "", nullptr, empty);
        }
        else if (tokenValue == "dec")
        {
            // GeneralDecleration
            return new Node(GeneralDecleration, "", nullptr, empty);
        }
        else if (tokenValue == "fun")
        {
            // FunctionDefinition
            return new Node(FunctionDefinition, "", nullptr, empty);
        }
        break;
    case START_TOKEN:
        // InitialExecutionBody
        return new Node(InitialExecutionBody, "", nullptr, empty);
        break;
    default:
        // UserDefinedTypeDefinition
        return new Node(UserDefinedTypeAttributes, "", nullptr, empty);
    }
}
