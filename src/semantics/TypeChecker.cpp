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
void
TypeChecker::typeExists(std::string type, std::string name, int lineNumber)
{
    if (!isPrimitive(type) && !udtTable->hasUDT(type))
        semanticErrorHandler->handle(new Error(
            lineNumber, "Declared type: " + type + " for variable named: " +
                            name + " is not a legal or known type."));
}

// encapsulate addition to symbol table and error handling for it
void
TypeChecker::tryAddToSymbolTable(std::string name, std::string type,
                                 SymbolTable* symbolTable, int lineNumber)
{
    // symbol table entry must be unique for current scope
    if (!symbolTable->addSymbol(name, type))
        symbolTableErrorHandler->handle(new Error(
            lineNumber,
            "Invalid redecleration of variable with name: " + name + "."));
}

// encapsulate addition to symbol table and error handling for it for
// attributes, of whom all scope is one and thus we use scope here to mean
// ordering
void
TypeChecker::tryAddToSymbolTableIterative(std::string name, std::string type,
                                          SymbolTable* symbolTable,
                                          int lineNumber)
{
    // symbol table entry must be unique for current scope
    if (!symbolTable->addSymbolIterative(name, type))
        symbolTableErrorHandler->handle(new Error(
            lineNumber,
            "Invalid redecleration of variable with name: " + name + "."));
}

// given a function's full type from the symbol table, extract the parameter
// types
std::vector<std::string>
getFunctionParamTypes(std::string fulltype)
{
    std::vector<std::string> params;

    std::string buffer = "";
    for (int i = fulltype.find("{") + 2; i < fulltype.find("}"); i++)
    {
        char c = fulltype.at(i);
        if (c == '$' || c == '}')
        {
            params.push_back(buffer);
            buffer = "";
        }
        else
            buffer += c;
    }

    if (buffer != "")
        params.push_back(buffer);

    return params;
}

// given a function's full type from the symbol table, extract the return type
std::string
getFunctionReturnType(std::string fulltype)
{
    std::string buffer = "";
    for (int i = fulltype.find("(") + 2; i < fulltype.length(); i++)
    {
        char c = fulltype.at(i);
        if (c == '$' || c == ')')
            break;

        buffer += c;
    }

    return buffer;
}

// given the full type from the symbol table, determine the return type
std::string
truncateType(std::string fulltype)
{
    switch (fulltype.at(0))
    {
    case 'U':
    case 'P':
        return fulltype.substr(1, fulltype.length());
    case 'F':
        return getFunctionReturnType(fulltype);
    default:
        return "unknown";
    }
}

// given a primary, determine its type
std::string
TypeChecker::getType(ast::Expression* node)
{
    switch (node->getExpressionType())
    {
    case ast::Expression::NewExpression:
    {
        auto subnode = dynamic_cast<ast::NewExpression*>(node);
        auto newnode = subnode->getNewVal();

        switch (newnode->getNewType())
        {
        case ast::New::UserDefinedType:

            auto udt = dynamic_cast<ast::UserDefinedType*>(newnode);

            auto name = udt->getName()->getValue();

            auto fullTypeName = "udt$" + name;

            for (auto const& item : udt->getAttributes())
            {
                fullTypeName += "$" + item->getKey()->getValue() + "$" +
                                getType(item->getValue());
            }

            return fullTypeName;
        }
        break;
    }
    case ast::Expression::GroupingExpression:
        return "bool";
    case ast::Expression::PrimaryExpression:
    {
        auto subnode = dynamic_cast<ast::PrimaryExpression*>(node);

        return getType(subnode->getPrimary());
    }
    case ast::Expression::UnaryExpression:
        return "bool";
    }

    return "unknown";
}

