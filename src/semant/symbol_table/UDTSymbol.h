/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include "Symbol.h"
#include "SymbolTable.h"

class UDTSymbol : public Symbol
{
  private:
    SymbolTable* attributes;
    SymbolTable* methods;

  public:
    // constructor
    UDTSymbol(std::string t, SymbolTable* a, SymbolTable* m)
        : Symbol(t, new ScopeStack(), UDT)
    {
        attributes = a;
        methods = m;
    }
    // deconstructor
    ~UDTSymbol()
    {
        delete attributes;
        delete methods;
    }
    // implements Symbol
    /* virtual */ SymbolTable* getAttributes();
    /* virtual */ SymbolTable* getMethods();
};