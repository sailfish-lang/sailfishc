/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error.h"
#include <memory>

class ParserErrorHandler
{
  public:
    ParserErrorHandler(){};
    void handle(std::unique_ptr<Error>);
};