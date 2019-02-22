/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Node.h"

class Visitor
{
  public:
    void visit(ast::Start*);
    void visit(ast::Source*);
    void visit(ast::ExportDefinition*);
    void visit(ast::GeneralDecleration*);
    void visit(ast::GeneralDefinition*);
    void visit(ast::ListDefinition*);
    void visit(ast::DictionaryDefinition*);
    void visit(ast::NewVariableDefinition*);
    void visit(ast::FunctionDefinition*);
    void visit(ast::Input*);
    void visit(ast::Output*);
    void visit(ast::UserDefinedTypeDefinition*);
    void visit(ast::UserDefinedTypeAttributes*);
    void visit(ast::UserDefinedTypeMethods*);
    void visit(ast::InitialExecutionBody*);
    void visit(ast::VariableAssignment*);
    void visit(ast::ShortVariableDefinition*);
    void visit(ast::Variable*);
    void visit(ast::Block*);
    void visit(ast::IfStatement*);
    void visit(ast::ExpressionStatement*);
    void visit(ast::ReturnStatement*);
    void visit(ast::SimpleStatement*);
    void visit(ast::NewExpression*);
    void visit(ast::IndexAccess*);
    void visit(ast::MemberAccess*);
    void visit(ast::FunctionCall*);
    void visit(ast::PrimaryExpression*);
    void visit(ast::BooleanLiteral*);
    void visit(ast::DictionaryLiteral*);
    void visit(ast::DictionaryItem*);
    void visit(ast::ListLiteral*);
    void visit(ast::ListItem*);
    void visit(ast::IntegerLiteral*);
    void visit(ast::FloatLiteral*);
    void visit(ast::ExpressionStatement);
    void visit(ast::StringLiteral*);
    void visit(ast::Identifier*);
    void visit(ast::Expression*);
    void visit(ast::Typename*);
    void visit(ast::Negation*);
    void visit(ast::Exponentiation*);
    void visit(ast::Multiplication*);
    void visit(ast::Division*);
    void visit(ast::Modulo*);
    void visit(ast::Addition*);
    void visit(ast::Subtraction*);
    void visit(ast::BinaryGreaterThan*);
    void visit(ast::BinaryLessThan*);
    void visit(ast::BinaryGreaterThanOrEqual*);
    void visit(ast::BinaryLessThanOrEqual*);
    void visit(ast::EquivalenceComparison*);
    void visit(ast::NonEquivalenceComparison*);
    void visit(ast::AndComparison*);
    void visit(ast::OrComparison*);
    void visit(ast::Break*);
    void visit(ast::Continue*);
    void visit(ast::Assignment*);
};