/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Visitor.h"

class InitialExecution : public Visitor
{
  private:
    int initExecCount;

  public:
    // Thought I was losing my mind, unable to override overloaded methods.
    // Turns out this is a "gotchya" in C++:
    // https://stackoverflow.com/questions/888235/overriding-a-bases-overloaded-function-in-c
    using Visitor::visit;

    // constructor
    InitialExecution(){};

    // destructor
    ~InitialExecution(){};

    // overloaded, overriden methods
    void visit(ast::InitialExecutionBody*);

    // return true or false depending on if one (and only one) initial execution
    // body is found
    bool hasInitExecBody(ast::Start*);
};