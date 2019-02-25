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
        ast::SourcePart::SourcePartType type = srcPart->getSourcePartType();
        switch (type)
        {
        case ast::SourcePart::ExportDefinition:
        {
            ast::ExportDefinition* subnode =
                dynamic_cast<ast::ExportDefinition*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::GeneralDecleration:
        {
            ast::GeneralDecleration* subnode =
                dynamic_cast<ast::GeneralDecleration*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::FunctionDefinition:
        {
            ast::FunctionDefinition* subnode =
                dynamic_cast<ast::FunctionDefinition*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::UserDefinedTypeDefinition:
        {
            ast::UserDefinedTypeDefinition* subnode =
                dynamic_cast<ast::UserDefinedTypeDefinition*>(srcPart);
            visit(subnode);
            break;
        }
        case ast::SourcePart::InitialExecutionBody:
        {
            ast::InitialExecutionBody* subnode =
                dynamic_cast<ast::InitialExecutionBody*>(srcPart);
            visit(subnode);
            break;
        }
        }
    }
}
void
Visitor::visit(ast::ExportDefinition* node)
{
    ast::Exportable* exprt = node->getExport();
    ast::Exportable::ExportableType type = exprt->getExportableType();
    switch (type)
    {
    case ast::Exportable::GeneralDecleration:
    {
        ast::GeneralDecleration* subnode =
            dynamic_cast<ast::GeneralDecleration*>(exprt);
        visit(subnode);
        break;
    }
    case ast::Exportable::FunctionDefinition:
    {
        ast::FunctionDefinition* subnode =
            dynamic_cast<ast::FunctionDefinition*>(exprt);
        visit(subnode);
        break;
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
    case ast::GeneralDefinition::ListDefinition:
    {
        ast::ListDefinition* subnode = dynamic_cast<ast::ListDefinition*>(node);
        visit(subnode);
        break;
    }
    case ast::GeneralDefinition::DictionaryDefinition:
    {
        ast::DictionaryDefinition* subnode =
            dynamic_cast<ast::DictionaryDefinition*>(node);
        visit(subnode);
        break;
    }
    case ast::GeneralDefinition::NewVariableDefinition:
    {
        ast::NewVariableDefinition* subnode =
            dynamic_cast<ast::NewVariableDefinition*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::ListDefinition* node)
{
    visit(node->getName());
    visit(node->getExpression());
}
void
Visitor::visit(ast::DictionaryDefinition* node)
{
    visit(node->getName());
    visit(node->getExpression());
}
void
Visitor::visit(ast::NewVariableDefinition* node)
{
    visit(node->getVariable());
    visit(node->getExpression());
}
void
Visitor::visit(ast::Expression* node)
{
    switch (node->getExpressionType())
    {
    case ast::Expression::NewExpression:
    {
        ast::NewExpression* subnode = dynamic_cast<ast::NewExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::BinaryExpression:
    {
        ast::BinaryExpression* subnode =
            dynamic_cast<ast::BinaryExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::ArrayExpression:
    {
        ast::ArrayExpression* subnode =
            dynamic_cast<ast::ArrayExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::MemberAccess:
    {
        ast::MemberAccess* subnode = dynamic_cast<ast::MemberAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::FunctionCallExpression:
    {
        ast::FunctionCall* subnode = dynamic_cast<ast::FunctionCall*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::PrimaryExpression:
    {
        ast::PrimaryExpression* subnode =
            dynamic_cast<ast::PrimaryExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Negation:
    {
        ast::Negation* subnode = dynamic_cast<ast::Negation*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Exponentiation:
    {
        ast::Exponentiation* subnode = dynamic_cast<ast::Exponentiation*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Multiplication:
    {
        ast::Multiplication* subnode = dynamic_cast<ast::Multiplication*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Division:
    {
        ast::Division* subnode = dynamic_cast<ast::Division*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Modulo:
    {
        ast::Modulo* subnode = dynamic_cast<ast::Modulo*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Addition:
    {
        ast::Addition* subnode = dynamic_cast<ast::Addition*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Subtraction:
    {
        ast::Subtraction* subnode = dynamic_cast<ast::Subtraction*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::BinaryGreaterThan:
    {
        ast::BinaryGreaterThan* subnode =
            dynamic_cast<ast::BinaryGreaterThan*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::BinaryLessThan:
    {
        ast::BinaryLessThan* subnode = dynamic_cast<ast::BinaryLessThan*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::BinaryGreaterThanOrEqual:
    {
        ast::BinaryGreaterThanOrEqual* subnode =
            dynamic_cast<ast::BinaryGreaterThanOrEqual*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::BinaryLessThanOrEqual:
    {
        ast::BinaryLessThanOrEqual* subnode =
            dynamic_cast<ast::BinaryLessThanOrEqual*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::EquivalenceComparison:
    {
        ast::EquivalenceComparison* subnode =
            dynamic_cast<ast::EquivalenceComparison*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::NonEquivalenceComparison:
    {
        ast::NonEquivalenceComparison* subnode =
            dynamic_cast<ast::NonEquivalenceComparison*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::AndComparison:
    {
        ast::AndComparison* subnode = dynamic_cast<ast::AndComparison*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::OrComparison:
    {
        ast::OrComparison* subnode = dynamic_cast<ast::OrComparison*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::Assignment:
    {
        ast::Assignment* subnode = dynamic_cast<ast::Assignment*>(node);
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
    {
        visit(input);
    }

    for (auto const& output : node->getOutputList())
    {
        visit(output);
    }

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
    visit(node->getAttributes());
    visit(node->getMethods());
}
void
Visitor::visit(ast::UserDefinedTypeAttributes* node)
{
    visit(node->getName());

    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute);
    }
}
void
Visitor::visit(ast::UserDefinedTypeMethods* node)
{
    visit(node->getName());

    for (auto const& method : node->getMethods())
    {
        visit(method);
    }
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
        ast::Statement::StatementType type = statement->getStatementType();

        switch (type)
        {
        case ast::Statement::IfStatement:
        {
            ast::IfStatement* subnode =
                dynamic_cast<ast::IfStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::ReturnStatement:
        {
            ast::ReturnStatement* subnode =
                dynamic_cast<ast::ReturnStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BlockStatement:
        {
            ast::Block* subnode = dynamic_cast<ast::Block*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::GeneralDecleration:
        {
            ast::GeneralDecleration* subnode =
                dynamic_cast<ast::GeneralDecleration*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::ExpressionStatement:
        {
            ast::ExpressionStatement* subnode =
                dynamic_cast<ast::ExpressionStatement*>(statement);
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
Visitor::visit(ast::ExpressionStatement* node)
{
    visit(node->getExpression());
}
void
Visitor::visit(ast::ReturnStatement* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::NewExpression* node)
{
    ast::New* newVal = node->getNewVal();
    ast::New::NewType type = newVal->getNewType();

    switch (type)
    {
    case ast::New::DictionaryLiteral:
    {
        ast::DictionaryLiteral* subnode =
            dynamic_cast<ast::DictionaryLiteral*>(newVal);
        visit(subnode);
        break;
    }
    case ast::New::ListLiteral:
    {
        ast::ListLiteral* subnode = dynamic_cast<ast::ListLiteral*>(newVal);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::ArrayExpression* node)
{
    for (auto const& expression : node->getExpressionList())
    {
        visit(expression);
    }
}
void
Visitor::visit(ast::MemberAccess* node)
{
    switch (node->getMemberAccessType())
    {
    case ast::MemberAccess::AttributeAccess:
    {
        ast::AttributeAccess* subnode =
            dynamic_cast<ast::AttributeAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::MemberAccess::MethodAccess:
    {
        ast::MethodAccess* subnode = dynamic_cast<ast::MethodAccess*>(node);
        visit(subnode);
        break;
    }
    }
}
void
Visitor::visit(ast::AttributeAccess* node)
{
    visit(node->getAttribute());
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
    std::vector<ast::Identifier*> args = node->getArguments();

    for (auto const& arg : args)
    {
        visit(arg);
    }
}
void
Visitor::visit(ast::PrimaryExpression* node)
{
    ast::Primary* primary = node->getPrimary();
    ast::Primary::PrimaryType type = primary->getPrimaryType();

    switch (type)
    {
    case ast::Primary::Identifier:
    {
        ast::Identifier* subnode = dynamic_cast<ast::Identifier*>(primary);
        visit(subnode);
        break;
    }
    case ast::Primary::StringLiteral:
    {
        ast::StringLiteral* subnode =
            dynamic_cast<ast::StringLiteral*>(primary);
        visit(subnode);
        break;
    }
    case ast::Primary::BooleanLiteral:
    {
        ast::BooleanLiteral* subnode =
            dynamic_cast<ast::BooleanLiteral*>(primary);
        visit(subnode);
        break;
    }
    case ast::Primary::IntegerLiteral:
    {
        ast::IntegerLiteral* subnode =
            dynamic_cast<ast::IntegerLiteral*>(primary);
        visit(subnode);
        break;
    }
    case ast::Primary::FloatLiteral:
    {
        ast::FloatLiteral* subnode = dynamic_cast<ast::FloatLiteral*>(primary);
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
Visitor::visit(ast::DictionaryLiteral* node)
{
    for (auto const& dictionaryItem : node->getItems())
    {
        visit(dictionaryItem);
    }
}
void
Visitor::visit(ast::DictionaryItem* node)
{
    visit(node->getKey());
    visit(node->getValue());
}
void
Visitor::visit(ast::ListLiteral* node)
{
    for (auto const& listItem : node->getItems())
    {
        visit(listItem);
    }
}
void
Visitor::visit(ast::ListItem* node)
{
    visit(node->getName());
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
Visitor::visit(ast::ByteLiteral* node)
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
    visit(node->getExpr());
}
void
Visitor::visit(ast::Exponentiation* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::Multiplication* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::Division* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::Modulo* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::Addition* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::Subtraction* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::BinaryExpression* node)
{
    for (auto const& expr : node->getExpressionList())
    {
        visit(expr);
    }
}
void
Visitor::visit(ast::BinaryGreaterThan* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::BinaryLessThan* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::BinaryGreaterThanOrEqual* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::BinaryLessThanOrEqual* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::EquivalenceComparison* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::NonEquivalenceComparison* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::AndComparison* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::OrComparison* node)
{
    visit(node->getExpr());
}
void
Visitor::visit(ast::Assignment* node)
{
    visit(node->getExpr());
}