// given a primary, determine the type
std::string
TypeChecker::getType(ast::Primary* primary)
{
    auto type = primary->getPrimaryType();

    switch (type)
    {
    case ast::Primary::IdentifierLiteral:
    {
        auto subnode = dynamic_cast<ast::Identifier*>(primary);
        auto variableName = subnode->getValue();

        if (!isPrimitive(variableName))
        {
            // ensure variable exists
            if (!symbolTable->hasVariable(variableName))
            {
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Variable: " + variableName + " is undefined."));

                return "unknown";
            }

            auto fullAttributeType = symbolTable->getSymbolType(variableName);

            return truncateType(fullAttributeType);
        }

        return variableName;
    }
    case ast::Primary::StringLiteral:
        return "str";
    case ast::Primary::BooleanLiteral:
        return "bool";
    case ast::Primary::IntegerLiteral:
        return "int";
    case ast::Primary::FloatLiteral:
        return "flt";
    case ast::Primary::AttributeAccessLiteral:
    {
        auto subsubnode = dynamic_cast<ast::AttributeAccess*>(primary);

        auto attributeName = subsubnode->getAttribute()->getValue();
        auto variableName = subsubnode->getUDT()->getValue();

        // hacks: REMOVE ASAP
        if (variableName == "own")
        {
            variableName = curUDT;
        }
        else
        {
            // ensure variable exists - double checked so no need for another
            // error message
            if (!symbolTable->hasVariable(variableName))
            {
                return "unknown";
            }

            auto udtTypeFull = symbolTable->getSymbolType(variableName);
            variableName = udtTypeFull.substr(1, udtTypeFull.length());
        }

        // ensure udt exists - double checked so no need for another
        // error message
        if (!udtTable->hasUDT(variableName))
            return "unknown";

        // ensure attribute exists for udt - double checked so only return error
        // once
        if (!udtTable->getAttributeSymbolTable(variableName)
                 ->hasVariable(attributeName))
            return "unknown";

        auto fullAttributeType = udtTable->getAttributeSymbolTable(variableName)
                                     ->getSymbolType(attributeName);

        return truncateType(fullAttributeType);
    }
    case ast::Primary::AttributeMethodAccessLiteral:
    {
        auto node = dynamic_cast<ast::AttributeMethodAccess*>(primary);

        // -------       first extract the attribute       ------- //
        auto subsubnode = node->getAttribute();

        auto attributeName = subsubnode->getAttribute()->getValue();
        auto variableName = subsubnode->getUDT()->getValue();

        // hacks: REMOVE ASAP
        if (variableName == "own")
        {
            variableName = curUDT;
        }
        else
        {
            // ensure variable exists - double checked so no need for another
            // error message
            if (!symbolTable->hasVariable(variableName))
            {
                return "unknown";
            }

            auto udtTypeFull = symbolTable->getSymbolType(variableName);
            variableName = udtTypeFull.substr(1, udtTypeFull.length());
        }

        // ensure udt exists - double checked so no need for another
        // error message
        if (!udtTable->hasUDT(variableName))
            return "unknown";

        // ensure attribute exists for udt - double checked so only return error
        // once
        if (!udtTable->getAttributeSymbolTable(variableName)
                 ->hasVariable(attributeName))
            return "unknown";

        // -------       now extract the method       ------- //
        auto methodName = node->getName()->getValue();
        auto variableUDTname = getType(node->getAttribute());

        // hacks: REMOVE ASAP
        if (variableUDTname == "own")
        {
            variableUDTname = curUDT;
        }
        else
        {
            // ensure variable's udt exists - double checked so no need for
            // another error message
            if (!udtTable->hasUDT(variableUDTname))
                return "unknown";
        }

        // ensure metho exists for udt - double checked so no need for another
        // error message
        if (!udtTable->getMethodSymbolTable(variableUDTname)
                 ->hasVariable(methodName))
            return "unknown";

        return truncateType(udtTable->getMethodSymbolTable(variableUDTname)
                                ->getSymbolType(methodName));
    }
    case ast::Primary::MethodAccessLiteral:
    {
        auto node = dynamic_cast<ast::MethodAccess*>(primary);
        auto methodName = node->getName()->getValue();
        auto variableUDTname = node->getUDT()->getValue();

        // hacks: REMOVE ASAP
        if (variableUDTname == "own")
        {
            variableUDTname = curUDT;
        }
        else
        {
            // ensure variable's udt exists - double checked so no need for
            // another error message
            if (!symbolTable->hasVariable(variableUDTname))
                return "unknown";

            std::string udtname = symbolTable->getSymbolType(variableUDTname);

            // ensure variable's udt exists - double checked so no need for
            // another error message
            if (!udtTable->hasUDT(udtname.substr(1, udtname.length())))
                return "unknown";

            variableUDTname = udtname.substr(1, udtname.length());
        }

        // ensure metho exists for udt - double checked so no need for another
        // error message
        if (!udtTable->getMethodSymbolTable(variableUDTname)
                 ->hasVariable(methodName))
        {
            return "unknown";
        }

        return truncateType(udtTable->getMethodSymbolTable(variableUDTname)
                                ->getSymbolType(methodName));
    }
    case ast::Primary::FunctionCallLiteral:
    {
        auto subsubnode = dynamic_cast<ast::FunctionCall*>(primary);

        auto functionName = subsubnode->getName()->getValue();

        // ensure function exists
        if (!symbolTable->hasVariable(functionName))
            return "unknown";

        // get function type from symbol table
        auto fullType = symbolTable->getSymbolType(functionName);

        return truncateType(fullType);
    }
    }

    return "unknown";
}

