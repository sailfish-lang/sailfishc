
#include "./TypeChecker.h"
#include "SymbolTable.h"

void
TypeChecker::check(ast::Start* root)
{
    visit(root);
}

void
TypeChecker::visit(ast::NewVariableDefinition* node)
{
    // capture variable information to add to the symbol table
    ast::Variable* var = node->getVariable();
    std::string name = var->getName()->getValue();
    std::string type = var->getType()->getType();

    bool isUnique =
        symbolTable->addSymbol(name, type, Symbol::SymbolType::Primitive);

    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            0, 0,
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpression());
}