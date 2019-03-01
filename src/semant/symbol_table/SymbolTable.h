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
#include <string>
#include <unordered_map>

class SymbolTable
{
  private:
    int scopeLevelCounter;
    std::unordered_map<std::string, Symbol*> globalScopeTable;

  public:
    // constructor
    SymbolTable()
    {
        // intial level is 0 to stay in computer science world
        scopeLevelCounter = 0;
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
    void exitScope(std::vector<std::string> localScopeCache);

    // check if a symbol is in the symbol table
    bool hasVariable(std::string);

    // retreive a symbol from the symbol table
    Symbol* getSymbol(std::string);

    // either push to the variables scope if exists or add variable
    // Primitive and List
    void addSymbol(std::string varName, std::string t, Symbol::SymbolType st);
    // Dictionary
    void addSymbol(std::string varName, std::string t, std::string keyType,
                   std::string valueType, Symbol::SymbolType st);
    // Function
    void addSymbol(std::string varName, std::string t,
                   std::vector<std::string> inputs,
                   std::vector<std::string> outputs, Symbol::SymbolType st);
    // UDT
    void addSymbol(std::string varName, std::string t, SymbolTable* attributes,
                   SymbolTable* methods, Symbol::SymbolType st);
};
