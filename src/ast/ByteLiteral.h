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
class ByteLiteral : public Primary, public Node
{
  private:
    std::string string;

  public:
    // constructor
    ByteLiteral(std::string s, int lineNum) : Node(lineNum)
    {
        string = s;
    };
    // destructor
    ~ByteLiteral(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::ByteLiteral;
    }
    // get methods
    std::string
    getString()
    {
        return string;
    }
};
}