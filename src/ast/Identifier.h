/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Primary.h"
#include <string>

namespace ast
{
class Identifier : public Primary
{
  private:
    std::string identifier;

  public:
    // constructor
    Identifier(std::string s)
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