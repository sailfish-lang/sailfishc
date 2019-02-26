/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <string>

namespace ast
{
class Typename
{
  private:
    std::string type;

  public:
    // constructor
    Typename(std::string t)
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