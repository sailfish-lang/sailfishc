/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "Primary.h"
#include <string>

namespace ast
{
class IntegerLiteral : public Primary, public Node
{
  private:
    std::string num;

  public:
    // constructor
    IntegerLiteral(std::string s, int lineNum) : Node(lineNum)
    {
        num = s;
    };
    // destructor
    ~IntegerLiteral(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::IntegerLiteral;
    }
    // get methods
    std::string
    getNum()
    {
        return num;
    }
};
}