/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Expression.h"
#include "GeneralDefinition.h"
#include "Identifier.h"
#include "Node.h"

namespace ast
{
class DictionaryDefinition : public GeneralDefinition, public Node
{
  private:
    Identifier* name;
    Expression* expr;

  public:
    // constructor
    DictionaryDefinition(Identifier* i, Expression* e, int lineNum)
        : Node(lineNum)
    {
        name = i;
        expr = e;
    };
    // destructor
    ~DictionaryDefinition()
    {
        delete expr;
    };
    // implement GeneralDefinition
    /* virtual */ GeneralDefinitionType
    getGeneralDefinitionType()
    {
        return GeneralDefinitionType::DictionaryDefinition;
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    Expression*
    getExpression()
    {
        return expr;
    }
};
}