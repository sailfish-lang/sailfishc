/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"

class Visitor
{
  public:
    virtual void visit(ast::Start*);
    virtual void visit(ast::Source*);
    virtual void visit(ast::ExportDefinition*);
    virtual void visit(ast::GeneralDecleration*);
    virtual void visit(ast::GeneralDefinition*);
    virtual void visit(ast::ListDefinition*);
    virtual void visit(ast::DictionaryDefinition*);
    virtual void visit(ast::NewUDTDefinition*);
    virtual void visit(ast::PrimitiveDefition*);
    virtual void visit(ast::FunctionDefinition*);
    virtual void visit(ast::Input*);
    virtual void visit(ast::Output*);
    virtual void visit(ast::UserDefinedTypeDefinition*);
    virtual void visit(ast::UserDefinedTypeAttributes*);
    virtual void visit(ast::UserDefinedTypeMethods*);
    virtual void visit(ast::UserDefinedType*);
    virtual void visit(ast::InitialExecutionBody*);
    virtual void visit(ast::Variable*);
    virtual void visit(ast::Block*);
    virtual void visit(ast::IfStatement*);
    virtual void visit(ast::ExpressionStatement*);
    virtual void visit(ast::ReturnStatement*);
    virtual void visit(ast::NewExpression*);
    virtual void visit(ast::ArrayExpression*);
    virtual void visit(ast::MemberAccess*);
    virtual void visit(ast::AttributeAccess*);
    virtual void visit(ast::MethodAccess*);
    virtual void visit(ast::FunctionCall*);
    virtual void visit(ast::UnaryExpression*);
    virtual void visit(ast::PrimaryExpression*);
    virtual void visit(ast::BooleanLiteral*);
    virtual void visit(ast::DictionaryLiteral*);
    virtual void visit(ast::DictionaryItem*);
    virtual void visit(ast::ListLiteral*);
    virtual void visit(ast::ListItem*);
    virtual void visit(ast::IntegerLiteral*);
    virtual void visit(ast::FloatLiteral*);
    virtual void visit(ast::ByteLiteral*);
    virtual void visit(ast::StringLiteral*);
    virtual void visit(ast::Identifier*);
    virtual void visit(ast::Expression*);
    virtual void visit(ast::Typename*);
    virtual void visit(ast::Negation*);
    virtual void visit(ast::Exponentiation*);
    virtual void visit(ast::Multiplication*);
    virtual void visit(ast::Division*);
    virtual void visit(ast::Modulo*);
    virtual void visit(ast::Addition*);
    virtual void visit(ast::Subtraction*);
    virtual void visit(ast::GroupingExpression*);
    virtual void visit(ast::BinaryExpression*);
    virtual void visit(ast::ExpressionOnlyStatement*);
    virtual void visit(ast::BinaryGreaterThan*);
    virtual void visit(ast::BinaryLessThan*);
    virtual void visit(ast::BinaryGreaterThanOrEqual*);
    virtual void visit(ast::BinaryLessThanOrEqual*);
    virtual void visit(ast::EquivalenceComparison*);
    virtual void visit(ast::NonEquivalenceComparison*);
    virtual void visit(ast::AndComparison*);
    virtual void visit(ast::OrComparison*);
    virtual void visit(ast::Assignment*);
};