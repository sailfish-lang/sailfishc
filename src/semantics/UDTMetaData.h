/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * UDTMetaData holds the attribute and method symbol table for a udt entry in
 * the udt table.
 */
#pragma once
#include "SymbolTable.h"
#include <memory>
#include <string>

class UDTMetaData
{
  private:
    std::shared_ptr<SymbolTable> attributes;
    std::shared_ptr<SymbolTable> methods;

  public:
    // constructor
    UDTMetaData(std::shared_ptr<SymbolTable> a, std::shared_ptr<SymbolTable> m)
    {
        attributes = a;
        methods = m;
    }
    // destructor
    ~UDTMetaData(){};
    // get methods
    std::shared_ptr<SymbolTable>
    getAttributeSymbolTable()
    {
        return attributes;
    }
    std::shared_ptr<SymbolTable>
    getMethodSymbolTable()
    {
        return methods;
    }
    // set methods
    void
    setAttributeSymbolTable(std::shared_ptr<SymbolTable> a)
    {
        attributes = a;
    }
    void
    setMethodSymbolTable(std::shared_ptr<SymbolTable> m)
    {
        methods = m;
    }
};
