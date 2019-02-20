/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Source.h"

namespace ast
{
class Start
{
  private:
    // FUTURE: imports
    Source* src;

  public:
    // constructor
    Start(Source* s)
    {
        src = s;
    };
    // get methods
    Source*
    getSource()
    {
        return src;
    }
};
}