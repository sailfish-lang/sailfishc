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
class ListDefinition : public GeneralDefinition, public Node
{
  private:
    Identifier* name;
    Typename* type;
    Expression* expr;

  public:
    // constructor
    ListDefinition(Identifier* i, Typename* t, Expression* e, int lineNum)
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
    Expression*
    getExpression()
    {
        return expr;
    }
};
}