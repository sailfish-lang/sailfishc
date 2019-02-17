/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "Visitor.h"
#include <iostream>

void
Visitor::visit(ast::Start* node)
{
    std::cout << "At the start\n";
}
void
Visitor::visit(ast::Source* node)
{
}
void
Visitor::visit(ast::ExportDefinition* node)
{
}
void
Visitor::visit(ast::GeneralDecleration* node)
{
}
void
Visitor::visit(ast::ListDefinition* node)
{
}
void
Visitor::visit(ast::DictionaryDefinition* node)
{
}
void
Visitor::visit(ast::NewVariableDefinition* node)
{
}
void
Visitor::visit(ast::FunctionDefinition* node)
{
}
void
Visitor::visit(ast::Input* node)
{
}
void
Visitor::visit(ast::Output* node)
{
}
void
Visitor::visit(ast::UserDefinedTypeDefinition* node)
{
}
void
Visitor::visit(ast::UserDefinedTypeAttributes* node)
{
}
void
Visitor::visit(ast::UserDefinedTypeMethods* node)
{
}
void
Visitor::visit(ast::InitialExecutionBody* node)
{
}
void
Visitor::visit(ast::RangeVariableDefinition* node)
{
}
void
Visitor::visit(ast::VariableAssignment* node)
{
}
void
Visitor::visit(ast::ShortVariableDefinition* node)
{
}
void
Visitor::visit(ast::Variable* node)
{
}
void
Visitor::visit(ast::IfStatement* node)
{
}
void
Visitor::visit(ast::LoopStatement* node)
{
}
void
Visitor::visit(ast::LoopRange* node)
{
}
void
Visitor::visit(ast::LoopExpression* node)
{
}
void
Visitor::visit(ast::ExpressionStatement* node)
{
}
void
Visitor::visit(ast::ReturnStatement* node)
{
}
void
Visitor::visit(ast::IndexAccess* node)
{
}
void
Visitor::visit(ast::MemberAccess* node)
{
}
void
Visitor::visit(ast::FunctionCall* node)
{
}
void
Visitor::visit(ast::PrimaryExpression* node)
{
}
void
Visitor::visit(ast::BooleanLiteral* node)
{
}
void
Visitor::visit(ast::DictionaryLiteral* node)
{
}
void
Visitor::visit(ast::DictionaryItem* node)
{
}
void
Visitor::visit(ast::ListLiteral* node)
{
}
void
Visitor::visit(ast::ListItem* node)
{
}
void
Visitor::visit(ast::NumberLiteral* node)
{
}
void
Visitor::visit(ast::StringLiteral* node)
{
}
void
Visitor::visit(ast::Identifier* node)
{
}
void
Visitor::visit(ast::TypenameExpression* node)
{
}
void
Visitor::visit(ast::Typename* node)
{
}
void
Visitor::visit(ast::Negation* node)
{
}
void
Visitor::visit(ast::Exponentiation* node)
{
}
void
Visitor::visit(ast::Multiplication* node)
{
}
void
Visitor::visit(ast::Division* node)
{
}
void
Visitor::visit(ast::Modulo* node)
{
}
void
Visitor::visit(ast::Addition* node)
{
}
void
Visitor::visit(ast::Subtraction* node)
{
}
void
Visitor::visit(ast::BinaryGreaterThan* node)
{
}
void
Visitor::visit(ast::BinaryLessThan* node)
{
}
void
Visitor::visit(ast::BinaryGreaterThanOrEqual* node)
{
}
void
Visitor::visit(ast::BinaryLessThanOrEqual* node)
{
}
void
Visitor::visit(ast::EquivalenceComparison* node)
{
}
void
Visitor::visit(ast::NonEquivalenceComparison* node)
{
}
void
Visitor::visit(ast::AndComparison* node)
{
}
void
Visitor::visit(ast::OrComparison* node)
{
}
void
Visitor::visit(ast::Assignment* node)
{
}