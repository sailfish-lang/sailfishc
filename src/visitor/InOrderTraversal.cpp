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
    visit(node->getName());

    push("UserDefinedTypeAttributes");
    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute);
    }

    push("UserDefinedTypeMethods");
    for (auto const& method : node->getMethods())
    {
        visit(method);
    }

    push("UserDefinedTypeDefinition");
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
InOrderTraversal::visit(ast::BinaryCompOrArith* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("BinaryCompOrArith");
}

void
InOrderTraversal::visit(ast::Assignment* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    push("Assignment");
}
