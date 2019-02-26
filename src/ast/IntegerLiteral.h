/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <string>

namespace ast
{
class IntegerLiteral : public Primary
{
  private:
    std::string num;

  public:
    // constructor
    IntegerLiteral(std::string s)
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