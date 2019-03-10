/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../common/ReservedWords.h"
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
        udtTable = new UDTTable();
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
    void visit(ast::PrimitiveDefition*);

    // list
    void visit(ast::ListDefinition*);

    // dictionary
    void visit(ast::DictionaryDefinition*);

    // function
    void visit(ast::FunctionDefinition*);

    // udt's
    void visit(ast::UserDefinedTypeDefinition*);
    void visit(ast::NewUDTDefinition*);

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
    void visit(ast::Exponentiation*);
    void visit(ast::Multiplication*);
    void visit(ast::Division*);
    void visit(ast::Modulo*);
    void visit(ast::Addition*);
    void visit(ast::Subtraction*);

    // unary
    void visit(ast::Negation*);

    // only global scopes
    void visit(ast::ExportDefinition*);

    // udt attribute and method access
    void visit(ast::MemberAccess*);
};