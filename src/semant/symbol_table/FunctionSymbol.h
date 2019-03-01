/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include "Symbol.h"
#include <string>
#include <vector>

class FunctionSymbol : public Symbol
{
  private:
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

  public:
    // constructor
    FunctionSymbol(std::string t, std::vector<std::string> i,
                   std::vector<std::string> o)
        : Symbol(t, new ScopeStack())
    {
        inputs = i;
        outputs = o;
    }
    // deconstructor
    ~FunctionSymbol()
    {
    }
    // implements Symbol
    /* virtual */ bool hasInputTypes();
    /* virtual */ std::vector<std::string> getInputTypes();
    /* virtual */ bool hasOutputTypes();
    /* virtual */ std::vector<std::string> getOutputTypes();
};