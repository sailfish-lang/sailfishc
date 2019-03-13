#include "CommonExtractions.h"

ast::Expression*
binaryExpressionHelper(ast::BinaryExpression* node)
{
    return node->getLeftExpr();
}

std::vector<std::string>
getFunctionParamTypes(std::string fulltype)
{
    std::vector<std::string> params;

    std::string buffer = "";
    for (int i = fulltype.find("{") + 2; i < fulltype.find("}"); i++)
    {
        char c = fulltype.at(i);
        if (c == '_' || c == '}')
        {
            params.push_back(buffer);
            buffer = "";
        }
        else
        {

            buffer += c;
        }
    }

    if (buffer != "")
    {
        params.push_back(buffer);
    }

    return params;
}

std::string
getFunctionReturnType(std::string fulltype)
{
    std::string buffer = "";
    for (int i = fulltype.find("(") + 2; i < fulltype.length(); i++)
    {
        char c = fulltype.at(i);
        if (c == '_' || c == ')')
        {
            break;
        }

        buffer += c;
    }

    return buffer;
}

std::string
getReturnType(std::string fulltype)
{
    switch (fulltype.at(0))
    {
    case 'U':
    case 'P':
        return fulltype.substr(1, fulltype.length());
    case 'L':
        return "list_" + fulltype.substr(1, fulltype.length());
    case 'D':
        return "dictionary_" + fulltype.substr(1, fulltype.length());
    case 'F':
        return getFunctionReturnType(fulltype);
    default:
        return "unknown";
    }
}

