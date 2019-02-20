/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <string>

namespace ast
{
class FloatLiteral : public Primary
{
  private:
    std::string num;

  public:
    // constructor
    FloatLiteral(std::string s)
    {
        num = s;
    };
    // implement Primary
    /* virtual */ PrimaryType
    getType()
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