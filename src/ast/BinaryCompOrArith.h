/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "BinaryExpression.h"
#include "Expression.h"
#include "Node.h"
#include <string>

namespace ast
{
class BinaryCompOrArith : public BinaryExpression, public Node
{
  private:
    std::string operation;
    bool intOnly;
    bool boolOnly;
    bool fltOrIntOnly;
    bool isComp;

  public:
    // constructor
    BinaryCompOrArith(std::string op, bool c, bool io, bool bo, bool fio,
                      Expression* l, BinaryExpression* r, int lineNum)
        : Node(lineNum), BinaryExpression(l, r)
    {
        operation = op;
        isComp = c;
        intOnly = io;
        boolOnly = bo;
        fltOrIntOnly = fio;
    };
    // destructor
    ~BinaryCompOrArith(){};
    // implement BinaryExpression
    /* virtual */ BinaryExpressionType
    getBinaryExpressionType()
    {
        return BinaryExpressionType::BinaryCompOrArith;
    }
    // get methods
    std::string
    getOperation()
    {
        return operation;
    }
    // methods for determining type usage
    bool
    isComparison()
    {
        return isComp;
    }
    bool
    onlyAcceptsInt()
    {
        return intOnly;
    }
    bool
    onlyAcceptsBool()
    {
        return boolOnly;
    }
    bool
    onlyAcceptsFltOrInt()
    {
        return fltOrIntOnly;
    }
};
}