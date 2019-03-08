/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SymbolTable.h"
#include <string>

class UDTMetaData
{
  private:
    SymbolTable* attributes;
    SymbolTable* methods;

  public:
    // constructor
    UDTMetaData(SymbolTable* a, SymbolTable* m)
    {
        attributes = a;
        methods = m;
    }
    // destructor
    ~UDTMetaData()
    {
    }
    // get methods
    SymbolTable*
    getAttributeSymbolTable()
    {
        return attributes;
    }
    SymbolTable*
    getMethodSymbolTable()
    {
        return methods;
    }
};