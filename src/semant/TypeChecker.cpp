/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "TypeChecker.h"

// ------- Helper Functions       -------- //
// given a symbol table, ensure a variable name is legal
bool
TypeChecker::nameIsLegal(std::string name, int lineNumber)
{
    if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
    {
        semanticErrorHandler->handle(
            new Error(lineNumber, "Declared variable named: " + name +
                                      " illegally shares its name with a "
                                      "type or a keyword/reserved word."));
        return false;
    }

    return true;
}

// given a type and its name, ensure the type exists
bool
TypeChecker::typeExists(std::string type, std::string name, int lineNumber)
{
    if (!isPrimitive(type) && !udtTable->hasUDT(type))
    {
        semanticErrorHandler->handle(new Error(
            lineNumber, "Declared type: " + type + " for variable named: " +
                            name + " is not a legal or known type."));
    }
}

// encapsulate addition to symbol table and error handling for it
bool
TypeChecker::tryAddToSymbolTable(std::string name, std::string type,
                                 SymbolTable* symbolTable, int lineNumber)
{
    bool isUnique = symbolTable->addSymbol(name, type);

    // symbol table entry must be unique for current scope
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            lineNumber,
            "Invalid redecleration of variable with name: " + name + "."));
    }
}

// ------- Additions to the symbol table ------- //
void
TypeChecker::check(ast::Start* root)
{
    visit(root);
}

void
TypeChecker::visit(ast::NewUDTDefinition* node)
{
    bool isGood = true;

    // capture variable information to add to the symbol table
    ast::Variable* var = node->getVariable();
    std::string name = var->getName()->getValue();
    std::string type = var->getType()->getType();
    int lineNumber = var->getLineNum();

    std::string adjustedName = "U" + type;

    nameIsLegal(name, lineNumber);
    typeExists(type, name, lineNumber);
    tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);

    // enter into the symbol table for udt
    SymbolTable* tempST = symbolTable;
    symbolTable = udtTable->getAttributeSymbolTable(type);

    // visit expression
    visit(node->getExpression());

    std::string exprType =
        expressionHelper(node->getExpression(), symbolTable,
                         semanticErrorHandler, udtTable, curUDT);

    std::string temp;
    std::string prev;

    std::string baseType = exprType.substr(0, exprType.find("_"));
    temp = exprType.substr(exprType.find("_") + 1, exprType.length());

    std::string udtTypeName = temp.substr(0, temp.find("_"));
    temp = temp.substr(temp.find("_") + 1, temp.length());
    prev = udtTypeName;

    // make sure the type of the assignment is a udt
    if (baseType != "udt")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected udt type. Instead received: " + baseType + "."));
    }

    // make sure the type name of the assigned udt matches
    else if (udtTypeName != type)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected udt of type: " + type +
                                    " and received: " + udtTypeName + "."));
    }

    // ensure all attributes exist
    else
    {
        SymbolTable* st = udtTable->getAttributeSymbolTable(udtTypeName);

        while (temp.length() != prev.length())
        {
            std::string varName = temp.substr(0, temp.find("_"));
            temp = temp.substr(temp.find("_") + 1, temp.length());

            std::string varType = temp.substr(0, temp.find("_"));
            temp = temp.substr(temp.find("_") + 1, temp.length());
            prev = varType;

            // ensure attribute type matches attribute name
            if (varName != varType)
            {
                semanticErrorHandler->handle(new Error(
                    node->getLineNum(),
                    "Attribute: " + varType + " for declared udt of type: " +
                        varName + " does not match expected type: " + "."));
                break;
            }
        }
    }

    // exit the symbol table for udt
    symbolTable = tempST;
}

void
TypeChecker::visit(ast::PrimitiveDefition* node)
{
    std::string name = node->getVariable()->getName()->getValue();
    std::string type = node->getVariable()->getType()->getType();
    int lineNumber = node->getLineNum();

    nameIsLegal(name, lineNumber);

    std::string adjustedName = "P" + type;

    tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);

    // ensure the type is a primitive
    if (!isPrimitive(type))
    {
        semanticErrorHandler->handle(new Error(
            node->getVariable()->getType()->getLineNum(),
            "Defined primitives's type of: " + type +
                " for primitive: " + name + " is not a recognized primitive."));
    }

    // visit expression
    visit(node->getBinaryExpression());

    std::string exprType =
        getRightExpressionType(node->getBinaryExpression(), symbolTable,
                               semanticErrorHandler, udtTable, curUDT);

    // ensure assignment is the expected type
    if (exprType != type)
    {
        semanticErrorHandler->handle(new Error(
            node->getVariable()->getType()->getLineNum(),
            "Declared type: " + type +
                " of primitive for variable named: " + name +
                " does not match assigned expression type of: " + exprType +
                "."));
    }
}

