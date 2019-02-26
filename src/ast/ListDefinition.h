/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "GeneralDefinition.h"
#include "Identifier.h"

namespace ast
{
class ListDefinition : public GeneralDefinition
{
  private:
    Identifier* name;
    Expression* expr;

  public:
    // constructor
    ListDefinition(Identifier* i, Expression* e)
    {
        name = i;
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
    Expression*
    getExpression()
    {
        return expr;
    }
};
}