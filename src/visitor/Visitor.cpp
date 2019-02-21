/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */

#include "Visitor.h"
#include <iostream>
#include <vector>

void
Visitor::visit(ast::Start* node)
{
    std::cout << "At the start\n";

    visit(node->getSource());
}
void
Visitor::visit(ast::Source* node)
{
    std::cout << "At the source\n";

    std::vector<ast::SourcePart*> srcParts = node->getSourceParts();

    for (auto const& srcPart : srcParts)
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
    std::cout << "At an Export Definition\n";

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
    std::cout << "At a General Decleration\n";

    ast::GeneralDefinition* gd = node->getDefinition();

    visit(gd);
}
void
Visitor::visit(ast::GeneralDefinition* node)
{
    std::cout << "At a General Definition\n";

    ast::GeneralDefinition::GeneralDefinitionType type =
        node->getGeneralDefinitionType();

    switch (type)
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
    std::cout << "At a List Definition\n";

    ast::Identifier* name = node->getName();
    ast::Expression* expr = node->getExpression();

    visit(name);
    visit(expr);
}
void
Visitor::visit(ast::DictionaryDefinition* node)
{
    std::cout << "At a Dictionary Definition\n";

    ast::Identifier* name = node->getName();
    ast::Expression* expr = node->getExpression();

    visit(name);
    visit(expr);
}
void
Visitor::visit(ast::NewVariableDefinition* node)
{
    std::cout << "At a New Variable Definition\n";

    ast::Variable* var = node->getVariable();
    visit(var);

    ast::Expression* expr = node->getExpression();
    visit(expr);
}
void
Visitor::visit(ast::Expression* node)
{
    std::cout << "At an Expression\n";

    ast::Expression::ExpressionType type = node->getExpressionType();

    switch (type)
    {
    case ast::Expression::NewExpression:
    {
        ast::NewExpression* subnode = dynamic_cast<ast::NewExpression*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::IndexAccess:
    {
        ast::IndexAccess* subnode = dynamic_cast<ast::IndexAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::MemberAccess:
    {
        ast::MemberAccess* subnode = dynamic_cast<ast::MemberAccess*>(node);
        visit(subnode);
        break;
    }
    case ast::Expression::FunctionCall:
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
    std::cout << "At a Function Definition\n";

    ast::Identifier* name = node->getName();
    std::vector<ast::Input*> inputs = node->getInputList();
    std::vector<ast::Output*> outputs = node->getOutputList();
    ast::Block* body = node->getBody();

    visit(name);

    for (auto const& input : inputs)
    {
        visit(input);
    }

    for (auto const& output : outputs)
    {
        visit(output);
    }

    visit(body);
}
void
Visitor::visit(ast::Input* node)
{
    std::cout << "At an input\n";

    ast::Variable* var = node->getInput();

    visit(var);
}
void
Visitor::visit(ast::Output* node)
{
    std::cout << "At an output\n";

    ast::Typename* type = node->getOutput();

    visit(type);
}
void
Visitor::visit(ast::UserDefinedTypeDefinition* node)
{
    std::cout << "At a User defined type definition\n";

    ast::UserDefinedTypeAttributes* attributes = node->getAttributes();
    ast::UserDefinedTypeMethods* methods = node->getMethods();

    visit(attributes);
    visit(methods);
}
void
Visitor::visit(ast::UserDefinedTypeAttributes* node)
{
    std::cout << "At a User defined type attributes.\n";

    ast::Identifier* id = node->getName();
    visit(id);
}
void
Visitor::visit(ast::UserDefinedTypeMethods* node)
{
    std::cout << "At a User defined type methods.\n";

    ast::Identifier* id = node->getName();
    visit(id);
}
void
Visitor::visit(ast::InitialExecutionBody* node)
{
    std::cout << "At an Initial Execution Body\n";
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
    std::cout << "At a Variable\n";

    ast::Identifier* name = node->getName();
    ast::Typename* type = node->getType();

    visit(type);
    visit(name);
}
void
Visitor::visit(ast::Block* node)
{
    std::cout << "At a Block\n";

    std::vector<ast::Statement*> statements = node->getStatements();

    for (auto const& statement : statements)
    {
    }
}
void
Visitor::visit(ast::IfStatement* node)
{
    std::cout << "At an If Statement\n";
}
void
Visitor::visit(ast::LoopStatement* node)
{
    std::cout << "At a Loop Statement\n";
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
Visitor::visit(ast::SimpleStatement* node)
{
    std::cout << "At a Simple Statement\n";
}
void
Visitor::visit(ast::ExpressionStatement* node)
{
}
void
Visitor::visit(ast::ReturnStatement* node)
{
    std::cout << "At a Return Statement\n";
}
void
Visitor::visit(ast::NewExpression* node)
{
    std::cout << "At a New Expression\n";

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
Visitor::visit(ast::IndexAccess* node)
{
    std::cout << "At a Index Access\n";
}
void
Visitor::visit(ast::MemberAccess* node)
{
    std::cout << "At a Member Access\n";
}
void
Visitor::visit(ast::FunctionCall* node)
{
    std::cout << "At a Function Call\n";
}
void
Visitor::visit(ast::PrimaryExpression* node)
{
    std::cout << "At a Primary Expression\n";

    ast::Primary* primary = node->getPrimary();
}
void
Visitor::visit(ast::BooleanLiteral* node)
{
}
void
Visitor::visit(ast::DictionaryLiteral* node)
{
    std::cout << "At Dictionary Literal\n";

    std::vector<ast::DictionaryItem*> dictionaryItems = node->getItems();

    for (auto const& dictionaryItem : dictionaryItems)
    {
        visit(dictionaryItem);
    }
}
void
Visitor::visit(ast::DictionaryItem* node)
{
    std::cout << "At Dictionary Item\n";

    ast::Identifier* key = node->getKey();
    ast::Identifier* value = node->getValue();

    visit(key);
    visit(value);
}
void
Visitor::visit(ast::ListLiteral* node)
{
    std::cout << "At List Literal\n";

    std::vector<ast::ListItem*> listItems = node->getItems();

    for (auto const& listItem : listItems)
    {
        visit(listItem);
    }
}
void
Visitor::visit(ast::ListItem* node)
{
    std::cout << "At List Item\n";

    ast::Identifier* name = node->getName();

    visit(name);
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
    std::cout << "At an identifier with ";

    std::string value = node->getValue();

    std::cout << "VALUE: " << value << '\n';
}
void
Visitor::visit(ast::Typename* node)
{
    std::cout << "At a typename";

    std::string type = node->getType();

    std::cout << " VALUE: " << type << '\n';
}
void
Visitor::visit(ast::Negation* node)
{
    std::cout << "At a Negation\n";
}
void
Visitor::visit(ast::Exponentiation* node)
{
    std::cout << "At a Exponentiation\n";
}
void
Visitor::visit(ast::Multiplication* node)
{
    std::cout << "At a Muktiplication\n";
}
void
Visitor::visit(ast::Division* node)
{
    std::cout << "At a Division\n";
}
void
Visitor::visit(ast::Modulo* node)
{
    std::cout << "At a Modulo\n";
}
void
Visitor::visit(ast::Addition* node)
{
    std::cout << "At an Addition\n";
}
void
Visitor::visit(ast::Subtraction* node)
{
    std::cout << "At a Subtraction\n";
}
void
Visitor::visit(ast::BinaryGreaterThan* node)
{
    std::cout << "At a Binary Greater Than\n";
}
void
Visitor::visit(ast::BinaryLessThan* node)
{
    std::cout << "At a Binary Less Than\n";
}
void
Visitor::visit(ast::BinaryGreaterThanOrEqual* node)
{
    std::cout << "At a Binary Greater Than or Equal\n";
}
void
Visitor::visit(ast::BinaryLessThanOrEqual* node)
{
    std::cout << "At a Binary Less Than or Equal\n";
}
void
Visitor::visit(ast::EquivalenceComparison* node)
{
    std::cout << "At a Equivalence Comparison\n";
}
void
Visitor::visit(ast::NonEquivalenceComparison* node)
{
    std::cout << "At a Non Equivalence Comparison\n";
}
void
Visitor::visit(ast::AndComparison* node)
{
    std::cout << "At a And Comparison\n";
}
void
Visitor::visit(ast::OrComparison* node)
{
    std::cout << "At a Or Comparison\n";
}
void
Visitor::visit(ast::Break* node)
{
    std::cout << "BREAK\n";
}
void
Visitor::visit(ast::Continue* node)
{
    std::cout << "CONTINUE\n";
}
void
Visitor::visit(ast::Assignment* node)
{
    std::cout << "At a Assignment\n";
}