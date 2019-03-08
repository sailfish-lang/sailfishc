/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SymbolMetaData.h"
#include <vector>

class ScopeStack
{
  private:
    std::vector<SymbolMetaData*> stack;
    int size;

  public:
    // constructor
    ScopeStack()
    {
        size = 0;
    }
    // destructor
    ~ScopeStack()
    {
    }
    // typical stack methods
    void
    push(std::string type, int scopeLevel)
    {
        stack.push_back(new SymbolMetaData(type, scopeLevel));
        ++size;
    }
    SymbolMetaData*
    pop()
    {
        stack.pop_back();
        --size;
    }
    SymbolMetaData*
    peek()
    {
        return stack.at(size - 1);
    }
    bool
    isEmpty()
    {
        return size == 0;
    }
};