/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SymbolTable.h"
#include "UDTMetaData.h"
#include <string>
#include <unordered_map>

class UDTTable
{
  private:
    std::unordered_map<std::string, UDTMetaData*> udtTable;

  public:
    // constructor
    UDTTable()
    {
        udtTable.clear();
    }
    // destructor
    ~UDTTable()
    {
    }

    // check if a UDT is in the UDT table
    bool hasUDT(std::string);

    // retreive a UDT's attribute Symbol Table from the UDT table
    SymbolTable* getAttributeSymbolTable(std::string);

    // retreive a UDT's method Symbol Table from the UDT table
    SymbolTable* getMethodSymbolTable(std::string);

    // returns true if added or false if already exists
    bool addUDT(std::string, SymbolTable*, SymbolTable*);

    // allow us to update the method table after udt key aleady added
    bool updateUDTMethods(std::string name, SymbolTable* methods);
};
