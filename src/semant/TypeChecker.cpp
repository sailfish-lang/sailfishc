
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
            var->getName()->getLineNum(),
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpression());
}

void
TypeChecker::visit(ast::ListDefinition* node)
{
    std::string name = node->getName()->getValue();
    std::string type = node->getType()->getType();

    bool isUnique =
        symbolTable->addSymbol(name, type + "[]", Symbol::SymbolType::List);

    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getName()->getLineNum(),
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpression());
}

void
TypeChecker::visit(ast::DictionaryDefinition* node)
{
    std::string name = node->getName()->getValue();
    std::string keyType = node->getKeyType()->getType();
    std::string valueType = node->getValueType()->getType();

    bool isUnique = symbolTable->addSymbol(
        name, "dictionary", keyType, valueType, Symbol::SymbolType::Dictionary);

    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getName()->getLineNum(),
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpression());
}

// void
// TypeChecker::visit(ast::UserDefinedTypeDefinition* node)
// {
//     ast::UserDefinedTypeAttributes* attributes = node->getAttributes();
//     ast::UserDefinedTypeMethods* methods = node->getMethods();

//     // check that the names match
//     std::string name_a = attributes->getName()->getValue();
//     std::string name_m = methods->getName()->getValue();

//     if (name_a != name_m)
//     {
//         symbolTableErrorHandler->handle(new Error(
//             attributes->getLineNum(),
//             "Expected UDT attributes and methods to be for same UDT, i.e.
//             name " "of attributes: " +
//                 name_a + " and name of methods: " + name_m + "don't
//                 match."));
//     }

//     // otherwise capture the name
//     std::string udt_name = name_a;

//     // create a symbol table for the attributes
//     SymbolTable* st_a = new SymbolTable();

//     for (auto const& var : attributes->getAttributes())
//     {
//         std::string name = var->getName()->getValue();
//         std::string type = var->getType()->getType();

//         bool isUnique =
//             st_a->addSymbol(name, type, Symbol::SymbolType::Primitive);

//         if (!isUnique)
//         {
//             symbolTableErrorHandler->handle(new Error(
//                 var->getName()->getLineNum(),
//                 "Invalid redecleration of attribute with name: " + name +
//                     " in UDT named: " + udt_name + "."));
//         }
//     }

//     // create a symbol table for the methods
//     SymbolTable* st_m = new SymbolTable();

//     for (auto const& func : methods->getMethods())
//     {
//         std::string func_name = func->getName()->getValue();
//         for (auto const& inp : func->getInputList())
//         {
//             // capture var
//             ast::Variable* var = inp->getInput();

//             std::string name = var->getName()->getValue();
//             std::string type = var->getType()->getType();

//             bool isUnique =
//                 st_a->addSymbol(name, type,
//                 Symbol::SymbolType::Primitive);

//             if (!isUnique)
//             {
//                 symbolTableErrorHandler->handle(new Error(
//                     var->getName()->getLineNum(),
//                     "Invalid duplication of parameter with name: " + name
//                     +
//                         " in function named: " + func_name + "."));
//             }
//         }
//     }
// }