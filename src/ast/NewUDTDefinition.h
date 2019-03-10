/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "GeneralDefinition.h"
#include "NewExpression.h"
#include "Node.h"
#include "Variable.h"

namespace ast
{
class NewUDTDefinition : public GeneralDefinition, public Node
{
  private:
    Variable* var;
    NewExpression* expr;

  public:
    // constructor
    NewUDTDefinition(Variable* v, NewExpression* e, int lineNum) : Node(lineNum)
    {
        var = v;
        expr = e;
    };
    // destructor
    ~NewUDTDefinition()
    {
        delete expr;
        delete var;
    };
    // implement GeneralDefinition
    /* virtual */ GeneralDefinitionType
    getGeneralDefinitionType()
    {
        return GeneralDefinitionType::NewUDTDefinition;
    }
    // get methods
    Variable*
    getVariable()
    {
        return var;
    }
    NewExpression*
    getExpression()
    {
        return expr;
    }
};
}