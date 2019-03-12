/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../common/ReservedWords.h"
#include "../errorhandler/Error.h"
#include "../errorhandler/SemanticErrorHandler.h"
#include "../errorhandler/SymbolTableErrorHandler.h"
#include "../visitor/Visitor.h"
#include "CommonExtractions.h"
#include "SymbolTable.h"
#include "UDTTable.h"
#include <tuple>

class TypeChecker : public Visitor
{
  private:
    SymbolTableErrorHandler* symbolTableErrorHandler;
    SemanticErrorHandler* semanticErrorHandler;
    SymbolTable* symbolTable;
    UDTTable* udtTable;

  public:
    using Visitor::visit;

    // constructor
    TypeChecker(SymbolTable* st, SymbolTableErrorHandler* steh,
                SemanticErrorHandler* seh, UDTTable* udtt)
    {
        symbolTableErrorHandler = steh;
        semanticErrorHandler = seh;
        symbolTable = st;
        udtTable = udtt;
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

    std::vector<Error*>
    getSemanticErrors()
    {
        return semanticErrorHandler->getErrors();
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

    // binary expressions
    void visit(ast::BinaryExpression*);

    // unary
    void visit(ast::Negation*);

    // only global scopes
    void visit(ast::ExportDefinition*);

    // udt attribute and method access
    void visit(ast::AttributeAccess*);
    void visit(ast::MethodAccess*);

    // function call
    void visit(ast::FunctionCall*);

    // grouping
    void visit(ast::GroupingExpression*);
};