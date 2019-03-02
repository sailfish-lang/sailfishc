/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"
#include "ErrorHandler.h"

class ParserErrorHandler : public ErrorHandler
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
};