/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include "Symbol.h"

class ListSymbol : public Symbol
{
  public:
    // constructor
    ListSymbol(std::string t) : Symbol(t, new ScopeStack(), List)
    {
    }
    // deconstructor
    ~ListSymbol()
    {
    }
    // implements Symbol, using all defaults
};