#include "InitialExecution.h"
#include "Visitor.h"
#include <iostream>

// for every initial execution body, increment the count
void
InitialExecution::visit(ast::InitialExecutionBody* node)
{
    initExecCount++;
}

bool
InitialExecution::hasInitExecBody(ast::Start* node)
{
    // initialize the count
    initExecCount = 0;

    // traverse the ast
    visit(node);

    // return true only if exactly one initial execution body
    return initExecCount == 1;
}