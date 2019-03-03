/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../errorhandler/ErrorHandler.h"
#include "../errorhandler/SymbolTableErrorHandler.h"
#include "../visitor/Visitor.h"
#include "SymbolTable.h"

class TypeChecker : public Visitor
{
  private:
    ErrorHandler* symbolTableErrorHandler;
    SymbolTable* symbolTable;

  public:
    using Visitor::visit;

    // constructor
    TypeChecker()
    {
        symbolTableErrorHandler = (ErrorHandler*)new SymbolTableErrorHandler();
        symbolTable = new SymbolTable();
    }
    // destructor
    ~TypeChecker()
    {
    }

    // visit all nodes and analyze
    void check(ast::Start*);

    // // primitives
    void visit(ast::NewVariableDefinition*);

    // // list
    virtual void visit(ast::ListDefinition*);

    // // dictionary
    virtual void visit(ast::DictionaryDefinition*);

    // // function
    // virtual void visit(ast::FunctionDefinition*);

    // // udt
    // virtual void visit(ast::UserDefinedTypeDefinition*);
};