// given a grouping, determine if it is ultimately a boolean expression
bool
isLegalGrouping(ast::BinaryExpression* node)
{
    switch (node->getBinaryExpressionType())
    {
    case ast::BinaryExpression::Assignment:
    case ast::BinaryExpression::ExpressionOnlyStatement:
        return false;

    case ast::BinaryExpression::BinaryCompOrArith:
        auto subnode = dynamic_cast<ast::BinaryCompOrArith*>(node);
        return subnode->isComparison();
    }

    return false;
}

// given a primary, determine if its type is void
bool
isVoid(ast::Primary* node)
{
    if (node->getPrimaryType() == ast::Primary::IdentifierLiteral)
    {
        auto id = dynamic_cast<ast::Identifier*>(node);
        if (id->getValue() == "void")
            return true;
    }

    return false;
}

// takes the input lists of two functions, often the expected types (args) and
// the actual types (inputs) and determines if everything is kosher
void
TypeChecker::compareFunctions(std::vector<std::string> inputs,
                              std::vector<ast::Primary*> args, std::string name,
                              int lineNumber)
{
    int numArgs = args.size();
    if (numArgs == 1 && isVoid(args.at(0)))
        numArgs = 0;

    int numInps = inputs.size();
    if (numInps == 1 && inputs.at(0) == "void")
        numInps = 0;

    if (numArgs < numInps)
        semanticErrorHandler->handle(new Error(
            lineNumber, "Not enough args supplied to function: " + name + "."));

    else if (numArgs > numInps)
        semanticErrorHandler->handle(new Error(
            lineNumber, "Too many args supplied to function: " + name + "."));

    else
    {
        for (int i = 0; i < numArgs; i++)
        {
            auto arg = args.at(i);
            auto actual = getType(arg);

            // if not primitive, see if it is a variable in the symbol table
            if (!isPrimitive(actual))
            {
                if (udtTable->hasUDT(actual))
                {
                    // ok
                }
                else if (!symbolTable->hasVariable(actual))
                    semanticErrorHandler->handle(
                        new Error(lineNumber,
                                  "Undefined argument: " + actual +
                                      " supplied for function: " + name + "."));

                else
                {
                    auto fullActual = symbolTable->getSymbolType(actual);
                    auto identChar = fullActual.at(0);
                    switch (identChar)
                    {
                    case 'U':
                    case 'P':
                        actual = fullActual.substr(1, fullActual.length());
                        break;
                    case 'F':
                        semanticErrorHandler->handle(new Error(
                            0, "Illegal argument: " + actual +
                                   " supplied for function: " + name +
                                   ". Sorry, functions are not first order "
                                   ":("));

                        // to continue semantic analysis
                        actual = inputs[i];
                        break;
                    }
                }
            }

            if (actual != inputs[i])
                semanticErrorHandler->handle(new Error(
                    lineNumber, "Supplied argument type of: " + actual +
                                    " does not match expected type of: " +
                                    inputs[i] + "."));
        }
    }
}

