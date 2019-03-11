/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "InOrderTraversal.h"
#include <vector>

// helper method to add strings to the vector
void
InOrderTraversal::push(std::string val)
{
    traversalVec.push_back(val);
}

std::vector<std::string>
InOrderTraversal::getInOrderTraversal(ast::Start* node)
{
    // ensure cleared
    traversalVec.clear();

    // traverse ast
    visit(node);

    return traversalVec;
}

void
InOrderTraversal::visit(ast::ExportDefinition* node)
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

    push("ExportDefintion");
}

void
InOrderTraversal::visit(ast::ListDefinition* node)
{
    visit(node->getName());
    visit(node->getExpression());

    push("ListDefinition");
}

void
InOrderTraversal::visit(ast::DictionaryDefinition* node)
{
    visit(node->getName());
    visit(node->getExpression());

    push("DictionaryDefinition");
}

void
InOrderTraversal::visit(ast::NewUDTDefinition* node)
{
    visit(node->getVariable());
    visit(node->getExpression());

    push("NewUDTDefinition");
}

void
InOrderTraversal::visit(ast::PrimitiveDefition* node)
{
    visit(node->getVariable());
    visit(node->getBinaryExpression());

    push("PrimitiveDefinition");
}

void
InOrderTraversal::visit(ast::FunctionDefinition* node)
{
    visit(node->getName());

    for (auto const& input : node->getInputList())
    {
        visit(input);
    }

    visit(node->getOutput());

    visit(node->getBody());

    push("FunctionDefinition");
}

void
InOrderTraversal::visit(ast::Input* node)
{
    visit(node->getInput());

    push("Input");
}

void
InOrderTraversal::visit(ast::Output* node)
{
    visit(node->getOutput());

    push("Output");
}

void
InOrderTraversal::visit(ast::UserDefinedTypeDefinition* node)
{
    visit(node->getAttributes());
    visit(node->getMethods());

    push("UserDefinedTypeDefinition");
}

void
InOrderTraversal::visit(ast::UserDefinedTypeAttributes* node)
{
    visit(node->getName());

    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute);
    }

    push("UserDefinedTypeAttributes");
}

void
InOrderTraversal::visit(ast::UserDefinedTypeMethods* node)
{
    visit(node->getName());

    for (auto const& method : node->getMethods())
    {
        visit(method);
    }

    push("UserDefinedTypeMethods");
}

void
InOrderTraversal::visit(ast::UserDefinedType* node)
{
    visit(node->getName());
    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute);
    }

    push("UserDefinedType");
}

void
InOrderTraversal::visit(ast::InitialExecutionBody* node)
{
    visit(node->getBody());

    push("InitialExecutionBody");
}

void
InOrderTraversal::visit(ast::Variable* node)
{
    visit(node->getName());
    visit(node->getType());

    push("Variable");
}

void
InOrderTraversal::visit(ast::IfStatement* node)
{
    visit(node->getIfConditional());
    visit(node->getIfStatements());
    visit(node->getElseStatements());

    push("IfStatement");
}

void
InOrderTraversal::visit(ast::ReturnStatement* node)
{
    visit(node->getBinaryExpression());

    push("ReturnStatement");
}

void
InOrderTraversal::visit(ast::NewExpression* node)
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
    case ast::New::UserDefinedType:
    {
        ast::UserDefinedType* subnode =
            dynamic_cast<ast::UserDefinedType*>(newVal);
        visit(subnode);
        break;
    }
    }

    push("NewExpression");
}

void
InOrderTraversal::visit(ast::ArrayExpression* node)
{
    for (auto const& expression : node->getExpressionList())
    {
        visit(expression);
    }

    push("ArrayExpression");
}

void
InOrderTraversal::visit(ast::AttributeAccess* node)
{
    visit(node->getAttribute());

    push("AttributeAccess");
}

void
InOrderTraversal::visit(ast::MethodAccess* node)
{
    visit(node->getName());
    visit(node->getFunctionCall());

    push("MethodAccess");
}

void
InOrderTraversal::visit(ast::FunctionCall* node)
{
    ast::Identifier* name = node->getName();
    std::vector<ast::Primary*> args = node->getArguments();

    visit(name);

    for (auto const& arg : args)
    {
        visit(arg);
    }

    push("FunctionCall");
}

void
InOrderTraversal::visit(ast::BooleanLiteral* node)
{
    push("BooleanLiteral");
}
void
InOrderTraversal::visit(ast::DictionaryLiteral* node)
{
    for (auto const& dictionaryItem : node->getItems())
    {
        visit(dictionaryItem);
    }

    push("DictionaryLiteral");
}
void
InOrderTraversal::visit(ast::DictionaryItem* node)
{
    push("DictionaryItem");
}
void
InOrderTraversal::visit(ast::ListLiteral* node)
{
    for (auto const& listItem : node->getItems())
    {
        visit(listItem);
    }

    push("ListLiteral");
}

void
InOrderTraversal::visit(ast::ListItem* node)
{
    push("ListItem");
}

void
InOrderTraversal::visit(ast::IntegerLiteral* node)
{
    push("IntegerLiteral: " + node->getNum());
}

void
InOrderTraversal::visit(ast::FloatLiteral* node)
{
    push("FloatLiteral: " + node->getNum());
}

void
InOrderTraversal::visit(ast::StringLiteral* node)
{
    push("StringLiteral: " + node->getString());
}

void
InOrderTraversal::visit(ast::Identifier* node)
{
    push("Identifier: " + node->getValue());
}

void
InOrderTraversal::visit(ast::Typename* node)
{
    push("Typename: " + node->getType());
}

void
InOrderTraversal::visit(ast::Negation* node)
{
    visit(node->getBinaryExpression());

    push("Negation");
}

void
InOrderTraversal::visit(ast::Exponentiation* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Exponentiation");
}

void
InOrderTraversal::visit(ast::Multiplication* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Multiplication");
}

void
InOrderTraversal::visit(ast::Division* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Division");
}

void
InOrderTraversal::visit(ast::Modulo* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Modulo");
}

void
InOrderTraversal::visit(ast::Addition* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Addition");
}

void
InOrderTraversal::visit(ast::Subtraction* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());
    push("Subtraction");
}

void
InOrderTraversal::visit(ast::BinaryGreaterThan* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("BinaryGreaterThan");
}

void
InOrderTraversal::visit(ast::BinaryLessThan* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("BinaryLessThan");
}

void
InOrderTraversal::visit(ast::BinaryGreaterThanOrEqual* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("BinaryGreaterThanOrEqual");
}

void
InOrderTraversal::visit(ast::BinaryLessThanOrEqual* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("BinaryLessThanOrEqual");
}

void
InOrderTraversal::visit(ast::EquivalenceComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("EquivalenceComparison");
}

void
InOrderTraversal::visit(ast::NonEquivalenceComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("NonEquivalenceComparison");
}

void
InOrderTraversal::visit(ast::AndComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("AndComparison");
}

void
InOrderTraversal::visit(ast::OrComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("OrComparison");
}

void
InOrderTraversal::visit(ast::Assignment* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Assignment");
}
