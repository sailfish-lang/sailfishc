/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * Semantic Analyzer Error Handler handles errors related to Semantic Analysis.
 */
#pragma once
#include "Error.h"
#include <memory>
#include <string>

class SemanticAnalyzerErrorHandler
{
  private:
    std::string filename;

  public:
    SemanticAnalyzerErrorHandler(const std::string& f) : filename(f){};
    void handle(std::unique_ptr<Error>);
};