// given a right expression, determin its type
std::string
TypeChecker::getType(ast::BinaryExpression* node)
{
    return getType(node->getLeftExpr());
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
    auto isGood = true;

    // capture variable information to add to the symbol table
    auto var = node->getVariable();
    auto name = var->getName()->getValue();
    auto type = var->getType()->getType();
    auto lineNumber = var->getLineNum();

    auto adjustedName = "U" + type;

    nameIsLegal(name, lineNumber);
    typeExists(type, name, lineNumber);
    tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);

    // enter into the symbol table for udt
    auto tempST = symbolTable;
    symbolTable = udtTable->getAttributeSymbolTable(type);

    // visit expression
    visit(node->getExpression());

    auto exprType = getType(node->getExpression());

    std::string temp;
    std::string prev;

    auto baseType = exprType.substr(0, exprType.find("$"));
    temp = exprType.substr(exprType.find("$") + 1, exprType.length());

    auto udtTypeName = temp.substr(0, temp.find("$"));
    temp = temp.substr(temp.find("$") + 1, temp.length());
    prev = udtTypeName;

    // make sure the type of the assignment is a udt
    if (baseType != "udt")
        semanticErrorHandler->handle(new Error(
            node->getExpression()->getLineNum(),
            "Expected udt type. Instead received: " + baseType + "."));

    // make sure the type name of the assigned udt matches
    else if (udtTypeName != type)
        semanticErrorHandler->handle(
            new Error(node->getExpression()->getLineNum(),
                      "Expected udt of type: " + type +
                          " and received: " + udtTypeName + "."));

    // ensure all attributes exist
    else
    {
        auto st = udtTable->getAttributeSymbolTable(udtTypeName);

        /**
         * To ensure that all the udt variables are in the proper ordering,
         * we compare this count below to the scope value, which, as noted
         * in the respective code, is the ordering for that attribute value.
         */
        int count = 0;
        while (temp != prev)
        {

            if (count + 1 > st->getCurrentScope())
            {
                semanticErrorHandler->handle(
                    new Error(node->getExpression()->getLineNum(),
                              "Too many arguments in udt initialization."));
                break;
            }

            auto varName = temp.substr(0, temp.find("$"));
            temp = temp.substr(temp.find("$") + 1, temp.length());

            auto actualType = temp.substr(0, temp.find("$"));
            temp = temp.substr(temp.find("$") + 1, temp.length());

            prev = actualType;

            if (!st->hasVariable(varName))
            {
                // var does not exist in method symbol table
                semanticErrorHandler->handle(new Error(
                    node->getExpression()->getLineNum(),
                    "Received unknown attribute name of: " + varName +
                        " in constructor of udt of type: " + udtTypeName +
                        "."));
            }
            else
            {
                auto expectedType = truncateType(st->getSymbolType(varName));

                if (actualType != "void" && actualType != expectedType)
                    // given var type does not match expected type
                    // determined by udt definition
                    semanticErrorHandler->handle(new Error(
                        node->getExpression()->getLineNum(),
                        "Received intiializer variable of type: " + actualType +
                            " in constructor of udt of type: " + udtTypeName +
                            " when a variable for type: " + expectedType +
                            " was expected for attribute named: " + varName +
                            "."));

                else
                {
                    int ordering = st->getSymbolScope(varName);
                    if (ordering != count)
                    {
                        semanticErrorHandler->handle(new Error(
                            node->getExpression()->getLineNum(),
                            "In udt inititialization, attributes must be "
                            "defined in the same ordering as the udt "
                            "definition. Received: " +
                                varName +
                                " at index: " + std::to_string(count) +
                                " and expected it at: " +
                                std::to_string(ordering) + "."));
                    }
                }
            }
            ++count;
        }

        if (count < st->getCurrentScope())
            semanticErrorHandler->handle(
                new Error(node->getExpression()->getLineNum(),
                          "Too few arguments in udt initialization."));
    }

    // exit the symbol table for udt
    symbolTable = tempST;
}