void
TypeChecker::visit(ast::ListDefinition* node)
{
    std::string name = node->getName()->getValue();
    std::string type = node->getType()->getType();
    int lineNumber = node->getLineNum();

    nameIsLegal(name, lineNumber);

    std::string adjustedName = "L" + type;

    tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);
    typeExists(type, name, lineNumber);

    // visit expression
    visit(node->getExpression());

    std::string exprType =
        expressionHelper(node->getExpression(), symbolTable,
                         semanticErrorHandler, udtTable, curUDT);

    std::string baseType = exprType.substr(0, exprType.find("_"));
    std::string valType =
        exprType.substr(exprType.find("_") + 1, exprType.length());

    if (baseType != "list")
    {
        semanticErrorHandler->handle(new Error(
            node->getName()->getLineNum(),
            "Declared type of list for variable named: " + name +
                " does not match assigned expression type of: " + baseType +
                "."));
    }
    else if (valType != "empty" && valType != type)
    {
        semanticErrorHandler->handle(new Error(
            node->getType()->getLineNum(),
            "Declared type of list: " + type + " for variable named: " + name +
                " does not match assigned expression's list of type: " +
                valType + "."));
    }
}

void
TypeChecker::visit(ast::DictionaryDefinition* node)
{
    std::string name = node->getName()->getValue();
    std::string keyType = node->getKeyType()->getType();
    std::string valueType = node->getValueType()->getType();
    int lineNumber = node->getLineNum();

    nameIsLegal(name, lineNumber);

    std::string adjustedName = "D" + keyType + "_" + valueType;

    tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);
    typeExists(keyType, keyType, lineNumber);
    typeExists(valueType, valueType, lineNumber);

    // visit expression
    visit(node->getExpression());

    std::string exprType =
        expressionHelper(node->getExpression(), symbolTable,
                         semanticErrorHandler, udtTable, curUDT);

    std::string baseType = exprType.substr(0, exprType.find("_"));

    std::string temp =
        exprType.substr(exprType.find("_") + 1, exprType.length());

    std::string receivedKeyType = temp.substr(0, temp.find("_"));

    std::string receivedValType =
        temp.substr(temp.find("_") + 1, temp.length());

    if (baseType != "dictionary")
    {
        semanticErrorHandler->handle(new Error(
            node->getName()->getLineNum(),
            "Declared type of dictionary for variable named: " + name +
                " does not match assigned expression type of: " + baseType +
                "."));
    }
    else if (receivedKeyType != "empty" && receivedKeyType != keyType)
    {
        semanticErrorHandler->handle(
            new Error(node->getKeyType()->getLineNum(),
                      "Declared type of dictionary keys: " + keyType +
                          " for variable named: " + name +
                          " does not match assigned expression's "
                          "dictionary keys of type: " +
                          receivedKeyType + "."));
    }
    else if (receivedValType != "empty" && receivedValType != valueType)
    {
        semanticErrorHandler->handle(
            new Error(node->getValueType()->getLineNum(),
                      "Declared type of dictionary values: " + valueType +
                          " for variable named: " + name +
                          " does not match assigned expression's "
                          "dictionary values of type: " +
                          receivedValType + "."));
    }
}

