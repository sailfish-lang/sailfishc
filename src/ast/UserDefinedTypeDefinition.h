/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SourcePart.h"
#include "UserDefinedTypeAttributes.h"
#include "UserDefinedTypeMethods.h"

namespace ast
{
class UserDefinedTypeDefinition : public SourcePart
{
  private:
    UserDefinedTypeAttributes* attributes;
    // methods are optional
    UserDefinedTypeMethods* methods;

  public:
    // constructor
    UserDefinedTypeDefinition(UserDefinedTypeAttributes* a,
                              UserDefinedTypeMethods* m)
    {
        attributes = a;
        methods = m;
    }
    // destructor
    ~UserDefinedTypeDefinition()
    {
        delete attributes;
        delete methods;
    };
    // implementation for SourcePart
    /* virtual */ SourcePartType
    getSourcePartType()
    {
        return SourcePartType::UserDefinedTypeDefinition;
    }
    // get methods
    UserDefinedTypeAttributes*
    getAttributes()
    {
        return attributes;
    }
    UserDefinedTypeMethods*
    getMethods()
    {
        return methods;
    }
};
}