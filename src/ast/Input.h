/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Variable.h"

namespace ast
{
class Input
{
  private:
    Variable* input;

  public:
    // constructor
    Input(Variable* v)
    {
        input = v;
    };
    // getMethods
    Variable*
    getInput()
    {
        return input;
    }
};
}