/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalyzer.h"

void
SemanticAnalyzer::analyze()
{
    // add built-in print method
    symbolTable->addSymbol("display_str", "Fdisplay{_str}(_void)");
    symbolTable->addSymbol("display_int", "Fdisplay{_int}(_void)");
    symbolTable->addSymbol("display_flt", "Fdisplay{_flt}(_void)");
    symbolTable->addSymbol("display_bool", "Fdisplay{_bool}(_void)");

    TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                      semanticErrorHandler, udtTable);

    // run type checker and symbol table creator
    tc->check(root);

    InitialExecution* ie = new InitialExecution();
    if (!ie->hasInitExecBody(root))
    {
        semanticErrorHandler->handle(new Error(
            0, "No initial execution body found, or more than one exists."));
    }

    // execute on built up errors
    tc->end();

    if (semanticErrorHandler->getErrors().size() > 0)
    {
        throw "Unable to compile.";
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