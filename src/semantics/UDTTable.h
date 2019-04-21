/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * UDTTable maps udt names to related data.
 */
#pragma once
#include "SymbolTable.h"
#include "UDTMetaData.h"
#include <memory>
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
    ~UDTTable(){};

    // check if a UDT is in the UDT table
    bool
    hasUDT(const std::string name)
    {
        return udtTable.find(name) != udtTable.end();
    }

    // retreive a UDT's attribute Symbol Table from the UDT table
    std::shared_ptr<SymbolTable>
    getAttributeSymbolTable(const std::string name)
    {
        if (hasUDT(name))
            return udtTable.at(name)->getAttributeSymbolTable();

        return nullptr;
    }

    // retreive a UDT's method Symbol Table from the UDT table
    std::shared_ptr<SymbolTable>
    getMethodSymbolTable(const std::string name)
    {
        if (hasUDT(name))
            return udtTable.at(name)->getMethodSymbolTable();

        return nullptr;
    }

    // returns true if added or false if already exists
    bool
    addUDT(const std::string name, std::shared_ptr<SymbolTable> attributes,
           std::shared_ptr<SymbolTable> methods)
    {
        if (hasUDT(name))
            return false;

        UDTMetaData* udtmd = new UDTMetaData(attributes, methods);
        udtTable.insert({name, udtmd});
        return true;
    }
};
