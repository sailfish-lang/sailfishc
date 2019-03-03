/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Identifier.h"
#include "Node.h"
#include "Typename.h"

namespace ast
{
class Variable : public Node
{
  private:
    Typename* type;
    Identifier* name;

  public:
    // constructor
    Variable(Typename* t, Identifier* n, int lineNum) : Node(lineNum)
    {
        type = t;
        name = n;
    }
    // destructor
    ~Variable()
    {
        delete type;
        delete name;
    };
    // get methods
    Typename*
    getType()
    {
        return type;
    }
    Identifier*
    getName()
    {
        return name;
    }
};
}