void
TypeChecker::visit(ast::PrimitiveDefition* node)
{
    auto name = node->getVariable()->getName()->getValue();
    auto type = node->getVariable()->getType()->getType();
    auto lineNumber = node->getLineNum();

    nameIsLegal(name, lineNumber);

    auto adjustedName = "P" + type;

    tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);

    // ensure the type is a primitive
    if (!isPrimitive(type))
        semanticErrorHandler->handle(new Error(
            node->getVariable()->getType()->getLineNum(),
            "Defined primitives's type of: " + type +
                " for primitive: " + name + " is not a recognized primitive."));

    // visit expression
    visit(node->getBinaryExpression());

    auto exprType = getType(node->getBinaryExpression());

    // ensure assignment is the expected type
    if (exprType != type)
        semanticErrorHandler->handle(new Error(
            node->getVariable()->getType()->getLineNum(),
            "Declared type: " + type +
                " of primitive for variable named: " + name +
                " does not match assigned expression type of: " + exprType +
                "."));
}

void
TypeChecker::visit(ast::FunctionDefinition* node)
{
    // if we hit errors, do not add it to the symbol table
    auto isGood = true;
    auto name = node->getName()->getValue();
    auto lineNumber = node->getLineNum();

    nameIsLegal(name, lineNumber);

    auto adjustedName = "F" + name + "{";

    // capture inputs to add to inner function scope
    // formatted: (name, type)
    std::vector<std::tuple<std::string, std::string>> inputsToAdd;

    //  capture for the addition of the function to the symbol table
    for (auto const& input : node->getInputList())
    {
        auto var = input->getInput();
        auto inp_type = var->getType()->getType();
        auto inp_name = var->getName()->getValue();

        typeExists(inp_type, inp_name, lineNumber);

        // make sure the name is not a reserved word, a primitive name, or a
        // UDT, however edge case where is void exists
        if (inp_name != "void" &&
            (isPrimitive(inp_name) || isKeyword(inp_name) ||
             udtTable->hasUDT(inp_name)))
        {
            semanticErrorHandler->handle(
                new Error(var->getLineNum(),
                          "Declared function input named: " + inp_name +
                              " illegally shares its name with a "
                              "type or a keyword/reserved word."));
        }

        inputsToAdd.push_back(
            std::tuple<std::string, std::string>(inp_name, inp_type));

        adjustedName += "$" + inp_type;
    }

    adjustedName += "}(";

    // capture for the addition of the function to the symbol table
    auto output = node->getOutput();
    auto var = output->getOutput();
    auto out_type = var->getType();

    typeExists(out_type, out_type, lineNumber);

    adjustedName += "$" + out_type;
    adjustedName += ")";

    if (isGood)
        tryAddToSymbolTable(name, adjustedName, symbolTable, lineNumber);

    // enter a scope for this function, starting with the parameters
    symbolTable->enterScope();

    // add parameters to the symbol table in the inner scope of the function
    // body

    for (std::tuple<std::string, std::string> const& input : inputsToAdd)
    {
        auto name = std::get<0>(input);
        auto type = std::get<1>(input);

        auto adjustedName = "U" + type;
        if (isPrimitive(type))
            adjustedName = "P" + type;

        symbolTable->addSymbol(name, adjustedName);
    }

    // visit the function body
    visit(node->getBody());

    auto hasReturn = false;
    auto body = node->getBody();
    for (ast::Statement* const& statement : body->getStatements())
    {
        if (statement->getStatementType() == ast::Statement::ReturnStatement)
        {

            if (out_type == "void")
            {
                semanticErrorHandler->handle(
                    new Error(var->getLineNum(),
                              "Unexpected return in function returning void."));
                break;
            }

            hasReturn = true;

            // ensure type is as expected
            auto subnode = dynamic_cast<ast::ReturnStatement*>(statement);

            auto returnedExpr = subnode->getBinaryExpression();

            auto actualReturnType = getType(returnedExpr);

            if (actualReturnType != out_type)
                semanticErrorHandler->handle(new Error(
                    subnode->getLineNum(),
                    "Actual return type of: " + actualReturnType +
                        " does not match expected return type of: " + out_type +
                        "."));
        }
    }

    if (!hasReturn && out_type != "void")
        semanticErrorHandler->handle(new Error(
            body->getLineNum(), "Function does not have a return statement."));

    // exit scope once we exit the body
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::UserDefinedTypeDefinition* node)
{
    auto attributes = node->getAttributes();
    auto methods = node->getMethods();

    // capture the name
    auto udt_name = node->getName()->getValue();
    auto lineNumber = node->getLineNum();

    // hacks: REMOVE ASAP
    curUDT = udt_name;

    nameIsLegal(udt_name, lineNumber);

    // create a symbol table for the attributes
    auto st_a = new SymbolTable();

    // create a symbol table for the methods
    auto st_m = new SymbolTable();

    // temporarilly set the symbolTable field to the attribute one so that I
    // can properly create this
    auto tempTable = symbolTable;
    symbolTable = st_a;

    auto isUnique = udtTable->addUDT(udt_name, st_a, st_m);

    for (auto const& var : attributes)
    {
        auto name = var->getName()->getValue();
        auto type = var->getType()->getType();
        auto lineNumber = node->getLineNum();

        nameIsLegal(name, lineNumber);

        // ensure the type exists, i.e. is a primitive or udt and not a
        // void, makes no sense here
        if ((!isPrimitive(type) && !udtTable->hasUDT(type)) || (type == "void"))
            semanticErrorHandler->handle(new Error(
                var->getLineNum(),
                "Udt attribute type of:  " + type + " for attribute: " + name +
                    " and for udt: " + udt_name + " does not exist."));

        auto adjustedName = "U" + type;
        if (isPrimitive(type))
            auto adjustedName = "P" + type;

        tryAddToSymbolTableIterative(name, adjustedName, symbolTable,
                                     lineNumber);
    }

    // capture the decorated symbol table and reset the class field back
    st_a = symbolTable;
    symbolTable = tempTable;

    // temporarilly set the symbolTable field to the attribute one so that I
    // can properly create this
    tempTable = symbolTable;
    symbolTable = st_m;

    // ADD BOTH BEFORE VISITING METHODS
    auto adjustedName = "U" + udt_name;
    tryAddToSymbolTable(udt_name, adjustedName, symbolTable, lineNumber);
    auto couldUpdate = udtTable->updateUDT(udt_name, st_a, st_m);

    // udt table entries MUST be unique unlike symbol table (for now)
    if (!isUnique)
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of udt with name: " + udt_name + "."));

    for (auto const& func : methods)
        visit(func);

    // capture the decorated symbol table and reset the class field back
    st_m = symbolTable;
    symbolTable = tempTable;

    couldUpdate = udtTable->updateUDT(udt_name, st_a, st_m);

    // udt table entries MUST be unique unlike symbol table (for now)
    if (!couldUpdate)
        symbolTableErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Attempted to update an unrecognized/undefined udt."));
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
    auto binExpr = node->getBinaryExpressionList();

    visit(binExpr);

    if (!isLegalGrouping(binExpr))
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected grouping to result in a boolean."));
}

