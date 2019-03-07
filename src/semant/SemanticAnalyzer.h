/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "./TypeChecker.h"

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

    void
    analyze()
    {
        TypeChecker* tc = new TypeChecker();
        tc->check(root);
        tc->end();
    }
};