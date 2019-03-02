/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"
#include "ErrorHandler.h"

class SymbolTableErrorHandler : public ErrorHandler
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
    // implement ErrorHandler
    /* virtual */
    void handle(Error*);
    void end();
};