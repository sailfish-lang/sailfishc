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
    virtual void visit(ast::NewUDTDefinition*);
    virtual void visit(ast::PrimitiveDefition*);
    virtual void visit(ast::FunctionDefinition*);
    virtual void visit(ast::Input*);
    virtual void visit(ast::Output*);
    virtual void visit(ast::UserDefinedTypeDefinition*);
    virtual void visit(ast::UserDefinedType*);
    virtual void visit(ast::UDTitem*);
    virtual void visit(ast::InitialExecutionBody*);
    virtual void visit(ast::Variable*);
    virtual void visit(ast::Block*);
    virtual void visit(ast::IfStatement*);
    virtual void visit(ast::ReturnStatement*);
    virtual void visit(ast::Statement*);
    virtual void visit(ast::NewExpression*);
    virtual void visit(ast::AttributeAccess*);
    virtual void visit(ast::MethodAccess*);
    virtual void visit(ast::FunctionCall*);
    virtual void visit(ast::UnaryExpression*);
    virtual void visit(ast::PrimaryExpression*);
    virtual void visit(ast::Primary*);
    virtual void visit(ast::BooleanLiteral*);
    virtual void visit(ast::IntegerLiteral*);
    virtual void visit(ast::FloatLiteral*);
    virtual void visit(ast::StringLiteral*);
    virtual void visit(ast::Identifier*);
    virtual void visit(ast::Expression*);
    virtual void visit(ast::Typename*);
    virtual void visit(ast::Negation*);
    virtual void visit(ast::GroupingExpression*);
    virtual void visit(ast::BinaryExpression*);
    virtual void visit(ast::BinaryCompOrArith*);
    virtual void visit(ast::ExpressionOnlyStatement*);
    virtual void visit(ast::Assignment*);
};