std::string
primaryHelper(ast::Primary* primary, SymbolTable* symbolTable,
              SemanticErrorHandler* semanticErrorHandler, UDTTable* udtTable,
              std::string curUDT)
{
    ast::Primary::PrimaryType type = primary->getPrimaryType();

    switch (type)
    {
    case ast::Primary::IdentifierLiteral:
    {
        ast::Identifier* subnode = dynamic_cast<ast::Identifier*>(primary);
        std::string variableName = subnode->getValue();

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

            std::string fullAttributeType =
                symbolTable->getSymbolType(variableName);

            return getReturnType(fullAttributeType);
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
    case ast::Primary::DictionaryLiteral:
    {
        ast::DictionaryLiteral* subsubnode =
            dynamic_cast<ast::DictionaryLiteral*>(primary);

        std::vector<ast::DictionaryItem*> dictionaryItems =
            subsubnode->getItems();

        if (dictionaryItems.size() == 0)
            return "dictionary_empty_empty";

        std::string expectedValueType =
            primaryHelper(dictionaryItems.at(0)->getValue(), symbolTable,
                          semanticErrorHandler, udtTable, curUDT);
        std::string expectedKeyType =
            primaryHelper(dictionaryItems.at(0)->getKey(), symbolTable,
                          semanticErrorHandler, udtTable, curUDT);

        for (ast::DictionaryItem* const& item : dictionaryItems)
        {
            std::string actualValueType =
                primaryHelper(item->getValue(), symbolTable,
                              semanticErrorHandler, udtTable, curUDT);
            std::string actualKeyType =
                primaryHelper(item->getKey(), symbolTable, semanticErrorHandler,
                              udtTable, curUDT);

            if (actualKeyType != expectedKeyType)
            {
                semanticErrorHandler->handle(
                    new Error(item->getLineNum(),
                              "Inconsistent key types in dictionary"));

                // continue with semantic analysis
                return "dictionary_empty_empty";
            }

            if (actualValueType != expectedValueType)
            {
                semanticErrorHandler->handle(
                    new Error(item->getLineNum(),
                              "Inconsistent value types in dictionary"));

                return "dictionary_empty_empty";
            }
        }

        return "dictionary_" + expectedKeyType + "_" + expectedValueType;
    }
    case ast::Primary::ListLiteral:
    {
        ast::ListLiteral* subnode = dynamic_cast<ast::ListLiteral*>(primary);

        std::vector<ast::ListItem*> items = subnode->getItems();

        // empty lists do not need type checking
        if (items.size() == 0)
        {
            return "list_empty";
        }

        std::string expectedType =
            primaryHelper(items.at(0)->getValue(), symbolTable,
                          semanticErrorHandler, udtTable, curUDT);

        if (subnode->isListIndex() || subnode->isDictionaryIndex())
        {
            // return list value type
            std::string variableName = subnode->getName()->getValue();
            if (!symbolTable->hasVariable(variableName))
            {
                semanticErrorHandler->handle(
                    new Error(subnode->getLineNum(),
                              "Variable: " + variableName + " is undefined."));

                return "unknown";
            }

            std::string fullAttributeType =
                symbolTable->getSymbolType(variableName);

            if (fullAttributeType.at(0) == 'L')
            {
                return fullAttributeType.substr(1, fullAttributeType.length());
            }

            if (fullAttributeType.at(0) == 'D')
            {

                std::string keyType = fullAttributeType.substr(
                    1, fullAttributeType.find("_") - 1);
                std::string valueType =
                    fullAttributeType.substr(fullAttributeType.find("_") + 1,
                                             fullAttributeType.length());

                // check that the provided key is the right type
                if (expectedType != keyType)
                {
                    semanticErrorHandler->handle(new Error(
                        subnode->getLineNum(),
                        "Supplied key type: " + expectedType +
                            " does not match expected key type: " + keyType +
                            " for: " + variableName + "."));

                    return "unknown";
                }

                return valueType;
            }

            return getReturnType(fullAttributeType);
        }

        for (ast::ListItem* const& item : items)
        {
            std::string type =
                primaryHelper(item->getValue(), symbolTable,
                              semanticErrorHandler, udtTable, curUDT);

            // confirm the list is homogenous
            if (type != expectedType)
            {
                semanticErrorHandler->handle(new Error(
                    item->getLineNum(),
                    "List is not homogenous. Received types: " + type +
                        " and " + expectedType + " which do not match."));

                // return list as empty to continue semantic analyis
                return "list_empty";
            }
        }
        return "list_" + expectedType;
    }
    case ast::Primary::AttributeAccessLiteral:
    {
        ast::AttributeAccess* subsubnode =
            dynamic_cast<ast::AttributeAccess*>(primary);

        std::string attributeName = subsubnode->getAttribute()->getValue();
        std::string variableName = subsubnode->getUDT()->getValue();

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

            std::string udtTypeFull = symbolTable->getSymbolType(variableName);
            variableName = udtTypeFull.substr(1, udtTypeFull.length());
        }

        // ensure udt exists - double checked so no need for another
        // error message
        if (!udtTable->hasUDT(variableName))
        {
            return "unknown";
        }

        // ensure attribute exists for udt - double checked so only return error
        // once
        if (!udtTable->getAttributeSymbolTable(variableName)
                 ->hasVariable(attributeName))
        {
            return "unknown";
        }

        std::string fullAttributeType =
            udtTable->getAttributeSymbolTable(variableName)
                ->getSymbolType(attributeName);

        return getReturnType(fullAttributeType);
    }
    case ast::Primary::MethodAccessLiteral:
    {
        ast::MethodAccess* node = dynamic_cast<ast::MethodAccess*>(primary);
        std::string methodName = node->getName()->getValue();
        std::string variableUDTname = node->getUDT()->getValue();

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
            {
                return "unknown";
            }

            std::string udtname = symbolTable->getSymbolType(variableUDTname);

            // ensure variable's udt exists - double checked so no need for
            // another error message
            if (!udtTable->hasUDT(udtname.substr(1, udtname.length())))
            {
                return "unknown";
            }

            variableUDTname = udtname.substr(1, udtname.length());
        }

        // ensure metho exists for udt - double checked so no need for another
        // error message
        if (!udtTable->getMethodSymbolTable(variableUDTname)
                 ->hasVariable(methodName))
        {
            return "unknown";
        }

        return getReturnType(udtTable->getMethodSymbolTable(variableUDTname)
                                 ->getSymbolType(methodName));
    }
    case ast::Primary::FunctionCallLiteral:
    {
        ast::FunctionCall* subsubnode =
            dynamic_cast<ast::FunctionCall*>(primary);

        std::string functionName = subsubnode->getName()->getValue();

        // ensure function exists
        if (!symbolTable->hasVariable(functionName))
        {
            return "unknown";
        }

        // get function type from symbol table
        std::string fullType = symbolTable->getSymbolType(functionName);

        return getReturnType(fullType);
    }
    }
}

