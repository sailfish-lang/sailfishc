/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "GeneralDefinition.h"
#include "Identifier.h"
#include "NewExpression.h"
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
    NewExpression* expr;

  public:
    // constructor
    DictionaryDefinition(Identifier* i, Typename* kt, Typename* vt,
                         NewExpression* e, int lineNum)
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
    NewExpression*
    getExpression()
    {
        return expr;
    }
};
}