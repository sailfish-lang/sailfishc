/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <string>

namespace ast
{
class BooleanLiteral : public Primary
{
  private:
    std::string value;

  public:
    // constructor
    BooleanLiteral(std::string s)
    {
        value = s;
    };
    // implement Primary
    /* virtual */ PrimaryType
    getType()
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