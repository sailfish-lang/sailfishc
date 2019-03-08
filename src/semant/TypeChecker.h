/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../errorhandler/Error.h"
#include "../errorhandler/ErrorHandler.h"
#include "../errorhandler/SemanticErrorHandler.h"
#include "../errorhandler/SymbolTableErrorHandler.h"
#include "../visitor/Visitor.h"
#include "SymbolTable.h"
#include "UDTTable.h"

class TypeChecker : public Visitor
{
  private:
    ErrorHandler* symbolTableErrorHandler;
    ErrorHandler* semanticErrorHandler;
    SymbolTable* symbolTable;
    UDTTable* udtTable;

  public:
    using Visitor::visit;

    // constructor
    TypeChecker()
    {
        symbolTableErrorHandler = (ErrorHandler*)new SymbolTableErrorHandler();
        semanticErrorHandler = (ErrorHandler*)new SemanticErrorHandler();
        symbolTable = new SymbolTable();
    }
    // destructor
    ~TypeChecker()
    {
    }

    // TODO: move this somewhere more reasonable
    void
    end()
    {
        symbolTableErrorHandler->end();
        semanticErrorHandler->end();
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

    // udt's
    void visit(ast::UserDefinedTypeDefinition*);

    // all nodes containing block statements must create enter/exit scopes
    void visit(ast::InitialExecutionBody*);
    void visit(ast::IfStatement*);

    // binary logic and comparison
    void visit(ast::BinaryGreaterThan*);
    void visit(ast::BinaryLessThan*);
    void visit(ast::BinaryGreaterThanOrEqual*);
    void visit(ast::BinaryLessThanOrEqual*);
    void visit(ast::EquivalenceComparison*);
    void visit(ast::NonEquivalenceComparison*);
    void visit(ast::AndComparison*);
    void visit(ast::OrComparison*);

    // all arith
    virtual void visit(ast::Exponentiation*);
    virtual void visit(ast::Multiplication*);
    virtual void visit(ast::Division*);
    virtual void visit(ast::Modulo*);
    virtual void visit(ast::Addition*);
    virtual void visit(ast::Subtraction*);

    // unary
    void visit(ast::Negation*);

    // only global scopes
    virtual void visit(ast::ExportDefinition*);
};