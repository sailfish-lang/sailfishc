#include "CommonExtractions.h"

ast::Expression*
binaryExpressionHelper(ast::BinaryExpression* node)
{
    return node->getLeftExpr();
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
              SemanticErrorHandler* semanticErrorHandler, UDTTable* udtTable)
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
                          semanticErrorHandler, udtTable);
        std::string expectedKeyType =
            primaryHelper(dictionaryItems.at(0)->getKey(), symbolTable,
                          semanticErrorHandler, udtTable);

        for (ast::DictionaryItem* const& item : dictionaryItems)
        {
            std::string actualValueType = primaryHelper(
                item->getValue(), symbolTable, semanticErrorHandler, udtTable);
            std::string actualKeyType = primaryHelper(
                item->getKey(), symbolTable, semanticErrorHandler, udtTable);

            if (actualKeyType != expectedKeyType)
            {
                semanticErrorHandler->handle(
                    new Error(subsubnode->getLineNum(),
                              "Inconsistent key types in dictionary"));

                // continue with semantic analysis
                return "dictionary_empty_empty";
            }

            if (actualValueType != expectedValueType)
            {
                semanticErrorHandler->handle(
                    new Error(subsubnode->getLineNum(),
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
                          semanticErrorHandler, udtTable);

        // if (subnode->getCanBeIndexAccess())
        // {
        //     std::cout << "here\n";
        //     return expectedType;
        // }

        for (ast::ListItem* const& item : items)
        {
            std::string type = primaryHelper(item->getValue(), symbolTable,
                                             semanticErrorHandler, udtTable);

            // confirm the list is homogenous
            if (type != expectedType)
            {
                semanticErrorHandler->handle(new Error(
                    subnode->getLineNum(),
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

        // ensure variable exists
        if (!symbolTable->hasVariable(variableName))
        {
            semanticErrorHandler->handle(new Error(
                subsubnode->getLineNum(),
                "Attribute: " + attributeName +
                    " called on undeclared variable: " + variableName + "."));
        }

        std::string udtTypeFull = symbolTable->getSymbolType(variableName);
        std::string udtType = udtTypeFull.substr(1, udtTypeFull.length());

        // ensure udt exists
        if (!udtTable->hasUDT(udtType))
        {
            semanticErrorHandler->handle(new Error(
                subsubnode->getLineNum(),
                "Attribute: " + attributeName +
                    " called on nonexistent udt type: " + udtType + "."));

            return "unknown";
        }

        // ensure attribute exists for udt
        if (!udtTable->getAttributeSymbolTable(udtType)->hasVariable(
                attributeName))
        {
            semanticErrorHandler->handle(new Error(
                subsubnode->getLineNum(),
                "Attribute: " + attributeName +
                    " does not exists for udt type: " + udtType + "."));
        }

        std::string fullAttributeType =
            udtTable->getAttributeSymbolTable(udtType)->getSymbolType(
                attributeName);

        return getReturnType(fullAttributeType);
    }
    case ast::Primary::MethodAccessLiteral:
    {
        ast::MethodAccess* subsubnode =
            dynamic_cast<ast::MethodAccess*>(primary);
        return "poopy"; // FIX
        // xx
    }
    case ast::Primary::FunctionCallLiteral:
    {
        ast::FunctionCall* subsubnode =
            dynamic_cast<ast::FunctionCall*>(primary);

        std::string functionName = subsubnode->getName()->getValue();

        // ensure function exists
        if (!symbolTable->hasVariable(functionName))
        {
            semanticErrorHandler->handle(
                new Error(subsubnode->getLineNum(),
                          "Function: " + functionName + " is undefined."));
            return "unknown";
        }

        // get function type from symbol table
        std::string fullType = symbolTable->getSymbolType(functionName);

        // TODO: make sure that the args are as expected

        return getReturnType(fullType);
    }
    }
}

std::string
expressionHelper(ast::Expression* node, SymbolTable* symbolTable,
                 SemanticErrorHandler* semanticErrorHandler, UDTTable* udtTable)
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
                              semanticErrorHandler, udtTable);
            std::string expectedValueType =
                primaryHelper(items.at(0)->getValue(), symbolTable,
                              semanticErrorHandler, udtTable);

            for (ast::DictionaryItem* const& item : items)
            {
                std::string keyType =
                    primaryHelper(item->getKey(), symbolTable,
                                  semanticErrorHandler, udtTable);
                std::string valueType =
                    primaryHelper(item->getValue(), symbolTable,
                                  semanticErrorHandler, udtTable);

                // confirm the dictionary keys are homogenous
                if (keyType != expectedKeyType)
                {
                    semanticErrorHandler->handle(
                        new Error(dictionaryLiteral->getLineNum(),
                                  "Inconsistent key types in dictionary"));

                    // return list as empty to continue semantic analyis
                    return "dictionary_empty_empty";
                }

                // confirm the dictionary values are homogenous
                if (valueType != expectedValueType)
                {
                    semanticErrorHandler->handle(
                        new Error(dictionaryLiteral->getLineNum(),
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
                              semanticErrorHandler, udtTable);

            for (ast::ListItem* const& item : items)
            {
                std::string type =
                    primaryHelper(item->getValue(), symbolTable,
                                  semanticErrorHandler, udtTable);

                // confirm the list is homogenous
                if (type != expectedType)
                {
                    semanticErrorHandler->handle(new Error(
                        subnode->getLineNum(),
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
                fullTypeName += "_" +
                                primaryHelper(item->getKey(), symbolTable,
                                              semanticErrorHandler, udtTable) +
                                "_" +
                                primaryHelper(item->getValue(), symbolTable,
                                              semanticErrorHandler, udtTable);
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
        // return list type to continue semantic analysis
        return "list";
    }
    case ast::Expression::PrimaryExpression:
    {
        ast::PrimaryExpression* subnode =
            dynamic_cast<ast::PrimaryExpression*>(node);

        return primaryHelper(subnode->getPrimary(), symbolTable,
                             semanticErrorHandler, udtTable);
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
                       UDTTable* udtTable)
{
    return expressionHelper(binaryExpressionHelper(node), symbolTable,
                            semanticErrorHandler, udtTable);
}

bool
isLegalGrouping(ast::BinaryExpression* node)
{
    switch (node->getBinaryExpressionType())
    {
    case ast::BinaryExpression::Multiplication:
    case ast::BinaryExpression::Exponentiation:
    case ast::BinaryExpression::Division:
    case ast::BinaryExpression::Modulo:
    case ast::BinaryExpression::Addition:
    case ast::BinaryExpression::Subtraction:
    case ast::BinaryExpression::Assignment:
    case ast::BinaryExpression::ExpressionOnlyStatement:
    {
        return false;
    }

    case ast::BinaryExpression::OrComparison:
    case ast::BinaryExpression::AndComparison:
    case ast::BinaryExpression::NonEquivalenceComparison:
    case ast::BinaryExpression::EquivalenceComparison:
    case ast::BinaryExpression::BinaryLessThanOrEqual:
    case ast::BinaryExpression::BinaryGreaterThanOrEqual:
    case ast::BinaryExpression::BinaryLessThan:
    case ast::BinaryExpression::BinaryGreaterThan:
    {
        return true;
    }
    }
}