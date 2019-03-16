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
        DictionaryLiteral,
        ListLiteral,
        UserDefinedType,
    };
    virtual NewType getNewType() = 0;
};
}