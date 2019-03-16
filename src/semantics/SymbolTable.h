/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SymbolMetaData.h"
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
    // constructor
    SymbolTable()
    {
        scopeLevel = 0;
        globalScopeTable.clear();
        localCache.push_back("|");

        // add standard library methods
        // ensures known even at internal symbol tables such as for udt
        // attributes and methods
        addSymbol("display_str", "Fdisplay_str{$str}($void)");
        addSymbol("display_int", "Fdisplay_int{$int}($void)");
        addSymbol("display_flt", "Fdisplay_flt{$flt}($void)");
        addSymbol("display_bool", "Fdisplay_bool{$bool}($void)");
    }
    // destructor
    ~SymbolTable()
    {
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

    // same as above, but used for attributes where we use scope level to mean
    // ordering
    bool addSymbolIterative(const std::string, const std::string);

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
};
