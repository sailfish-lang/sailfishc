/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * SymbolMetaData represents the per scope level data associated with a field in
 * the SymbolTable.
 */
#pragma once
#include <string>

class SymbolMetaData
{
  private:
    std::string type;
    int scopeLevel;

  public:
    // constructor
    SymbolMetaData(std::string t, int s)
    {
        type = t;
        scopeLevel = s;
    }
    // destructor
    ~SymbolMetaData(){};
    // get methods
    std::string
    getType()
    {
        return type;
    }
    int
    getScopeLevel()
    {
        return scopeLevel;
    }
};