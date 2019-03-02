/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Symbol.h"

class DictionarySymbol : public Symbol
{
  private:
    std::string keyType;
    std::string valueType;

  public:
    // constructor
    DictionarySymbol(std::string type, std::string k, std::string v)
        : Symbol(Dictionary, type)
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