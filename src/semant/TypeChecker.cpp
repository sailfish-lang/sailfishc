/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "TypeChecker.h"

// ------- Ugly/Hacky Helper Functions ------- //

// ------- END ------- //

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

    std::string adjustedName = "U" + type;

    // make sure the name is not a reserved word, a primitive name, or a UDT
    if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared udt named: " + name +
                                    " illegally shares its name with a "
                                    "type or a keyword/reserved word."));
    }

    // make sure the type is either primitive or a udt
    if (!isPrimitive(type) && !udtTable->hasUDT(type))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared udt type: " + type +
                                    " for variable named: " + name +
                                    " is not a legal or known type."));
    }

    bool isUnique = symbolTable->addSymbol(name, adjustedName);

    // symbol table entry must be unique for current scope
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Invalid redecleration of udt with name: " + name + "."));
    }

    // enter into the symbol table for udt
    SymbolTable* tempST = symbolTable;
    symbolTable = udtTable->getAttributeSymbolTable(type);

    // visit expression
    visit(node->getExpression());

    std::string exprType = expressionHelper(node->getExpression(), symbolTable,
                                            semanticErrorHandler, udtTable);

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

    // make sure the name is not a reserved word, a primitive name, or a UDT
    if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared list named: " + name +
                                    " illegally shares its name with a "
                                    "type or a keyword/reserved word."));
    }

    std::string adjustedName = "P" + type;

    bool isUnique = symbolTable->addSymbol(name, adjustedName);

    // symbol table entry must be unique for current scope
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of list with name: " + name + "."));
    }

    // ensure the type is a primitive
    if (!isPrimitive(type))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Defined primitives's type of: " + type +
                                    " for primitive: " + name +
                                    " is not a recognized primitive."));
    }

    // visit expression
    visit(node->getBinaryExpression());

    std::string exprType =
        getRightExpressionType(node->getBinaryExpression(), symbolTable,
                               semanticErrorHandler, udtTable);

    // ensure assignment is the expected type
    if (exprType != type)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
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

    // make sure the name is not a reserved word, a primitive name, or a UDT
    if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared primitive named: " + name +
                                    " illegally shares its name with a "
                                    "type or a keyword/reserved word."));
    }

    std::string adjustedName = "L" + type;

    bool isUnique = symbolTable->addSymbol(name, adjustedName);

    // symbol table entry must be unique for current scope
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of list with name: " + name + "."));
    }

    // ensure the type exists, i.e. is a primitive or udt
    if (!isPrimitive(type) && !udtTable->hasUDT(type))
    {
        semanticErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Defined lists's type of: " + type +
                          " for list: " + name + " does not exist."));
    }

    // visit expression
    visit(node->getExpression());

    std::string exprType = expressionHelper(node->getExpression(), symbolTable,
                                            semanticErrorHandler, udtTable);

    std::string baseType = exprType.substr(0, exprType.find("_"));
    std::string valType =
        exprType.substr(exprType.find("_") + 1, exprType.length());

    if (baseType != "list")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Declared type of list for variable named: " + name +
                " does not match assigned expression type of: " + baseType +
                "."));
    }
    else if (valType != "empty" && valType != type)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
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

    // make sure the name is not a reserved word, a primitive name, or a UDT
    if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared definition named: " + name +
                                    " illegally shares its name with a "
                                    "type or a keyword/reserved word."));
    }

    std::string adjustedName = "D" + keyType + "_" + valueType;

    bool isUnique = symbolTable->addSymbol(name, adjustedName);

    // symbol table entry must be unique for current scope
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of dictionary with name: " + name + "."));
    }

    // ensure the keyType exists, i.e. is a primitive or udt
    if (!isPrimitive(keyType) && !udtTable->hasUDT(keyType))
    {
        semanticErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Defined dictionary's key type of: " + keyType +
                          " for dictionary: " + name + " does not exist."));
    }

    // ensure the valueType exists, i.e. is a primitive or udt
    if (!isPrimitive(valueType) && !udtTable->hasUDT(valueType))
    {
        semanticErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Defined dictionary's value type of: " + valueType +
                          " for dictionary: " + name + " does not exist."));
    }

    // visit expression
    visit(node->getExpression());

    std::string exprType = expressionHelper(node->getExpression(), symbolTable,
                                            semanticErrorHandler, udtTable);

    std::string baseType = exprType.substr(0, exprType.find("_"));

    std::string temp =
        exprType.substr(exprType.find("_") + 1, exprType.length());

    std::string receivedKeyType = temp.substr(0, temp.find("_"));

    std::string receivedValType =
        temp.substr(temp.find("_") + 1, temp.length());

    if (baseType != "dictionary")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Declared type of dictionary for variable named: " + name +
                " does not match assigned expression type of: " + baseType +
                "."));
    }
    else if (receivedKeyType != "empty" && receivedKeyType != keyType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared type of dictionary keys: " + keyType +
                                    " for variable named: " + name +
                                    " does not match assigned expression's "
                                    "dictionary keys of type: " +
                                    receivedKeyType + "."));
    }
    else if (receivedValType != "empty" && receivedValType != valueType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared type of dictionary values: " +
                                    valueType + " for variable named: " + name +
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

    // ensure scope is global since functions cannot be in blocks
    if (!symbolTable->isGlobalScope())
    {
        symbolTableErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Functions can only be declared at the global level, "
                      "i.e. cannot be nested!"));
    }

    std::string name = node->getName()->getValue();

    // make sure the name is not a reserved word, a primitive name, or a UDT
    if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared function named: " + name +
                                    " illegally shares its name with a "
                                    "type or a keyword/reserved word."));
    }

    std::string adjustedName = "F" + name + "{";

    //  capture for the addition of the function to the symbol table
    for (auto const& input : node->getInputList())
    {
        ast::Variable* var = input->getInput();
        std::string inp_type = var->getType()->getType();
        std::string inp_name = var->getName()->getValue();

        // ensure the type exists, i.e. is a primitive or udt
        if (!isPrimitive(inp_type) && !udtTable->hasUDT(inp_type))
        {
            semanticErrorHandler->handle(
                new Error(node->getName()->getLineNum(),
                          "Functions input type of:  " + inp_type +
                              " for function: " + name + " does not exist."));
        }

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

        adjustedName += "_" + inp_type;
    }

    adjustedName += "}(";

    // capture for the addition of the function to the symbol table
    for (auto const& output : node->getOutputList())
    {
        ast::Typename* var = output->getOutput();
        std::string out_type = var->getType();

        if (!isPrimitive(out_type) && !symbolTable->hasVariable(out_type))
        {
            semanticErrorHandler->handle(
                new Error(node->getName()->getLineNum(),
                          "Functions output type of:  " + out_type +
                              " for function: " + name + " does not exist."));
        }

        adjustedName += "_" + out_type;
    }

    adjustedName += ")";

    if (isGood)
    {
        bool isUnique = symbolTable->addSymbol(name, adjustedName);

        // symbol table entry must be unique for current scope
        if (!isUnique)
        {
            symbolTableErrorHandler->handle(new Error(
                node->getLineNum(),
                "Invalid redecleration of function with name: " + name + "."));
        }
    }

    // enter a scope for this function, starting with the parameters
    symbolTable->enterScope();

    // visit the function body
    visit(node->getBody());

    // exit scope once we exit the body
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::UserDefinedTypeDefinition* node)
{

    // ensure scope is global since functions cannot be in blocks
    if (!symbolTable->isGlobalScope())
    {
        symbolTableErrorHandler->handle(
            new Error(node->getAttributes()->getName()->getLineNum(),
                      "UDT's can only be declared at the global level, "
                      "i.e. cannot be nested!"));
    }

    ast::UserDefinedTypeAttributes* attributes = node->getAttributes();
    ast::UserDefinedTypeMethods* methods = node->getMethods();

    // capture the name
    std::string udt_name = attributes->getName()->getValue();

    // make sure the name is not a reserved word, a primitive name, or a UDT
    if (isPrimitive(udt_name) || isKeyword(udt_name) ||
        udtTable->hasUDT(udt_name))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Declared udt named: " + udt_name +
                                    " illegally shares its name with a "
                                    "type or a keyword/reserved word."));
    }

    // create a symbol table for the attributes
    SymbolTable* st_a = new SymbolTable();

    // temporarilly set the symbolTable field to the attribute one so that I can
    // properly create this
    SymbolTable* tempTable = symbolTable;
    symbolTable = st_a;

    for (auto const& var : attributes->getAttributes())
    {
        std::string name = var->getName()->getValue();
        std::string type = var->getType()->getType();

        // make sure the name is not a reserved word, a primitive name, or a UDT
        if (isPrimitive(name) || isKeyword(name) || udtTable->hasUDT(name))
        {
            semanticErrorHandler->handle(new Error(
                node->getLineNum(), "Declared udt attribute named: " + name +
                                        " illegally shares its name with a "
                                        "type or a keyword/reserved word."));
        }

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

        bool isUnique = symbolTable->addSymbol(name, adjustedName);

        // symbol table entry must be unique for current scope
        if (!isUnique)
        {
            symbolTableErrorHandler->handle(new Error(
                node->getLineNum(),
                "Invalid redecleration of method with name: " + name + "."));
        }
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

    for (auto const& func : methods->getMethods())
    {
        visit(func);
    }

    // capture the decorated symbol table and reset the class field back
    st_m = symbolTable;
    symbolTable = tempTable;

    std::string adjustedName = "U" + udt_name;

    // add to both symbol table and udt table
    bool isUnique = symbolTable->addSymbol(udt_name, adjustedName);

    // symbol table entry must be unique for current scope
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of udt with name: " + udt_name + "."));
    }

    isUnique = udtTable->addUDT(udt_name, st_a, st_m);

    // udt table entries MUST be unique unlike symbol table (for now)
    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getLineNum(),
            "Invalid redecleration of udt with name: " + udt_name + "."));
    }
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
TypeChecker::visit(ast::Negation* node)
{
    visit(node->getBinaryExpression());

    // ensure that all negations are boolean
    std::string type =
        getRightExpressionType(node->getBinaryExpression(), symbolTable,
                               semanticErrorHandler, udtTable);
    if (type != "Boolean")
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

    std::string lType = expressionHelper(node->getLeftExpr(), symbolTable,
                                         semanticErrorHandler, udtTable);
    std::string rType = getRightExpressionType(
        node->getRightExpr(), symbolTable, semanticErrorHandler, udtTable);

    switch (node->getBinaryExpressionType())
    {
    // only int AND int
    case ast::BinaryExpression::Modulo:
    {
        if (lType != "int")
        {
            semanticErrorHandler->handle(
                new Error(0, "int type on left "
                             "side of modulo. Instead received: " +
                                 lType + "."));
        }
        else if (rType != "int")
        {
            semanticErrorHandler->handle(
                new Error(0, "int type on left "
                             "side of modulo. Instead received: " +
                                 rType + "."));
        }
        break;
    }
    // only int AND int or flt AND flt
    case ast::BinaryExpression::Addition:
    case ast::BinaryExpression::Subtraction:
    case ast::BinaryExpression::Exponentiation:
    case ast::BinaryExpression::Multiplication:
    case ast::BinaryExpression::Division:
    case ast::BinaryExpression::BinaryLessThan:
    case ast::BinaryExpression::BinaryLessThanOrEqual:
    case ast::BinaryExpression::BinaryGreaterThanOrEqual:
    case ast::BinaryExpression::BinaryGreaterThan:
    {
        if (lType != rType)
        {
            semanticErrorHandler->handle(new Error(
                0, "Expected the same types on each side of greater than "
                   "comparison. Instead recevied: " +
                       lType + " and " + rType + "."));
        }
        else if (lType != "int" && lType != "flt")
        {
            semanticErrorHandler->handle(
                new Error(0, "Expected either float or integer type on left "
                             "side of greater than "
                             "comparison. Instead received: " +
                                 lType + "."));
        }
        else if (rType != "int" && rType != "flt")
        {
            semanticErrorHandler->handle(
                new Error(0, "Expected either float or integer type on left "
                             "side of greater than "
                             "comparison. Instead received: " +
                                 rType + "."));
        }
        break;
    }

    // both sides same type
    case ast::BinaryExpression::EquivalenceComparison:
    case ast::BinaryExpression::NonEquivalenceComparison:
    case ast::BinaryExpression::Assignment:
    {
        if (lType != rType)
        {
            semanticErrorHandler->handle(new Error(
                0, "Expected the same types on each side of equivalence "
                   "comparison. Instead recevied: " +
                       lType + " and " + rType + "."));
        }
        break;
    }

    // both side boolean
    case ast::BinaryExpression::AndComparison:
    case ast::BinaryExpression::OrComparison:
    {
        if (lType != "Boolean")
        {
            semanticErrorHandler->handle(
                new Error(0, "Expected boolean type on left side of and "
                             "comparison. Instead received: " +
                                 lType + "."));
        }

        else if (rType != "Boolean")
        {
            semanticErrorHandler->handle(
                new Error(0, "Expected boolean type on right side of and "
                             "comparison. Instead received: " +
                                 rType + "."));
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

    // ensure variable exists
    if (!symbolTable->hasVariable(variableName))
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Attribue: " + attributeName +
                " called on undeclared variable: " + variableName + "."));
    }

    std::string udtTypeFull = symbolTable->getSymbolType(variableName);
    std::string udtType = udtTypeFull.substr(1, udtTypeFull.length());

    // ensure udt exists
    if (!udtTable->hasUDT(udtType))
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Attribue: " + attributeName +
                          " called on nonexistent udt type: " + udtType + "."));

        return;
    }

    // ensure attribute exists for udt
    if (!udtTable->getAttributeSymbolTable(udtType)->hasVariable(attributeName))
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Attribue: " + attributeName +
                          " does not exists for udt type: " + udtType + "."));
    }
}

