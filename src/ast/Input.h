/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "Variable.h"

namespace ast
{
class Input : public Node
{
  private:
    Variable* input;

  public:
    // constructor
    Input(Variable* v, int lineNum) : Node(lineNum)
    {
        input = v;
    };
    // destructor
    ~Input()
    {
        delete input;
    };
    // getMethods
    Variable*
    getInput()
    {
        return input;
    }
};
}