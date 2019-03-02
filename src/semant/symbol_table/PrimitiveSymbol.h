/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Symbol.h"

class PrimitiveSymbol : public Symbol
{
  public:
    // constructor
    PrimitiveSymbol(std::string type) : Symbol(Primitive, type)
    {
    }
    // deconstructor
    ~PrimitiveSymbol()
    {
    }
    // implements Symbol, using all defaults
};