void
TypeChecker::visit(ast::MethodAccess* node)
{
    std::string methodName = node->getName()->getValue();
    std::string udtname = node->getUDT()->getValue();

    // ensure udt exists
    if (!udtTable->hasUDT(udtname))
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Method: " + methodName +
                          " called on nonexistent udt type: " + udtname + "."));

        return;
    }

    std::string udtTypeFull = symbolTable->getSymbolType(udtname);
    std::string udtType = udtTypeFull.substr(1, udtTypeFull.length());

    // ensure metho exists for udt
    if (!udtTable->getMethodSymbolTable(udtType)->hasVariable(methodName))
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Method: " + methodName +
                          " does not exist for udt type: " + udtType + "."));
    }
}

void
TypeChecker::visit(ast::FunctionCall* node)
{
    std::string name = node->getName()->getValue();

    // ensure variable exists in symbol tabel
    if (!symbolTable->hasVariable(name))
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Function named: " + name + " has not been defined."));

        // just bail
        return;
    }

    // parse out type info from symbol table form of type name
    int state = 0;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::string buffer = "";
    for (auto c : symbolTable->getSymbolType(name))
    {
        switch (state)
        {
        case 0:
        {
            if (c == '{')
            {
                ++state;
            }
            break;
        }
        case 1:
        {
            if (c == '}')
            {
                if (buffer != "")
                {
                    inputs.push_back(buffer);
                }

                buffer = "";
                ++state;
            }
            else if (c == '_')
            {
                if (buffer != "")
                {
                    inputs.push_back(buffer);
                }

                buffer = "";
            }
            else
            {
                buffer += c;
            }
            break;
        }
        case 2:
        {
            if (c == '(')
            {
                // ignore this
            }
            else if (c == ')')
            {
                if (buffer != "")
                {
                    outputs.push_back(buffer);
                }

                buffer = "";
                ++state;
            }
            else if (c == '_')
            {
                if (buffer != "")
                {
                    outputs.push_back(buffer);
                }

                buffer = "";
            }
            else
            {
                buffer += c;
            }
            break;
        }
        }
    }

    // ensure that each of the arguments is supplied and of the proper type
    std::vector<ast::Primary*> args = node->getArguments();

    int numArgs = args.size();
    int numInps = inputs.size();

    std::cout << numArgs << " " << numInps << "\n";

    if (numArgs < numInps)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Not enough args supplied to function."));
    }
    else if (numArgs > numInps)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Too many args supplied to function."));
    }
    else
    {
        for (int i = 0; i < numArgs; i++)
        {
            ast::Primary* arg = args.at(i);
            std::string actual =
                primaryHelper(arg, symbolTable, semanticErrorHandler, udtTable);

            // if not primitive, see if it is a variable in the symbol table
            if (!isPrimitive(actual))
            {
                if (!symbolTable->hasVariable(actual))
                {
                    semanticErrorHandler->handle(
                        new Error(node->getLineNum(),
                                  "Undefined argument: " + actual +
                                      " supplied for function: " + name + "."));
                }
                else
                {
                    std::string fullActual = symbolTable->getSymbolType(actual);
                    char identChar = fullActual.at(0);
                    switch (identChar)
                    {
                    case 'U':
                    case 'P':
                        actual = fullActual.substr(1, fullActual.length());
                        break;
                    case 'F':
                        semanticErrorHandler->handle(new Error(
                            node->getLineNum(),
                            "Illegal argument: " + actual +
                                " supplied for function: " + name +
                                ". Sorry, functions are not first order :("));

                        // to continue semantic analysis
                        actual = inputs[i];
                        break;
                    case 'L':
                        new Error(node->getLineNum(),
                                  "Illegal argument: " + actual +
                                      " supplied for function: " + name +
                                      ". Lists cannot be passed to functions");

                        // to continue semantic analysis
                        actual = inputs[i];
                        break;
                    case 'D':
                        new Error(
                            node->getLineNum(),
                            "Illegal argument: " + actual +
                                " supplied for function: " + name +
                                ". Dictionaries cannot be passed to functions");

                        // to continue semantic analysis
                        actual = inputs[i];
                        break;
                    }
                }
            }

            if (actual != inputs[i])
            {
                semanticErrorHandler->handle(new Error(
                    node->getLineNum(),
                    "Supplied argument type of: " + actual +
                        " does not match expected type of: " + inputs[i] +
                        "."));
            }
        }
    }

    // visit(name);

    for (auto const& arg : args)
    {
        visit(arg);
    }
}
