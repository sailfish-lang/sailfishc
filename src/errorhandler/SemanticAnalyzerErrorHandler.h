/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error2.h"
#include <memory>

class SemanticAnalyzerErrorHandler
{
  public:
    SemanticAnalyzerErrorHandler(){};
    void handle(std::unique_ptr<Error2>);
};