/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class New
{
  public:
    enum NewType
    {
        ListLiteral,
        DictionaryLiteral,
        UserDefinedType,
    };
    virtual NewType getNewType() = 0;
};
}