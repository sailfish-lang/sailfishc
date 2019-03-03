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
class StringLiteral : public Primary, public Node
{
  private:
    std::string string;

  public:
    // constructor
    StringLiteral(std::string s, int lineNum) : Node(lineNum)
    {
        string = s;
    };
    // destructor
    ~StringLiteral(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::StringLiteral;
    }
    // get methods
    std::string
    getString()
    {
        return string;
    }
};
}