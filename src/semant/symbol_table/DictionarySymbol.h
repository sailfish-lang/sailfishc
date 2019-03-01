/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include "Symbol.h"

class DictionarySymbol : public Symbol
{
  private:
    std::string keyType;
    std::string valueType;

  public:
    // constructor
    DictionarySymbol(std::string t, std::string k, std::string v)
        : Symbol(t, new ScopeStack(), Dictionary)
    {

        keyType = k;
        valueType = v;
    }
    // deconstructor
    ~DictionarySymbol()
    {
    }
    // implements Symbol
    /* virtual */ std::string getKeyType();
    /* virtual */ std::string getValueType();
};