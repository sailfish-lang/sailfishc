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
class BooleanLiteral : public Primary, public Node
{
  private:
    std::string value;

  public:
    // constructor
    BooleanLiteral(std::string s, int lineNum) : Node(lineNum)
    {
        value = s;
    };
    // destructor
    ~BooleanLiteral(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::BooleanLiteral;
    }
    // get methods
    std::string
    getValue()
    {
        return value;
    }
};
}