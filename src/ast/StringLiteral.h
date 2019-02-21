/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <string>

namespace ast
{
class StringLiteral : public Primary
{
  private:
    std::string string;

  public:
    // constructor
    StringLiteral(std::string s)
    {
        string = s;
    };
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