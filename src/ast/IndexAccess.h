/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "IntegerLiteral.h"

namespace ast
{
class IndexAccess : public Expression
{
  private:
    IntegerLiteral* index;

  public:
    // constructor
    IndexAccess(IntegerLiteral* i)
    {
        index = i;
    };
    // implement Expression
    /* virtual */ ExpressionType
    getExpressionType()
    {
        return ExpressionType::IndexAccess;
    }
    // get method
    IntegerLiteral*
    getIndex()
    {
        return index;
    }
};
}