void
TypeChecker::visit(ast::FunctionDefinition* node)
{
    // if we hit errors, do not add it to the symbol table
    bool isGood = true;
    std::string name = node->getName()->getValue();
    int lineNumber = node->getLineNum();

    nameIsLegal(name, lineNumber);

    std::string adjustedName = "F" + name + "{";

    // capture inputs to add to inner function scope
    // formatted: (name, type)
    std::vector<std::tuple<std::string, std::string>> inputsToAdd;

    //  capture for the addition of the function to the symbol table
    for (auto const& input : node->getInputList())
    {
        ast::Variable* var = input->getInput();
        std::string inp_type = var->getType()->getType();
        std::string inp_name = var->getName()->getValue();

        typeExists(inp_type, inp_name, lineNumber);

        // make sure the name is not a reserved word, a primitive name, or a
        // UDT, however edge case where is void exists
        if (inp_name != "void" &&
            (isPrimitive(inp_name) || isKeyword(inp_name) ||
             udtTable->hasUDT(inp_name)))
        {
            semanticErrorHandler->handle(
                new Error(node->getLineNum(),
                          "Declared function input named: " + inp_name +
                              " illegally shares its name with a "
                              "type or a keyword/reserved word."));
        }

        inputsToAdd.push_back(
            std::tuple<std::string, std::string>(inp_name, inp_type));

        adjustedName += "_" + inp_type;
    }

    adjustedName += "}(";

    // capture for the addition of the function to the symbol table
    ast::Output* output = node->getOutput();
    ast::Typename* var = output->getOutput();
    std::string out_type = var->getType();

    typeExists(out_type, out_type, lineNumber);

    adjustedName += "_" + out_type;
    adjustedName += ")";

    if (isGood)
    {
        tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);
    }

    // enter a scope for this function, starting with the parameters
    symbolTable->enterScope();

    // add parameters to the symbol table in the inner scope of the function
    // body

    for (std::tuple<std::string, std::string> const& input : inputsToAdd)
    {
        std::string name = std::get<0>(input);
        std::string type = std::get<1>(input);

        std::string adjustedName = "U" + type;
        if (isPrimitive(type))
        {
            std::string adjustedName = "P" + type;
        }

        symbolTable->addSymbol(name, adjustedName);
    }

    // visit the function body
    visit(node->getBody());

    bool hasReturn = false;
    ast::Block* body = node->getBody();
    for (ast::Statement* const& statement : body->getStatements())
    {
        if (statement->getStatementType() == ast::Statement::ReturnStatement)
        {

            if (out_type == "void")
            {
                semanticErrorHandler->handle(
                    new Error(node->getLineNum(),
                              "Unexpected return in function returning void."));
                break;
            }

            hasReturn = true;

            // ensure type is as expected
            ast::ReturnStatement* subnode =
                dynamic_cast<ast::ReturnStatement*>(statement);

            ast::BinaryExpression* returnedExpr =
                subnode->getBinaryExpression();

            std::string actualReturnType =
                getRightExpressionType(returnedExpr, symbolTable,
                                       semanticErrorHandler, udtTable, curUDT);

            if (actualReturnType != out_type)
            {
                semanticErrorHandler->handle(new Error(
                    node->getLineNum(),
                    "Actual return type of: " + actualReturnType +
                        " does not match expected return type of: " + out_type +
                        "."));
            }
        }
    }

    if (!hasReturn && out_type != "void")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Function does not have a return statement."));
    }

    // exit scope once we exit the body
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::UserDefinedTypeDefinition* node)
{
    std::vector<ast::Variable*> attributes = node->getAttributes();
    std::vector<ast::FunctionDefinition*> methods = node->getMethods();

    // capture the name
    std::string udt_name = node->getName()->getValue();
    int lineNumber = node->getLineNum();

    // hacks: REMOVE ASAP
    curUDT = udt_name;

    nameIsLegal(udt_name, lineNumber);

    // create a symbol table for the attributes
    SymbolTable* st_a = new SymbolTable();

    // temporarilly set the symbolTable field to the attribute one so that I can
    // properly create this
    SymbolTable* tempTable = symbolTable;
    symbolTable = st_a;

    for (auto const& var : attributes)
    {
        std::string name = var->getName()->getValue();
        std::string type = var->getType()->getType();
        int lineNumber = node->getLineNum();

        nameIsLegal(name, lineNumber);

        // ensure the type exists, i.e. is a primitive or udt and not a void,
        // makes no sense here
        if ((!isPrimitive(type) && !udtTable->hasUDT(type)) || (type == "void"))
        {
            semanticErrorHandler->handle(new Error(
                node->getLineNum(),
                "Udt attribute type of:  " + type + " for attribute: " + name +
                    " and for udt: " + udt_name + " does not exist."));
        }

        std::string adjustedName = "U" + type;
        if (isPrimitive(type))
        {
            std::string adjustedName = "P" + type;
        }

        tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);
    }

    // capture the decorated symbol table and reset the class field back
    st_a = symbolTable;
    symbolTable = tempTable;

    // create a symbol table for the methods
    SymbolTable* st_m = new SymbolTable();

    // temporarilly set the symbolTable field to the attribute one so that I can
    // properly create this
    tempTable = symbolTable;
    symbolTable = st_m;

    // capture the decorated symbol table and reset the class field back
    st_m = symbolTable;
    symbolTable = tempTable;

    // ADD BOTH BEFORE VISITING METHODS
    std::string adjustedName = "U" + udt_name;
    tryAddToSymbolTable(udt_name, adjustedName, symbolTable, lineNumber);
    bool isUnique = udtTable->addUDT(udt_name, st_a, st_m);

    // udt table entries MUST be unique unlike symbol table (for now)
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of udt with name: " + udt_name + "."));
    }

    for (auto const& func : methods)
    {
        visit(func);
    }

    // std::string adjustedName = "U" + udt_name;

    // tryAddToSymbolTable(udt_name, adjustedName, symbolTable, lineNumber);

    // bool isUnique = udtTable->addUDT(udt_name, st_a, st_m);
}

