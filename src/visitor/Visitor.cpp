/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Visitor.h"
#include <vector>

void
Visitor::visit(ast::Start* node)
{
    visit(node->getSource());
}
void
Visitor::visit(ast::Source* node)
{
    for (auto const& srcPart : node->getSourceParts())
    {
        auto type = srcPart->getSourcePartType();
        switch (type)
        {
        case ast::SourcePart::GeneralDecleration:
        {
            auto subnode = dynamic_cast<ast::GeneralDecleration*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::FunctionDefinitionPart:
        {
            auto subnode = dynamic_cast<ast::FunctionDefinition*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::UserDefinedTypeDefinition:
        {
            auto subnode =
                dynamic_cast<ast::UserDefinedTypeDefinition*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::InitialExecutionBody:
        {
            auto subnode = dynamic_cast<ast::InitialExecutionBody*>(srcPart);
            visit(subnode);
            break;
        }
        }
    }
}

void
Visitor::visit(ast::GeneralDecleration* node)
{
    visit(node->getDefinition());
}
void
Visitor::visit(ast::GeneralDefinition* node)
{
    switch (node->getGeneralDefinitionType())
    {
    case ast::GeneralDefinition::NewUDTDefinition:
    {
        auto subnode = dynamic_cast<ast::NewUDTDefinition*>(node);
        visit(subnode);
        break;
    }
    case ast::GeneralDefinition::PrimitiveDefition:
    {
        auto subnode = dynamic_cast<ast::PrimitiveDefition*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::NewUDTDefinition* node)
{
    visit(node->getVariable());
    visit(node->getExpression());
}
void
Visitor::visit(ast::PrimitiveDefition* node)
{
    visit(node->getVariable());
    visit(node->getBinaryExpression());
}
void
Visitor::visit(ast::BinaryExpression* node)
{
    switch (node->getBinaryExpressionType())
    {
    case ast::BinaryExpression::BinaryCompOrArith:
    {
        auto subnode = dynamic_cast<ast::BinaryCompOrArith*>(node);
        visit(subnode);
        break;
    }

    case ast::BinaryExpression::Assignment:
    {
        auto subnode = dynamic_cast<ast::Assignment*>(node);
        visit(subnode);
        break;
    }
    case ast::BinaryExpression::ExpressionOnlyStatement:
    {
        auto subnode = dynamic_cast<ast::ExpressionOnlyStatement*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::ExpressionOnlyStatement* node)
{
    visit(node->getLeftExpr());
}
void
Visitor::visit(ast::UnaryExpression* node)
{
    switch (node->getUnaryExpressionype())
    {
    case ast::UnaryExpression::Negation:
    {
        auto subnode = dynamic_cast<ast::Negation*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::Expression* node)
{
    switch (node->getExpressionType())
    {
    case ast::Expression::NewExpression:
    {
        auto subnode = dynamic_cast<ast::NewExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::GroupingExpression:
    {
        auto subnode = dynamic_cast<ast::GroupingExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::PrimaryExpression:
    {
        auto subnode = dynamic_cast<ast::PrimaryExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::UnaryExpression:
    {
        auto subnode = dynamic_cast<ast::UnaryExpression*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::Statement* node)
{
    switch (node->getStatementType())
    {
    case ast::Statement::BlockStatement:
    {
        auto subnode = dynamic_cast<ast::Block*>(node);
        visit(subnode);
        break;
    }
    case ast::Statement::BinaryExpressionStatement:
    {
        auto subnode = dynamic_cast<ast::BinaryExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Statement::IfStatement:
    {
        auto subnode = dynamic_cast<ast::IfStatement*>(node);
        visit(subnode);
        break;
    }
    case ast::Statement::ReturnStatement:
    {
        auto subnode = dynamic_cast<ast::ReturnStatement*>(node);
        visit(subnode);
        break;
    }
    case ast::Statement::GeneralDecleration:
    {
        auto subnode = dynamic_cast<ast::GeneralDecleration*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::FunctionDefinition* node)
{
    visit(node->getName());

    for (auto const& input : node->getInputList())
        visit(input);

    visit(node->getOutput());
    visit(node->getBody());
}
void
Visitor::visit(ast::Input* node)
{
    visit(node->getInput());
}
void
Visitor::visit(ast::Output* node)
{
    visit(node->getOutput());
}
void
Visitor::visit(ast::UserDefinedTypeDefinition* node)
{
    visit(node->getName());

    for (auto const& attribute : node->getAttributes())
        visit(attribute);

    for (auto const& method : node->getMethods())
        visit(method);
}
void
Visitor::visit(ast::InitialExecutionBody* node)
{
    visit(node->getBody());
}
void
Visitor::visit(ast::Variable* node)
{
    visit(node->getName());
    visit(node->getType());
}
void
Visitor::visit(ast::Block* node)
{
    for (auto const& statement : node->getStatements())
    {
        switch (statement->getStatementType())
        {
        case ast::Statement::IfStatement:
        {
            auto subnode = dynamic_cast<ast::IfStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::ReturnStatement:
        {
            auto subnode = dynamic_cast<ast::ReturnStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BlockStatement:
        {
            auto subnode = dynamic_cast<ast::Block*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::GeneralDecleration:
        {
            auto subnode = dynamic_cast<ast::GeneralDecleration*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BinaryExpressionStatement:
        {
            auto subnode = dynamic_cast<ast::BinaryExpression*>(statement);
            visit(subnode);
            break;
        }
        }
    }
}
void
Visitor::visit(ast::IfStatement* node)
{
    visit(node->getIfConditional());
    visit(node->getIfStatements());
    visit(node->getElseStatements());
}

void
Visitor::visit(ast::ReturnStatement* node)
{
    visit(node->getBinaryExpression());
}
void
Visitor::visit(ast::NewExpression* node)
{
    auto newVal = node->getNewVal();
    auto type = newVal->getNewType();

    switch (type)
    {
    case ast::New::UserDefinedType:
    {
        auto subnode = dynamic_cast<ast::UserDefinedType*>(newVal);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::AttributeAccess* node)
{
    visit(node->getUDT());
    visit(node->getAttribute());
}
void
Visitor::visit(ast::AttributeMethodAccess* node)
{
    visit(node->getAttribute());
    visit(node->getName());
    visit(node->getFunctionCall());
}
void
Visitor::visit(ast::MethodAccess* node)
{

    visit(node->getName());
    visit(node->getFunctionCall());
}
void
Visitor::visit(ast::FunctionCall* node)
{
    auto name = node->getName();
    auto args = node->getArguments();

    visit(name);

    for (auto const& arg : args)
        visit(arg);
}
void
Visitor::visit(ast::UserDefinedType* node)
{
    visit(node->getName());
    for (auto const& attribute : node->getAttributes())
        visit(attribute);
}
void
Visitor::visit(ast::UDTitem* node)
{
    visit(node->getKey());
    visit(node->getValue());
}
void
Visitor::visit(ast::PrimaryExpression* node)
{
    visit(node->getPrimary());
}
void
Visitor::visit(ast::Primary* node)
{
    ast::Primary::PrimaryType type = node->getPrimaryType();
    switch (type)
    {
    case ast::Primary::IdentifierLiteral:
    {
        auto subnode = dynamic_cast<ast::Identifier*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::StringLiteral:
    {
        auto subnode = dynamic_cast<ast::StringLiteral*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::BooleanLiteral:
    {
        auto subnode = dynamic_cast<ast::BooleanLiteral*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::IntegerLiteral:
    {
        auto subnode = dynamic_cast<ast::IntegerLiteral*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::FloatLiteral:
    {
        auto subnode = dynamic_cast<ast::FloatLiteral*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::AttributeAccessLiteral:
    {
        auto subnode = dynamic_cast<ast::AttributeAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::AttributeMethodAccessLiteral:
    {
        auto subnode = dynamic_cast<ast::AttributeMethodAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::MethodAccessLiteral:
    {
        auto subnode = dynamic_cast<ast::MethodAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::Primary::FunctionCallLiteral:
    {
        auto subnode = dynamic_cast<ast::FunctionCall*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::BooleanLiteral* node)
{
}
void
Visitor::visit(ast::IntegerLiteral* node)
{
}
void
Visitor::visit(ast::FloatLiteral* node)
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
Visitor::visit(ast::Typename* node)
{
}
void
Visitor::visit(ast::Negation* node)
{
    visit(node->getBinaryExpression());
}
void
Visitor::visit(ast::GroupingExpression* node)
{
    visit(node->getBinaryExpressionList());
}
void
Visitor::visit(ast::BinaryCompOrArith* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());
}
void
Visitor::visit(ast::Assignment* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());
}