void
TypeChecker::visit(ast::Negation* node)
{
    visit(node->getBinaryExpression());

    // ensure that all negations are boolean
    auto type = getType(node->getBinaryExpression());
    if (type != "bool")
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected boolean type following negation. Instead received: " +
                type + "."));
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

    auto lType = getType(node->getLeftExpr());

    auto rType = getType(node->getRightExpr());

    switch (node->getBinaryExpressionType())
    {
    // only int AND int
    case ast::BinaryExpression::BinaryCompOrArith:
    {
        auto subnode = dynamic_cast<ast::BinaryCompOrArith*>(node);
        if (subnode->onlyAcceptsInt())
        {
            if (lType != "int")
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected int type on left "
                              "side of operation. Instead received: " +
                                  lType + "."));
            else if (rType != "int")
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected int type on left "
                              "side of operation. Instead received: " +
                                  rType + "."));
        }
        else if (subnode->onlyAcceptsBool())
        {
            if (lType != "bool")
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected boolean type on left side of "
                              "operation. Instead received: " +
                                  lType + "."));

            else if (rType != "bool")
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected boolean type on right side of "
                              "operation. Instead received: " +
                                  rType + "."));
        }
        else if (subnode->onlyAcceptsFltOrInt())
        {
            if (lType != rType)
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected the same types on each side of "
                              "operation. Instead received: " +
                                  lType + " and " + rType + "."));

            else if (lType != "int" && lType != "flt")
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected either float or integer type on left "
                              "side of operation. Instead received: " +
                                  lType + "."));

            else if (rType != "int" && rType != "flt")
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected either float or integer type on left "
                              "side of operation. Instead received: " +
                                  rType + "."));
        }
        else
        {
            if (lType == "void" && udtTable->hasUDT(rType) ||
                rType == "void" && udtTable->hasUDT(lType) || lType == rType)
            {
            }
            else
            {
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Expected the same types on each side of "
                              "operation. Instead received: " +
                                  lType + " and " + rType + "."));
            }
        }
        break;
    }

    // both sides same types with some restrictions on left hand and right
    // hand side
    case ast::BinaryExpression::Assignment:
    {
        auto left = node->getLeftExpr();

        switch (left->getExpressionType())
        {
        case ast::Expression::PrimaryExpression:
        {
            auto subnode = dynamic_cast<ast::PrimaryExpression*>(left);

            switch (subnode->getPrimary()->getPrimaryType())
            {
            case ast::Primary::IdentifierLiteral:
            case ast::Primary::AttributeAccessLiteral:
            {
                ast::Identifier* z = dynamic_cast<ast::Identifier*>(subnode);
                break;
            }
            default:
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Illegal left hand expression in assignment."));
            }
            break;
        }
        default:
            semanticErrorHandler->handle(
                new Error(0, "Illegal left hand expression in assignment."));
        }

        if (lType != rType)
            semanticErrorHandler->handle(
                new Error(0, "Expected the same types on each side of "
                             "operation. Instead received: " +
                                 lType + " and " + rType + "."));
        break;
    }
    }
}