// ------- END ------- //

void
TypeChecker::visit(ast::InitialExecutionBody* node)
{
    // enter and exit scope for initial execution body
    symbolTable->enterScope();
    visit(node->getBody());
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::IfStatement* node)
{
    visit(node->getIfConditional());

    // enter and exit scope for if statement body
    symbolTable->enterScope();
    visit(node->getIfStatements());
    symbolTable->exitScope();

    // enter and exit scope for else statement body
    symbolTable->enterScope();
    visit(node->getElseStatements());
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::GroupingExpression* node)
{
    ast::BinaryExpression* binExpr = node->getBinaryExpressionList();

    visit(binExpr);

    if (!isLegalGrouping(binExpr))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected grouping to result in a boolean."));
    }
}

void
TypeChecker::visit(ast::Negation* node)
{
    visit(node->getBinaryExpression());

    // ensure that all negations are boolean
    std::string type =
        getRightExpressionType(node->getBinaryExpression(), symbolTable,
                               semanticErrorHandler, udtTable, curUDT);
    if (type != "bool")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected boolean type following negation. Instead received: " +
                type + "."));
    }
}

void
TypeChecker::visit(ast::BinaryExpression* node)
{
    // edge case where there is no right expression
    if (node->getBinaryExpressionType() ==
        ast::BinaryExpression::ExpressionOnlyStatement)
    {
        visit(node->getLeftExpr());
        return;
    }

    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), symbolTable, semanticErrorHandler,
                         udtTable, curUDT);

    std::string rType =
        getRightExpressionType(node->getRightExpr(), symbolTable,
                               semanticErrorHandler, udtTable, curUDT);

    switch (node->getBinaryExpressionType())
    {
    // only int AND int
    case ast::BinaryExpression::BinaryCompOrArith:
    {
        ast::BinaryCompOrArith* subnode =
            dynamic_cast<ast::BinaryCompOrArith*>(node);
        if (subnode->onlyAcceptsInt())
        {
            if (lType != "int")
            {
                semanticErrorHandler->handle(
                    new Error(0, "Expected int type on left "
                                 "side of operation. Instead received: " +
                                     lType + "."));
            }
            else if (rType != "int")
            {
                semanticErrorHandler->handle(
                    new Error(0, "Expected int type on left "
                                 "side of operation. Instead received: " +
                                     rType + "."));
            }
        }
        else if (subnode->onlyAcceptsBool())
        {
            if (lType != "bool")
            {
                semanticErrorHandler->handle(
                    new Error(0, "Expected boolean type on left side of "
                                 "operation. Instead received: " +
                                     lType + "."));
            }

            else if (rType != "bool")
            {
                semanticErrorHandler->handle(
                    new Error(0, "Expected boolean type on right side of "
                                 "operation. Instead received: " +
                                     rType + "."));
            }
        }
        else if (subnode->onlyAcceptsFltOrInt())
        {
            if (lType != rType)
            {
                semanticErrorHandler->handle(
                    new Error(0, "Expected the same types on each side of "
                                 "operation. Instead received: " +
                                     lType + " and " + rType + "."));
            }
            else if (lType != "int" && lType != "flt")
            {
                semanticErrorHandler->handle(new Error(
                    0, "Expected either float or integer type on left "
                       "side of operation. Instead received: " +
                           lType + "."));
            }
            else if (rType != "int" && rType != "flt")
            {
                semanticErrorHandler->handle(new Error(
                    0, "Expected either float or integer type on left "
                       "side of operation. Instead received: " +
                           rType + "."));
            }
        }
        else
        {
            if (lType != rType)
            {
                semanticErrorHandler->handle(
                    new Error(0, "Expected the same types on each side of "
                                 "operation. Instead received: " +
                                     lType + " and " + rType + "."));
            }
        }
        break;
    }

    // both sides same types with some restrictions on left hand and right hand
    // side
    case ast::BinaryExpression::Assignment:
    {
        ast::Expression* left = node->getLeftExpr();

        switch (left->getExpressionType())
        {
        case ast::Expression::PrimaryExpression:
        {
            ast::PrimaryExpression* subnode =
                dynamic_cast<ast::PrimaryExpression*>(left);

            switch (subnode->getPrimary()->getPrimaryType())
            {
            case ast::Primary::IdentifierLiteral:
            case ast::Primary::AttributeAccessLiteral:
            {
                ast::Identifier* z = dynamic_cast<ast::Identifier*>(subnode);

                break;
            }
            default:
            {
                semanticErrorHandler->handle(new Error(
                    0, "Illegal left hand expression in assignment."));
            }
            }
            break;
        }
        default:
        {
            semanticErrorHandler->handle(
                new Error(0, "Illegal left hand expression in assignment."));
        }
        }

        if (lType != rType)
        {
            semanticErrorHandler->handle(
                new Error(0, "Expected the same types on each side of "
                             "operation. Instead received: " +
                                 lType + " and " + rType + "."));
        }
        break;
    }
    }
}

