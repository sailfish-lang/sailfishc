/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "UDTTable.h"
#include "UDTMetaData.h"
#include <string>

// check if a UDT is in the UDT table
bool
UDTTable::hasUDT(std::string name)
{
    return udtTable.find(name) != udtTable.end();
}

// retreive a UDT's attribute Symbol Table from the UDT table
SymbolTable*
UDTTable::getAttributeSymbolTable(std::string name)
{
    if (hasUDT(name))
    {
        return udtTable.find(name)->second->getAttributeSymbolTable();
    }

    return nullptr;
}

// retreive a UDT's method Symbol Table from the UDT table
SymbolTable*
UDTTable::getMethodSymbolTable(std::string name)
{
    if (hasUDT(name))
    {
        return udtTable.find(name)->second->getMethodSymbolTable();
    }

    return nullptr;
}

// returns true if added or false if already exists
bool
UDTTable::addUDT(std::string name, SymbolTable* attributes,
                 SymbolTable* methods)
{
    if (hasUDT(name))
    {
        return false;
    }

    UDTMetaData* udtmd = new UDTMetaData(attributes, methods);

    udtTable.insert({name, udtmd});

    return true;
}
