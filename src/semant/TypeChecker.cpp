
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

void
TypeChecker::visit(ast::FunctionDefinition* node)
{

    // ensure scope is global since functions cannot be in blocks
    if (!symbolTable->isGlobalScope())
    {
        symbolTableErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Functions can only be declared at the global level, "
                      "i.e. cannot be nested!"));
    }

    std::string name = node->getName()->getValue();

    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    // enter a scope for this function, starting with the parameters
    symbolTable->enterScope();

    //  capture for the addition of the function to the symbol table
    for (auto const& input : node->getInputList())
    {
        ast::Variable* var = input->getInput();
        std::string inp_type = var->getType()->getType();

        // add to list of function inputs
        inputs.push_back(inp_type);
    }

    // capture for the addition of the function to the symbol table
    for (auto const& output : node->getOutputList())
    {
        ast::Typename* var = output->getOutput();
        std::string out_type = var->getType();

        outputs.push_back(out_type);
    }

    symbolTable->addSymbol(name, "function", inputs, outputs,
                           Symbol::SymbolType::Function);

    // enter a scope for this function, starting with the parameters
    symbolTable->enterScope();

    // TODO: don't visit each param for a second time here!
    //  add each input to the nested scope for the function
    for (auto const& input : node->getInputList())
    {
        ast::Variable* var = input->getInput();
        std::string inp_name = var->getName()->getValue();
        std::string inp_type = var->getType()->getType();

        // add to the symbol table
        bool isUnique = symbolTable->addSymbol(inp_name, inp_type,
                                               Symbol::SymbolType::Dictionary);

        if (!isUnique)
        {
            symbolTableErrorHandler->handle(new Error(
                node->getName()->getLineNum(),
                "Invalid redecleration of a variable with name: " + name +
                    "."));
        }
    }

    // visit the function body
    visit(node->getBody());

    // exit scope once we exit the body
    symbolTable->exitScope();
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