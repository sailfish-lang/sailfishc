/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "DictionarySymbol.h"
#include "FunctionSymbol.h"
#include "ListSymbol.h"
#include "PrimitiveSymbol.h"
#include "Symbol.h"
#include "UDTSymbol.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable
{
  private:
    int scopeLevel;
    std::unordered_map<std::string, Symbol*> globalScopeTable;
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

    // retreive a symbol from the symbol table
    Symbol* getSymbol(std::string);

    // either push to the variables scope if exists or add variable
    // Primitive and List
    void addSymbol(std::string, std::string, Symbol::SymbolType);
    // Dictionary
    void addSymbol(std::string, std::string, std::string, std::string,
                   Symbol::SymbolType);
    // Function
    void addSymbol(std::string, std::string, std::vector<std::string>,
                   std::vector<std::string>, Symbol::SymbolType);
    // UDT
    void addSymbol(std::string, std::string, SymbolTable*, SymbolTable*,
                   Symbol::SymbolType);
};
