/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "Typename.h"

namespace ast
{
class Output : public Node
{
  private:
    Typename* output;

  public:
    // constructor
    Output(Typename* t, int lineNum) : Node(lineNum)
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