void
TypeChecker::visit(ast::AttributeAccess* node)
{
    auto attributeName = node->getAttribute()->getValue();
    auto variableName = node->getUDT()->getValue();

    // hacks: REMOVE ASAP
    if (variableName == "own")
        variableName = curUDT;

    else
    {
        // ensure variable exists
        if (!symbolTable->hasVariable(variableName))
        {
            semanticErrorHandler->handle(new Error(
                node->getUDT()->getLineNum(),
                "Attribute: " + attributeName +
                    " called on undeclared variable: " + variableName + "."));

            return;
        }

        auto udtTypeFull = symbolTable->getSymbolType(variableName);
        variableName = udtTypeFull.substr(1, udtTypeFull.length());
    }

    // ensure udt exists
    if (!udtTable->hasUDT(variableName))
    {
        semanticErrorHandler->handle(new Error(
            node->getUDT()->getLineNum(),
            "Attribute: " + attributeName +
                " called on nonexistent udt type: " + variableName + "."));

        return;
    }

    // ensure attribute exists for udt
    if (!udtTable->getAttributeSymbolTable(variableName)
             ->hasVariable(attributeName))
    {
        semanticErrorHandler->handle(new Error(
            node->getAttribute()->getLineNum(),
            "Attribute: " + attributeName +
                " does not exists for udt type: " + variableName + "."));

        return;
    }
}

