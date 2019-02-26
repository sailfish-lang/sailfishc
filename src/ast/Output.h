/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Typename.h"

namespace ast
{
class Output
{
  private:
    Typename* output;

  public:
    // constructor
    Output(Typename* t)
    {
        output = t;
    };
    // destructor
    ~Output()
    {
        delete output;
    };
    // get methods
    Typename*
    getOutput()
    {
        return output;
    }
};
}