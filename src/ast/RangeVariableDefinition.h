/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "Variable.h"

namespace ast
{
class RangeVariableDefinition
{
  private:
    Variable* var;
    Expression* expr;

  public:
    // constructor
    RangeVariableDefinition(Variable* v, Expression* e)
    {
        var = v;
        expr = e
    };
    // get methods
    Variable*
    getVar()
    {
        return var;
    }
    Expression*
    getExpr()
    {
        return expr;
    }
};
}