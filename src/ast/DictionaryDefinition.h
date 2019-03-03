/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "GeneralDefinition.h"
#include "Identifier.h"
#include "Node.h"
#include "Typename.h"

namespace ast
{
class DictionaryDefinition : public GeneralDefinition, public Node
{
  private:
    Identifier* name;
    Typename* keyType;
    Typename* valueType;
    Expression* expr;

  public:
    // constructor
    DictionaryDefinition(Identifier* i, Typename* kt, Typename* vt,
                         Expression* e, int lineNum)
        : Node(lineNum)
    {
        name = i;
        keyType = kt;
        valueType = vt;
        expr = e;
    };
    // destructor
    ~DictionaryDefinition()
    {
        delete expr;
    };
    // implement GeneralDefinition
    /* virtual */ GeneralDefinitionType
    getGeneralDefinitionType()
    {
        return GeneralDefinitionType::DictionaryDefinition;
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    Typename*
    getKeyType()
    {
        return keyType;
    }
    Typename*
    getValueType()
    {
        return valueType;
    }
    Expression*
    getExpression()
    {
        return expr;
    }
};
}