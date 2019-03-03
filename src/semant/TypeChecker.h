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

    // primitives
    void visit(ast::NewVariableDefinition*);

    // list
    void visit(ast::ListDefinition*);

    // dictionary
    void visit(ast::DictionaryDefinition*);

    // function
    void visit(ast::FunctionDefinition*);

    // udt
    void visit(ast::UserDefinedTypeDefinition*);

    // all nodes containing block statements must create enter/exit scopes
    virtual void visit(ast::InitialExecutionBody*);
    virtual void visit(ast::IfStatement*);
};