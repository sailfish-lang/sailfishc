/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Symbol.h"

class ListSymbol : public Symbol
{
  public:
    // constructor
    ListSymbol(std::string type) : Symbol(List, type)
    {
    }
    // deconstructor
    ~ListSymbol()
    {
    }
    // implements Symbol, using all defaults
};