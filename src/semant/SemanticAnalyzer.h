/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../errorhandler/Error.h"
#include "../visitor/InitialExecution.h"
#include "./TypeChecker.h"
#include <vector>

class SemanticAnalyzer
{
  private:
    ast::Start* root;

  public:
    // constructor
    SemanticAnalyzer(ast::Start* r)
    {
        root = r;
    };
    // deconstructor
    ~SemanticAnalyzer()
    {
        delete root;
    };

    void analyze();

    // strictly a helper for writing tests that returns all the semantic errors
    std::vector<Error*> testAnalyze();
};