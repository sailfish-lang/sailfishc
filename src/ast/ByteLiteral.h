/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <string>

namespace ast
{
class ByteLiteral : public Primary
{
  private:
    std::string string;

  public:
    // constructor
    ByteLiteral(std::string s)
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