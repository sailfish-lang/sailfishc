/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../semantics/UDTTable.h"
#include "../visitor/Visitor.h"
#include <fstream>
#include <iostream>
#include <string>

const std::string OUTPUT_HEADER =
    "/**\n * Do not alter! This code is generated by the sailfishc\n * "
    "compiler and might/will break if anything changed.\n */\n#include "
    "\"stdlib/stdlib.h\"\n\n";

class Transpiler : public Visitor
{
  private:
    ast::Start* root;
    std::string filename;
    std::string fileBuffer;
    UDTTable* udtTable;
    SymbolTable* symbolTable;

  public:
    using Visitor::visit;

    // constructor
    Transpiler(ast::Start* r, std::string f, UDTTable* u, SymbolTable* s)
    {
        root = r;
        filename = f;
        udtTable = u;
        symbolTable = s;

        // set the file header here
        fileBuffer = OUTPUT_HEADER;
    }

    // destructor
    ~Transpiler()
    {
        delete root;
    }

    void transpile();

    // initial execution body
    void visit(ast::InitialExecutionBody*);

    // function declerations
    void visit(ast::FunctionDefinition*);

    // special function definition for insertion of udt param
    void visit(ast::FunctionDefinition*, std::string);

    // function call
    void visit(ast::FunctionCall*);

    // variable
    void visit(ast::Variable*);

    // typename
    void visit(ast::Typename*);

    // literals
    void visit(ast::Identifier*);
    void visit(ast::BooleanLiteral*);
    void visit(ast::IntegerLiteral*);
    void visit(ast::FloatLiteral*);
    void visit(ast::StringLiteral*);

    // return statement
    void visit(ast::ReturnStatement*);

    // blocks
    void visit(ast::Block*);

    // if statement
    void visit(ast::IfStatement*);

    // binaries
    void visit(ast::BinaryCompOrArith*);
    void visit(ast::Assignment*);

    // unaries
    void visit(ast::Negation*);

    // grouping statement
    void visit(ast::GroupingExpression*);

    // primtive definition
    void visit(ast::PrimitiveDefition* node);

    // expression only binary expression
    void visit(ast::ExpressionOnlyStatement*);

    // user defined type
    void visit(ast::UserDefinedTypeDefinition*);

    // declared udt
    void visit(ast::NewUDTDefinition*);
    void visit(ast::UserDefinedType* node);

    // member accessors for udts
    void visit(ast::AttributeAccess*);
    void visit(ast::MethodAccess*);
};