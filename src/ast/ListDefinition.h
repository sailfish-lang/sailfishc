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
class ListDefinition : public GeneralDefinition, public Node
{
  private:
    Identifier* name;
    Typename* type;
    NewExpression* expr;

  public:
    // constructor
    ListDefinition(Identifier* i, Typename* t, NewExpression* e, int lineNum)
        : Node(lineNum)
    {
        name = i;
        type = t;
        expr = e;
    };
    // destructor
    ~ListDefinition()
    {
        delete name;
        delete expr;
    };
    // implement GeneralDefinition
    /* virtual */ GeneralDefinitionType
    getGeneralDefinitionType()
    {
        return GeneralDefinitionType::ListDefinition;
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    Typename*
    getType()
    {
        return type;
    }
    NewExpression*
    getExpression()
    {
        return expr;
    }
};
}