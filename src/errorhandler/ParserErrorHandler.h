/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"

class ParserErrorHandler
{
  private:
    bool foundError;

  public:
    // constructor
    ParserErrorHandler()
    {
        foundError = false;
    };
    // destructor
    ~ParserErrorHandler(){};
    // implement ErrorHandler
    /* virtual */
    void handle(Error*);
    void end();
}