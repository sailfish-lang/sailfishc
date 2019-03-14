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
    void handle(Error*);
};