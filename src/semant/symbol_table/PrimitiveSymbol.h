/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include "Symbol.h"

class PrimitiveSymbol : public Symbol
{
  public:
    // constructor
    PrimitiveSymbol(std::string t) : Symbol(t, new ScopeStack())
    {
    }
    // deconstructor
    ~PrimitiveSymbol()
    {
    }
    // implements Symbol, using all defaults
};