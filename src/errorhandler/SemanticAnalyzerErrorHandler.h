/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error2.h"
#include "Warning.h"
#include <memory>
#include <string>

class SemanticAnalyzerErrorHandler
{
  private:
    std::string filename;
  public:
    SemanticAnalyzerErrorHandler(const std::string& f): filename(f){};
    void handle(std::unique_ptr<Error2>);
    void handle(std::unique_ptr<Warning>);
};