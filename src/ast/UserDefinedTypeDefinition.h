/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "SourcePart.h"

namespace ast
{
class UserDefinedTypeDefinition : public SourcePart, public Node
{
  private:
    ast::Identifier* name;
    std::vector<Variable*> attributes;
    std::vector<FunctionDefinition*> methods;

  public:
    // constructor
    UserDefinedTypeDefinition(ast::Identifier* n, std::vector<Variable*> a,
                              std::vector<FunctionDefinition*> m, int lineNum)
        : Node(lineNum)
    {
        name = n;
        attributes = a;
        methods = m;
    }
    // destructor
    ~UserDefinedTypeDefinition(){};
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::UserDefinedTypeDefinition;
    }
    // get methods
    std::vector<Variable*>
    getAttributes()
    {
        return attributes;
    }
    std::vector<FunctionDefinition*>
    getMethods()
    {
        return methods;
    }
    Identifier*
    getName()
    {
        return name;
    }
};
}