void
TypeChecker::visit(ast::AttributeMethodAccess* node)
{
    visit(node->getAttribute());

    auto methodName = node->getName()->getValue();
    auto variableUDTname = getType(node->getAttribute());

    // hacks: REMOVE ASAP
    if (variableUDTname == "own")
        variableUDTname = curUDT;

    else
    {
        // ensure variable's udt exists
        if (!udtTable->hasUDT(variableUDTname))
        {
            semanticErrorHandler->handle(new Error(
                node->getUDT()->getLineNum(),
                "Method: " + methodName + " called on nonexistent udt type: " +
                    variableUDTname + "."));

            return;
        }
    }

    auto udtType = variableUDTname;

    // ensure metho exists for udt
    if (!udtTable->getMethodSymbolTable(udtType)->hasVariable(methodName))
    {
        semanticErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Method: " + methodName +
                          " does not exist for udt type: " + udtType + "."));
        // abort
        return;
    }

    // visit function call
    auto fulltype =
        udtTable->getMethodSymbolTable(udtType)->getSymbolType(methodName);

    auto inputs = getFunctionParamTypes(fulltype);

    // ensure that each of the arguments is supplied and of the proper type
    auto args = node->getFunctionCall()->getArguments();

    compareFunctions(inputs, args, methodName,
                     node->getFunctionCall()->getLineNum());

    for (auto const& arg : args)
        visit(arg);
}

void
TypeChecker::visit(ast::MethodAccess* node)
{
    auto methodName = node->getName()->getValue();
    auto variableUDTname = node->getUDT()->getValue();

    // hacks: REMOVE ASAP
    if (variableUDTname == "own")
        variableUDTname = curUDT;

    else
    {
        // ensure variable's udt exists
        if (!symbolTable->hasVariable(variableUDTname))
        {
            semanticErrorHandler->handle(new Error(
                node->getUDT()->getLineNum(),
                "Method: " + methodName + " called on nonexistent udt type: " +
                    variableUDTname + "."));

            return;
        }

        auto udtname = symbolTable->getSymbolType(variableUDTname);
        udtname = udtname.substr(1, udtname.length());

        // ensure variable's udt exists
        if (!udtTable->hasUDT(udtname))
        {
            semanticErrorHandler->handle(new Error(
                node->getUDT()->getLineNum(),
                "Method: " + methodName +
                    " called on nonexistent udt type: " + udtname + "."));

            return;
        }

        variableUDTname = udtname;
    }

    auto udtType = variableUDTname;

    // ensure metho exists for udt
    if (!udtTable->getMethodSymbolTable(udtType)->hasVariable(methodName))
    {
        semanticErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Method: " + methodName +
                          " does not exist for udt type: " + udtType + "."));
        // abort
        return;
    }

    // visit function call
    auto fulltype =
        udtTable->getMethodSymbolTable(udtType)->getSymbolType(methodName);

    auto inputs = getFunctionParamTypes(fulltype);

    // ensure that each of the arguments is supplied and of the proper type
    auto args = node->getFunctionCall()->getArguments();

    compareFunctions(inputs, args, methodName,
                     node->getFunctionCall()->getLineNum());

    for (auto const& arg : args)
        visit(arg);
}

void
TypeChecker::visit(ast::FunctionCall* node)
{
    auto name = node->getName()->getValue();

    if (!symbolTable->hasVariable(name))
    {
        semanticErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Function: " + name + " is not defined."));
        return;
    }

    // parse out type info from symbol table form of type name
    auto state = 0;
    auto inputs = getFunctionParamTypes(symbolTable->getSymbolType(name));
    auto buffer = "";

    // ensure that each of the arguments is supplied and of the proper type
    auto args = node->getArguments();

    compareFunctions(inputs, args, name, node->getLineNum());

    for (auto const& arg : args)
        visit(arg);
}
