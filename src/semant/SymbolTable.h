/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class SymbolTable
{
  private:
    int scopeLevel;
    std::unordered_map<std::string, ScopeStack*> globalScopeTable;
    std::vector<std::string> localCache;

  public:
    // constructor
    SymbolTable()
    {
        scopeLevel = 0;
        globalScopeTable.clear();
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
    bool hasVariable(std::string);

    // retreive a symbol's type from the symbol table
    std::string getSymbolType(std::string);

    // retreive a symbol's scope level from the symbol table
    int getSymbolScope(std::string);

    // either push to the variables scope if exists or add variable
    bool addSymbol(std::string, std::string);

    // helper methods
    bool
    isGlobalScope()
    {
        return scopeLevel == 0;
    }
};
