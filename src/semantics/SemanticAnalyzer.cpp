/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalyzer.h"

void
SemanticAnalyzer::analyze()
{
    // add standard library methods
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