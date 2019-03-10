/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"
#include "ErrorHandler.h"
#include <vector>

class SemanticErrorHandler : public ErrorHandler
{
  private:
    std::vector<Error*> errors;

  public:
    // constructor
    SemanticErrorHandler(){};
    // destructor
    ~SemanticErrorHandler()
    {
        for (auto const error : errors)
        {
            delete error;
        }
    };
    // implement ErrorHandler
    /* virtual */
    void handle(Error*);
    void end();
    // get methods
    std::vector<Error*> getErrors();
};