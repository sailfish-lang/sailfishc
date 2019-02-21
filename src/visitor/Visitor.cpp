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
    }
}
void
Visitor::visit(ast::ExportDefinition* node)
{
    std::cout << "At an Export Definition\n";

    ast::Exportable* exprt = node->getExport();
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
}
void
Visitor::visit(ast::ListDefinition* node)
{
    std::cout << "At a List Definition\n";
}
void
Visitor::visit(ast::DictionaryDefinition* node)
{
    std::cout << "At a Dictionary Definition\n";
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
    std::cout << "At a Adiition\n";
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