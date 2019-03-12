/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalyzer.h"

void
SemanticAnalyzer::analyze()
{
    // add built-in print method
    symbolTable->addSymbol("display", "Fdisplay{_str}(_void)");

    TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                      semanticErrorHandler, udtTable);

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
    TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                      semanticErrorHandler, udtTable);

    tc->check(root);
    return tc->getSemanticErrors();
}