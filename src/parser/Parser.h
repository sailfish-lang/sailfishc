/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../errorhandler/Error.h"
#include "../errorhandler/ParserErrorHandler.h"
#include "../lexar/Lexar.h"
#include <iostream>
#include <string>
#include <vector>

class Parser
{
  private:
    Lexar* lexar;
    Token* currentToken; // LL(1)
    ParserErrorHandler* errorHandler;

    // helper function for ignoring extraneous tokens
    Token* getNextUsefulToken();

    // parse methods
    ast::Start* parseStart();
    ast::Source* parseSource();
    ast::SourcePart* parseSourcePart();

    ast::FunctionDefinition* parseFunctionDefintion();
    ast::GeneralDecleration* parseGeneralDecleration();
    ast::UserDefinedTypeDefinition* parseUserDefinedTypeDefinition();
    ast::InitialExecutionBody* parseInitialExecutionBody();

    ast::Input* parseInput();
    ast::Output* parseOutput();

    ast::GeneralDefinition* parseGeneralDefinition();

    ast::UserDefinedType* parseUserDefinedType();
    ast::UDTitem* parseUDTitem();

    ast::NewUDTDefinition* parseNewUDTDefinition();
    ast::PrimitiveDefition* parsePrimitiveDefinition();

    ast::Expression* parseExpression();

    ast::NewExpression* parseNewExpression();
    ast::New* parseNew();

    ast::GroupingExpression* parseGroupingExpression();

    ast::AttributeAccess* parseAttributeAccess(ast::Identifier*);
    ast::MethodAccess* parseMethodAccess(ast::Identifier*);

    ast::UnaryExpression* parseUnaryExpression();
    ast::BinaryExpression* parseBinaryExpression();

    ast::PrimaryExpression* parsePrimaryExpression();
    ast::Primary* parsePrimary();

    ast::Block* parseBlock();

    ast::Statement* parseStatement();
    ast::IfStatement* parseIfStatement();
    ast::ReturnStatement* parseReturnStatement();

    ast::Variable* parseVariable();

    ast::Identifier* parseIdentifier();
    ast::Typename* parseTypename();

  public:
    // constructor
    Parser();
    // destructor
    ~Parser()
    {
        delete lexar;
        delete currentToken;
    };
    // actual parse function -- given filename returns the root of the ast
    ast::Start* parse(const std::string);
    void
    checkErrors()
    {
        errorHandler->end();
    }
};
