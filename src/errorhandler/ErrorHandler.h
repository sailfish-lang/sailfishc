/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"

/**
 * ErrorHandler is an abstract base class for handling and displaying errors.
 * There are two main concrete implementations of the ErrorHandler, the handler
 * who throws on first error and the handler who catches all errors (with an
 * analyzer that presumably corrects errors along the way) and displays them at
 * the end.
 */
class ErrorHandler
{
  public:
    virtual void handle(Error*) = 0;
    virtual void end() = 0;
};