/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

namespace ast
{
class GeneralDefinition
{
  public:
    enum GeneralDefinitionType
    {
        DictionaryDefinition,
        ListDefinition,
        NewUDTDefinition,
        PrimitiveDefition,
    };
    virtual GeneralDefinitionType getGeneralDefinitionType() = 0;
};
}