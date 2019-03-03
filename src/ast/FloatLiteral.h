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
class FloatLiteral : public Primary, public Node
{
  private:
    std::string num;

  public:
    // constructor
    FloatLiteral(std::string s, int lineNum) : Node(lineNum)
    {
        num = s;
    };
    // destructor
    ~FloatLiteral(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::FloatLiteral;
    }
    // get methods
    std::string
    getNum()
    {
        return num;
    }
};
}