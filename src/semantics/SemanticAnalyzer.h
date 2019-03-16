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
    SymbolTableErrorHandler* symbolTableErrorHandler;
    SemanticErrorHandler* semanticErrorHandler;
    SymbolTable* symbolTable;
    UDTTable* udtTable;

  public:
    // constructor
    SemanticAnalyzer(ast::Start* r)
    {
        symbolTableErrorHandler = new SymbolTableErrorHandler();
        semanticErrorHandler = new SemanticErrorHandler();
        symbolTable = new SymbolTable();
        udtTable = new UDTTable();
        root = r;
    };
    // deconstructor
    ~SemanticAnalyzer()
    {
        delete root;
        delete symbolTableErrorHandler;
        delete semanticErrorHandler;
        delete symbolTable;
        delete udtTable;
    };

    void
    analyze()
    {
        TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                          semanticErrorHandler, udtTable);

        // run type checker and symbol table creator
        tc->check(root);

        InitialExecution* ie = new InitialExecution();
        if (!ie->hasInitExecBody(root))
        {
            std::string errormsg = "Unable to compile. Reason: missing an "
                                   "initial execution body.\n";
            throw errormsg;
        }

        // execute on built up errors
        tc->end();
    }

    // strictly a helper for writing tests that returns all the semantic errors
    std::vector<Error*>
    testAnalyze()
    {
        TypeChecker* tc = new TypeChecker(symbolTable, symbolTableErrorHandler,
                                          semanticErrorHandler, udtTable);

        tc->check(root);
        return tc->getSemanticErrors();
    }

    // get methods
    UDTTable*
    getUDTTable()
    {
        return udtTable;
    }
    SymbolTable*
    getSymbolTable()
    {
        return symbolTable;
    }
};