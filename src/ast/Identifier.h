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
class Identifier : public Primary, public Node
{
  private:
    std::string identifier;

  public:
    // constructor
    Identifier(std::string s, int lineNum) : Node(lineNum)
    {
        identifier = s;
    };
    // destructor
    ~Identifier(){};
    // implement Primary
    /* virtual */ PrimaryType
    getPrimaryType()
    {
        return PrimaryType::Identifier;
    }
    // get method
    std::string
    getValue()
    {
        return identifier;
    }
};
}