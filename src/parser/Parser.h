/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Node.h"
#include "../lexar/Lexar.h"
#include "../lexar/Token.h"
#include <string>
#include <vector>

class Parser
{
  private:
    Lexar* lexar;
    Token* currentToken; // LL(1)

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

    ast::DictionaryDefinition* parseDictionaryDefinition();
    ast::ListDefinition* parseListDefinition();
    ast::NewVariableDefinition* parseNewVariableDefinition();

    ast::Expression* parseExpression();

    ast::NewExpression* parseNewExpression();
    ast::New* parseNew();

    ast::ArrayExpression* parseArrayExpression();

    ast::BinaryExpression* parseBinaryExpression();

    ast::MemberAccess* parseMemberAccess();
    ast::AttributeAccess* parseAttributeAccess();
    ast::MethodAccess* parseMethodAccess();

    ast::FunctionCall* parseFunctionCall();

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
    ast::ExpressionStatement* parseExpressionStatement();

    ast::Variable* parseVariable();

    ast::Identifier* parseIdentifier();
    ast::Typename* parseTypename();

  public:
    // constructor
    Parser();
    // actual parse function -- given filename returns the root of the ast
    ast::Start* parse(const std::string);
};
