/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "DictionarySymbol.h"

bool
DictionarySymbol::hasKeyType()
{
    return true;
}

std::string
DictionarySymbol::getKeyType()
{
    return keyType;
}

bool
DictionarySymbol::hasValueType()
{
    return true;
}

std::string
DictionarySymbol::getValueType()
{
    return valueType;
}