std::string
expressionHelper(ast::Expression* node, SymbolTable* symbolTable,
                 SemanticErrorHandler* semanticErrorHandler, UDTTable* udtTable,
                 std::string curUDT)
{
    switch (node->getExpressionType())
    {
    case ast::Expression::NewExpression:
    {
        ast::NewExpression* subnode = dynamic_cast<ast::NewExpression*>(node);
        ast::New* newnode = subnode->getNewVal();

        switch (newnode->getNewType())
        {
        case ast::New::DictionaryLiteral:
        {
            ast::DictionaryLiteral* dictionaryLiteral =
                dynamic_cast<ast::DictionaryLiteral*>(newnode);

            std::vector<ast::DictionaryItem*> items =
                dictionaryLiteral->getItems();

            // empty lists do not need type checking
            if (items.size() == 0)
            {
                return "dictionary_empty_empty";
            }

            std::string expectedKeyType =
                primaryHelper(items.at(0)->getKey(), symbolTable,
                              semanticErrorHandler, udtTable, curUDT);
            std::string expectedValueType =
                primaryHelper(items.at(0)->getValue(), symbolTable,
                              semanticErrorHandler, udtTable, curUDT);

            for (ast::DictionaryItem* const& item : items)
            {
                std::string keyType =
                    primaryHelper(item->getKey(), symbolTable,
                                  semanticErrorHandler, udtTable, curUDT);
                std::string valueType =
                    primaryHelper(item->getValue(), symbolTable,
                                  semanticErrorHandler, udtTable, curUDT);

                // confirm the dictionary keys are homogenous
                if (keyType != expectedKeyType)
                {
                    semanticErrorHandler->handle(
                        new Error(item->getLineNum(),
                                  "Inconsistent key types in dictionary"));

                    // return list as empty to continue semantic analyis
                    return "dictionary_empty_empty";
                }

                // confirm the dictionary values are homogenous
                if (valueType != expectedValueType)
                {
                    semanticErrorHandler->handle(
                        new Error(item->getLineNum(),
                                  "Inconsistent value types in dictionary"));

                    // return list as empty to continue semantic analyis
                    return "dictionary_empty_empty";
                }
            }

            return "dictionary_" + expectedKeyType + "_" + expectedValueType;
        }
        case ast::New::ListLiteral:
        {
            ast::ListLiteral* listLiteral =
                dynamic_cast<ast::ListLiteral*>(newnode);

            std::vector<ast::ListItem*> items = listLiteral->getItems();

            // empty lists do not need type checking
            if (items.size() == 0)
            {
                return "list_empty";
            }

            std::string expectedType =
                primaryHelper(items.at(0)->getValue(), symbolTable,
                              semanticErrorHandler, udtTable, curUDT);

            for (ast::ListItem* const& item : items)
            {
                std::string type =
                    primaryHelper(item->getValue(), symbolTable,
                                  semanticErrorHandler, udtTable, curUDT);

                // confirm the list is homogenous
                if (type != expectedType)
                {
                    semanticErrorHandler->handle(new Error(
                        item->getLineNum(),
                        "List is not homogenous. Received types: " + type +
                            " and " + expectedType + " which do not match."));

                    // return list as empty to continue semantic analyis
                    return "list_empty";
                }
            }

            return "list_" + expectedType;
        }
        case ast::New::UserDefinedType:

            ast::UserDefinedType* udt =
                dynamic_cast<ast::UserDefinedType*>(newnode);

            std::string name = udt->getName()->getValue();

            std::string fullTypeName = "udt_" + name;

            for (ast::DictionaryItem* const& item : udt->getAttributes())
            {
                fullTypeName +=
                    "_" +
                    primaryHelper(item->getKey(), symbolTable,
                                  semanticErrorHandler, udtTable, curUDT) +
                    "_" +
                    primaryHelper(item->getValue(), symbolTable,
                                  semanticErrorHandler, udtTable, curUDT);
            }

            return fullTypeName;
        }
        break;
    }
    case ast::Expression::GroupingExpression:
    {
        ast::GroupingExpression* subnode =
            dynamic_cast<ast::GroupingExpression*>(node);

        semanticErrorHandler->handle(
            new Error(subnode->getLineNum(),
                      "Unexpected grouping inside binary expression. "
                      "Groupings cannot be nested."));

        return "boolean";
    }
    case ast::Expression::PrimaryExpression:
    {
        ast::PrimaryExpression* subnode =
            dynamic_cast<ast::PrimaryExpression*>(node);

        return primaryHelper(subnode->getPrimary(), symbolTable,
                             semanticErrorHandler, udtTable, curUDT);
    }
    case ast::Expression::UnaryExpression:
    {
        ast::UnaryExpression* subnode =
            dynamic_cast<ast::UnaryExpression*>(node);

        // must be a boolean so fine to return boolean here
        return "bool";
    }
    }
}

