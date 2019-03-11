/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../errorhandler/Error.h"
#include "../errorhandler/ErrorHandler.h"
#include "../errorhandler/ParserErrorHandler.h"
#include "../lexar/Lexar.h"
#include <string>

class Parser
{
  private:
    Lexar* lexar;
    Token* currentToken; // LL(1)
    ErrorHandler* errorHandler;

    // helper function for ignoring extraneous tokens
    Token* getNextUsefulToken();

    // parse methods
    ast::Start* parseStart();
    ast::Source* parseSource();
    ast::SourcePart* parseSourcePart();

    ast::ExportDefinition* parseExportDefinition();
    ast::FunctionDefinition* parseFunctionDefintion();
    ast::GeneralDecleration* parseGeneralDecleration();
    ast::UserDefinedTypeDefinition* parseUserDefinedTypeDefinition();
    ast::InitialExecutionBody* parseInitialExecutionBody();

    ast::Exportable* parseExportable();

    ast::Input* parseInput();
    ast::Output* parseOutput();

    ast::GeneralDefinition* parseGeneralDefinition();

    ast::UserDefinedTypeAttributes* UserDefinedTypeAttributes();
    ast::UserDefinedTypeMethods* UserDefinedTypeMethods();
    ast::UserDefinedType* parseUserDefinedType();

    ast::DictionaryDefinition* parseDictionaryDefinition();
    ast::ListDefinition* parseListDefinition();
    ast::NewUDTDefinition* parseNewUDTDefinition();
    ast::PrimitiveDefition* parsePrimitiveDefinition();

    ast::Expression* parseExpression();

    ast::NewExpression* parseNewExpression();
    ast::New* parseNew();

    ast::ArrayExpression* parseArrayExpression();

    ast::GroupingExpression* parseGroupingExpression();

    ast::AttributeAccess* parseAttributeAccess(ast::Identifier*);
    ast::MethodAccess* parseMethodAccess(ast::Identifier*);

    ast::UnaryExpression* parseUnaryExpression();
    ast::BinaryExpression* parseBinaryExpression();

    ast::PrimaryExpression* parsePrimaryExpression();
    ast::Primary* parsePrimary();

    ast::DictionaryLiteral* parseDictionaryLiteral();
    ast::DictionaryItem* parseDictionaryItem();

    ast::ListLiteral* parseListLiteral();
    ast::ListItem* parseListItem();

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
};
