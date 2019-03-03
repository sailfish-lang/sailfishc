/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include <string>

namespace ast
{
class Typename : public Node
{
  private:
    std::string type;

  public:
    // constructor
    Typename(std::string t, int lineNum) : Node(lineNum)
    {
        type = t;
    };
    // destructor
    ~Typename(){};
    // get method
    std::string
    getType()
    {
        return type;
    }
};
}