/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalyzer.h"

void
SemanticAnalyzer::analyze()
{
    TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                      semanticErrorHandler, udtTable);

    // run type checker and symbol table creator
    tc->check(root);

    InitialExecution* ie = new InitialExecution();
    if (!ie->hasInitExecBody(root))
    {
        std::string errormsg =
            "Unable to compile. Reason: missing an initial execution body.\n";
        throw errormsg;
    }

    // execute on built up errors
    tc->end();
}

std::vector<Error*>
SemanticAnalyzer::testAnalyze()
{
    TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                      semanticErrorHandler, udtTable);

    tc->check(root);
    return tc->getSemanticErrors();
}