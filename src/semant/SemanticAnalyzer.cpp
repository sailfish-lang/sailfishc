/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalyzer.h"

void
SemanticAnalyzer::analyze()
{
    TypeChecker* tc = new TypeChecker();

    // run type checker and symbol table creator
    tc->check(root);
    tc->end();

    InitialExecution* ie = new InitialExecution();
    if (!ie->hasInitExecBody(root))
    {
        std::cout
            << "No initial execution body found, or more than one exists.";
    }
}

std::vector<Error*>
SemanticAnalyzer::testAnalyze()
{
    TypeChecker* tc = new TypeChecker();
    tc->check(root);
    return tc->getSemanticErrors();
}