void
TypeChecker::visit(ast::ExportDefinition* node)
{
    // ensure scope is global since functions cannot be in blocks
    if (!symbolTable->isGlobalScope())
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Export definitions can only be declared at the global level, "
            "i.e. cannot be nested!"));
    }

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
TypeChecker::visit(ast::AttributeAccess* node)
{
    std::string attributeName = node->getAttribute()->getValue();
    std::string variableName = node->getUDT()->getValue();

    // hacks: REMOVE ASAP
    if (variableName == "own")
    {
        variableName = curUDT;
    }
    else
    {
        // ensure variable exists
        if (!symbolTable->hasVariable(variableName))
        {
            semanticErrorHandler->handle(new Error(
                node->getLineNum(),
                "Attribute: " + attributeName +
                    " called on undeclared variable: " + variableName + "."));

            return;
        }

        std::string udtTypeFull = symbolTable->getSymbolType(variableName);
        variableName = udtTypeFull.substr(1, udtTypeFull.length());
    }

    // ensure udt exists
    if (!udtTable->hasUDT(variableName))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Attribute: " + attributeName +
                " called on nonexistent udt type: " + variableName + "."));

        return;
    }

    // ensure attribute exists for udt
    if (!udtTable->getAttributeSymbolTable(variableName)
             ->hasVariable(attributeName))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Attribute: " + attributeName +
                " does not exists for udt type: " + variableName + "."));

        return;
    }
}

void
TypeChecker::visit(ast::MethodAccess* node)
{
    std::string methodName = node->getName()->getValue();
    std::string variableUDTname = node->getUDT()->getValue();

    // hacks: REMOVE ASAP
    if (variableUDTname == "own")
    {
        variableUDTname = curUDT;
    }
    else
    {
        // ensure variable's udt exists
        if (!symbolTable->hasVariable(variableUDTname))
        {
            semanticErrorHandler->handle(new Error(
                node->getLineNum(), "Method: " + methodName +
                                        " called on nonexistent udt type: " +
                                        variableUDTname + "."));

            return;
        }

        std::string variableUDTname =
            symbolTable->getSymbolType(variableUDTname);

        // ensure variable's udt exists
        if (!udtTable->hasUDT(
                variableUDTname.substr(1, variableUDTname.length())))
        {
            semanticErrorHandler->handle(new Error(
                node->getLineNum(), "Method: " + methodName +
                                        " called on nonexistent udt type: " +
                                        variableUDTname + "."));

            return;
        }
    }

    std::string udtType = variableUDTname.substr(1, variableUDTname.length());

    // ensure metho exists for udt
    if (!udtTable->getMethodSymbolTable(udtType)->hasVariable(methodName))
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Method: " + methodName +
                          " does not exist for udt type: " + udtType + "."));
        // abort
        return;
    }

    // visit function call
    std::string fulltype =
        udtTable->getMethodSymbolTable(udtType)->getSymbolType(methodName);

    std::vector<std::string> inputs = getFunctionParamTypes(fulltype);

    // ensure that each of the arguments is supplied and of the proper type
    std::vector<ast::Primary*> args = node->getFunctionCall()->getArguments();

    compareFunctions(inputs, args, methodName, symbolTable,
                     semanticErrorHandler, udtTable, curUDT);

    for (auto const& arg : args)
    {
        visit(arg);
    }
}

void
TypeChecker::visit(ast::FunctionCall* node)
{
    std::string name = node->getName()->getValue();

    if (!symbolTable->hasVariable(name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Function: " + name + " is not defined."));
        return;
    }

    // parse out type info from symbol table form of type name
    int state = 0;
    std::vector<std::string> inputs =
        getFunctionParamTypes(symbolTable->getSymbolType(name));
    std::string buffer = "";

    // ensure that each of the arguments is supplied and of the proper type
    std::vector<ast::Primary*> args = node->getArguments();

    compareFunctions(inputs, args, name, symbolTable, semanticErrorHandler,
                     udtTable, curUDT);

    for (auto const& arg : args)
    {
        visit(arg);
    }
}
