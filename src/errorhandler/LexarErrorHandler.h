/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"

class LexarErrorHandler
{
  private:
    bool foundError;

  public:
    // constructor
    LexarErrorHandler()
    {
        foundError = false;
    };
    // destructor
    ~LexarErrorHandler(){};
    // implement ErrorHandler
    /* virtual */
    void handle(Error*);
    void end();
};