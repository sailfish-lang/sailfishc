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
#include <vector>

class SemanticAnalyzerErrorHandler
{
  private:
    std::string filename;
    std::vector<std::shared_ptr<Error>> errors;
    bool shouldDisplay;

  public:
    SemanticAnalyzerErrorHandler(const std::string& f, bool sd)
        : filename(f), shouldDisplay(sd){};
    void handle(std::unique_ptr<Error>);
    std::vector<std::shared_ptr<Error>>
    getErrors()
    {
        return errors;
    }
};