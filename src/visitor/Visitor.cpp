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
        std::string type = srcPart->getType();

        if (type == "ExportDefinition")
        {
            ast::ExportDefinition* ed =
                dynamic_cast<ast::ExportDefinition*>(srcPart);
            visit(ed);
        }
        else if (type == "GeneralDecleration")
        {
            ast::GeneralDecleration* gd =
                dynamic_cast<ast::GeneralDecleration*>(srcPart);
            visit(gd);
        }
        else if (type == "FunctionDefinition")
        {
            ast::FunctionDefinition* fd =
                dynamic_cast<ast::FunctionDefinition*>(srcPart);
            visit(fd);
        }
        else if (type == "UserDefinedTypeDefinition")
        {
            ast::UserDefinedTypeDefinition* udtd =
                dynamic_cast<ast::UserDefinedTypeDefinition*>(srcPart);
            visit(udtd);
        }
        else if (type == "InitialExecutionBody")
        {
            ast::InitialExecutionBody* ieb =
                dynamic_cast<ast::InitialExecutionBody*>(srcPart);
            visit(ieb);
        }
    }
}
void
Visitor::visit(ast::ExportDefinition* node)
{
    std::cout << "At an Export Definition\n";

    ast::Exportable* exprt = node->getExport();

    std::string type = exprt->getType();

    if (type == "FunctionDefinition")
    {
        ast::FunctionDefinition* fd =
            dynamic_cast<ast::FunctionDefinition*>(exprt);
        visit(fd);
    }
    else if (type == "GeneralDecleration")
    {
        ast::GeneralDecleration* gd =
            dynamic_cast<ast::GeneralDecleration*>(exprt);
        visit(gd);
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

    std::string type = node->getType();

    if (type == "ListDefinition")
    {
        ast::ListDefinition* ld = dynamic_cast<ast::ListDefinition*>(node);
        visit(ld);
    }
    else if (type == "DictionaryDefinition")
    {
        ast::DictionaryDefinition* dd =
            dynamic_cast<ast::DictionaryDefinition*>(node);
        visit(dd);
    }
    else if (type == "NewVariableDefinition")
    {
        ast::NewVariableDefinition* nvd =
            dynamic_cast<ast::NewVariableDefinition*>(node);
        visit(nvd);
    }
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
    std::string type = node->getType();

    if (type == "IndexAccess")
    {
        ast::IndexAccess* ia = dynamic_cast<ast::IndexAccess*>(node);
        visit(ia);
    }
    else if (type == "MemberAccess")
    {
        ast::MemberAccess* ma = dynamic_cast<ast::MemberAccess*>(node);
        visit(ma);
    }
    else if (type == "FunctionCall")
    {
        ast::FunctionCall* fc = dynamic_cast<ast::FunctionCall*>(node);
        visit(fc);
    }
    else if (type == "PrimaryExpression")
    {
        ast::PrimaryExpression* pe =
            dynamic_cast<ast::PrimaryExpression*>(node);
        visit(pe);
    }
    else if (type == "Negation")
    {
        ast::Negation* n = dynamic_cast<ast::Negation*>(node);
        visit(n);
    }
    else if (type == "Exponentiation")
    {
        ast::Exponentiation* e = dynamic_cast<ast::Exponentiation*>(node);
        visit(e);
    }
    else if (type == "Multiplication")
    {
        ast::Multiplication* m = dynamic_cast<ast::Multiplication*>(node);
        visit(m);
    }
    else if (type == "Divison")
    {
        ast::Division* d = dynamic_cast<ast::Division*>(node);
        visit(d);
    }
    else if (type == "Modulo")
    {
        ast::Modulo* m = dynamic_cast<ast::Modulo*>(node);
        visit(m);
    }
    else if (type == "Addition")
    {
        ast::Addition* a = dynamic_cast<ast::Addition*>(node);
        visit(a);
    }
    else if (type == "Subtraction")
    {
        ast::Subtraction* s = dynamic_cast<ast::Subtraction*>(node);
        visit(s);
    }
    else if (type == "BinaryGreaterThan")
    {
        ast::BinaryGreaterThan* bgt =
            dynamic_cast<ast::BinaryGreaterThan*>(node);
        visit(bgt);
    }
    else if (type == "BinaryLessThan")
    {
        ast::BinaryLessThan* blt = dynamic_cast<ast::BinaryLessThan*>(node);
        visit(blt);
    }
    else if (type == "BinaryGreaterThanOrEqual")
    {
        ast::BinaryGreaterThanOrEqual* bgtoe =
            dynamic_cast<ast::BinaryGreaterThanOrEqual*>(node);
        visit(bgtoe);
    }
    else if (type == "BinaryLessThanOrEqual")
    {
        ast::BinaryLessThanOrEqual* bltoe =
            dynamic_cast<ast::BinaryLessThanOrEqual*>(node);
        visit(bltoe);
    }
    else if (type == "EquivalenceComparison")
    {
        ast::EquivalenceComparison* ec =
            dynamic_cast<ast::EquivalenceComparison*>(node);
        visit(ec);
    }
    else if (type == "NonEquivalenceComparison")
    {
        ast::NonEquivalenceComparison* nec =
            dynamic_cast<ast::NonEquivalenceComparison*>(node);
        visit(nec);
    }
    else if (type == "AndComparison")
    {
        ast::AndComparison* ac = dynamic_cast<ast::AndComparison*>(node);
        visit(ac);
    }
    else if (type == "OrComparison")
    {
        ast::OrComparison* oc = dynamic_cast<ast::OrComparison*>(node);
        visit(oc);
    }
    else if (type == "Assignment")
    {
        ast::Assignment* a = dynamic_cast<ast::Assignment*>(node);
        visit(a);
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
        std::string type = statement->getType();

        if (type == "IfStatement")
        {
            ast::IfStatement* is = dynamic_cast<ast::IfStatement*>(statement);
            visit(is);
        }
        else if (type == "LoopStatement")
        {
            ast::LoopStatement* ls =
                dynamic_cast<ast::LoopStatement*>(statement);
            visit(ls);
        }
        else if (type == "ReturnStatement")
        {
            ast::ReturnStatement* rs =
                dynamic_cast<ast::ReturnStatement*>(statement);
            visit(rs);
        }
        else if (type == "SimpleStatement")
        {
            ast::SimpleStatement* ss =
                dynamic_cast<ast::SimpleStatement*>(statement);
            visit(ss);
        }
        else if (type == "Continue")
        {
            ast::Continue* c = dynamic_cast<ast::Continue*>(statement);
            visit(c);
        }
        else if (type == "Break")
        {
            ast::Break* b = dynamic_cast<ast::Break*>(statement);
            visit(b);
        }
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

    std::string type = node->getType();

    if (type == "ExpressionStatement")
    {
        ast::ExpressionStatement* es =
            dynamic_cast<ast::ExpressionStatement*>(node);
        visit(es);
    }
    else if (type == "GeneralDefinition")
    {
        ast::GeneralDefinition* gd =
            dynamic_cast<ast::GeneralDefinition*>(node);
        visit(gd);
    }
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

    std::string type = primary->getType();

    if (type == "BooleanLiteral")
    {
        ast::BooleanLiteral* bl = dynamic_cast<ast::BooleanLiteral*>(primary);
        visit(bl);
    }
    else if (type == "DictionaryLiteral")
    {
        ast::DictionaryLiteral* bl =
            dynamic_cast<ast::DictionaryLiteral*>(primary);
        visit(bl);
    }
    else if (type == "ListLiteral")
    {
        ast::ListLiteral* ll = dynamic_cast<ast::ListLiteral*>(primary);
        visit(ll);
    }
    else if (type == "IntegerLiteral")
    {
        ast::IntegerLiteral* il = dynamic_cast<ast::IntegerLiteral*>(primary);
        visit(il);
    }
    else if (type == "FloatLiteral")
    {
        ast::FloatLiteral* fl = dynamic_cast<ast::FloatLiteral*>(primary);
        visit(fl);
    }
    else if (type == "StringLiteral")
    {
        ast::StringLiteral* sl = dynamic_cast<ast::StringLiteral*>(primary);
        visit(sl);
    }
    else if (type == "Identifier")
    {
        ast::Identifier* i = dynamic_cast<ast::Identifier*>(primary);
        visit(i);
    }
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