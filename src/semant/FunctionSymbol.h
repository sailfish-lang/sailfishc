/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
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
    FunctionSymbol(std::string type, std::vector<std::string> i,
                   std::vector<std::string> o)
        : Symbol(Function, type)
    {
        inputs = i;
        outputs = o;
    }
    // deconstructor
    ~FunctionSymbol()
    {
    }
    // implements Symbol
    /* virtual */ std::vector<std::string> getInputTypes();
    /* virtual */ std::vector<std::string> getOutputTypes();
};