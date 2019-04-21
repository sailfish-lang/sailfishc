/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * SymbolTable maps variable names to types.
 */
#pragma once
#include "SymbolMetaData.h"
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable
{
  private:
    int scopeLevel;
    std::unordered_map<std::string, std::stack<SymbolMetaData*>>
        globalScopeTable;
    std::vector<std::string> localCache;

    // internal helper methods
    void addStdlib(std::string, std::string);

  public:
    SymbolTable()
    {
        scopeLevel = 0;
        globalScopeTable.clear();
        localCache.push_back("|");

        addBuiltins();
    }

    // enter the scope, incrementing the scope level counter
    void enterScope();

    // exit the scope, decrementing the scope level counter and popping all
    // local scoped variables scopes
    void exitScope();

    // check if a symbol is in the symbol table
    bool hasVariable(const std::string);

    // retreive a symbol's type from the symbol table
    std::string getSymbolType(const std::string);

    // retreive a symbol's scope level from the symbol table
    int getSymbolScope(const std::string);

    // either push to the variables scope if exists or add variable
    bool addSymbol(const std::string, const std::string);

    // for removing symbols which should basically never happen except for the
    // class name from the attributes
    void removeSymbol(const std::string&);

    // helper methods
    bool
    isGlobalScope()
    {
        return scopeLevel == 0;
    }
    int
    getCurrentScope()
    {
        return scopeLevel;
    }

    std::vector<std::string>
    getSymbols()
    {
        std::vector<std::string> symbols;
        for (auto const& element : globalScopeTable)
        {
            symbols.push_back(element.first);
        }

        return symbols;
    }

    void addBuiltins();
    void clear();
};
