/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"

class SymbolTableErrorHandler
{
  private:
    bool foundError;

  public:
    // constructor
    SymbolTableErrorHandler()
    {
        foundError = false;
    };
    // destructor
    ~SymbolTableErrorHandler(){};
    void handle(Error*);
};