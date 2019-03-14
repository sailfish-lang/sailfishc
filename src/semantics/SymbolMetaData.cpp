/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SymbolMetaData.h"

std::string
SymbolMetaData::getType()
{
    return type;
}

int
SymbolMetaData::getScopeLevel()
{
    return scopeLevel;
}