std::string
getRightExpressionType(ast::BinaryExpression* node, SymbolTable* symbolTable,
                       SemanticErrorHandler* semanticErrorHandler,
                       UDTTable* udtTable, std::string curUDT)
{
    return expressionHelper(binaryExpressionHelper(node), symbolTable,
                            semanticErrorHandler, udtTable, curUDT);
}

bool
isLegalGrouping(ast::BinaryExpression* node)
{
    switch (node->getBinaryExpressionType())
    {
    case ast::BinaryExpression::Assignment:
    case ast::BinaryExpression::ExpressionOnlyStatement:
        return false;

    case ast::BinaryExpression::BinaryCompOrArith:
        ast::BinaryCompOrArith* subnode =
            dynamic_cast<ast::BinaryCompOrArith*>(node);
        return subnode->isComparison();
    }
}

void
compareFunctions(std::vector<std::string> inputs,
                 std::vector<ast::Primary*> args, std::string name,
                 SymbolTable* symbolTable,
                 SemanticErrorHandler* semanticErrorHandler, UDTTable* udtTable,
                 std::string curUDT)
{
    int numArgs = args.size();
    int numInps = inputs.size();

    if (numArgs < numInps)
    {
        semanticErrorHandler->handle(new Error(
            0, "Not enough args supplied to function: " + name + "."));
    }
    else if (numArgs > numInps)
    {
        semanticErrorHandler->handle(
            new Error(0, "Too many args supplied to function: " + name + "."));
    }
    else
    {
        for (int i = 0; i < numArgs; i++)
        {
            ast::Primary* arg = args.at(i);
            std::string actual = primaryHelper(
                arg, symbolTable, semanticErrorHandler, udtTable, curUDT);

            // if not primitive, see if it is a variable in the symbol table
            if (!isPrimitive(actual))
            {
                if (!symbolTable->hasVariable(actual))
                {
                    semanticErrorHandler->handle(new Error(
                        0, "Undefined argument: " + actual +
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
                            0,
                            "Illegal argument: " + actual +
                                " supplied for function: " + name +
                                ". Sorry, functions are not first order :("));

                        // to continue semantic analysis
                        actual = inputs[i];
                        break;
                    case 'L':
                        new Error(0,
                                  "Illegal argument: " + actual +
                                      " supplied for function: " + name +
                                      ". Lists cannot be passed to functions");

                        // to continue semantic analysis
                        actual = inputs[i];
                        break;
                    case 'D':
                        new Error(
                            0,
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
                    0, "Supplied argument type of: " + actual +
                           " does not match expected type of: " + inputs[i] +
                           "."));
            }
        }
    }
}