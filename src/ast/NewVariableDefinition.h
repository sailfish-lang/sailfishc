/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "GeneralDefinition.h"
#include "Variable.h"

namespace ast
{
class NewVariableDefinition : public GeneralDefinition
{
  private:
    Variable* var;
    Expression* expr;

  public:
    // constructor
    NewVariableDefinition(Variable* v, Expression* e)
    {
        var = v;
        expr = e;
    };
    // destructor
    ~NewVariableDefinition()
    {
        delete expr;
        delete var;
    };
    // implement GeneralDefinition
    /* virtual */ GeneralDefinitionType
    getGeneralDefinitionType()
    {
        return GeneralDefinitionType::NewVariableDefinition;
    }
    // get methods
    Variable*
    getVariable()
    {
        return var;
    }
    Expression*
    getExpression()
    {
        return expr;
    }
};
}