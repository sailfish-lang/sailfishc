/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Visitor.h"
#include <iostream>

class InOrderTraversal : public Visitor
{
  private:
    std::vector<std::string> traversalVec;

    void push(std::string val);

  public:
    using Visitor::visit;

    // constructor
    InOrderTraversal(){};
    // destructor
    ~InOrderTraversal(){};

    // overloaded, overriden methods
    void visit(ast::NewUDTDefinition*);
    void visit(ast::PrimitiveDefition*);
    void visit(ast::FunctionDefinition*);
    void visit(ast::Input*);
    void visit(ast::Output*);
    void visit(ast::UserDefinedTypeDefinition*);
    void visit(ast::UserDefinedType*);
    void visit(ast::InitialExecutionBody*);
    void visit(ast::Variable*);
    void visit(ast::IfStatement*);
    void visit(ast::ReturnStatement*);
    void visit(ast::NewExpression*);
    void visit(ast::AttributeAccess*);
    void visit(ast::MethodAccess*);
    void visit(ast::FunctionCall*);
    void visit(ast::BooleanLiteral*);
    void visit(ast::IntegerLiteral*);
    void visit(ast::FloatLiteral*);
    void visit(ast::StringLiteral*);
    void visit(ast::Identifier*);
    void visit(ast::Typename*);
    void visit(ast::Negation*);
    void visit(ast::BinaryCompOrArith*);
    void visit(ast::Assignment*);

    // return a vector of strings in an order traversal
    std::vector<std::string> getInOrderTraversal(ast::Start*);
};