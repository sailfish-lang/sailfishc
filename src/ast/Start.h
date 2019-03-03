/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "Source.h"

namespace ast
{
class Start : public Node
{
  private:
    // FUTURE: imports
    Source* src;

  public:
    // constructor
    Start(Source* s, int lineNum) : Node(lineNum)
    {
        src = s;
    };
    // destructor
    ~Start()
    {
        delete src;
    };
    // get methods
    Source*
    getSource()
    {
        return src;
    }
};
}