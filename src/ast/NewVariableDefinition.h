/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ExpressionStatement.h"
#include "GeneralDefinition.h"
#include "Node.h"
#include "Variable.h"

namespace ast
{
class NewVariableDefinition : public GeneralDefinition, public Node
{
  private:
    Variable* var;
    ExpressionStatement* expr;

  public:
    // constructor
    NewVariableDefinition(Variable* v, ExpressionStatement* e, int lineNum)
        : Node(lineNum)
    {
        var = v;
        expr = e;
    };
    // destructor
    ~NewVariableDefinition()
    {
        delete expr;
        delete var;
    };
    // implement GeneralDefinition
    /* virtual */ GeneralDefinitionType
    getGeneralDefinitionType()
    {
        return GeneralDefinitionType::NewVariableDefinition;
    }
    // get methods
    Variable*
    getVariable()
    {
        return var;
    }
    ExpressionStatement*
    getExpressionStatement()
    {
        return expr;
    }
};
}