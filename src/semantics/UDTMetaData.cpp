/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "UDTMetaData.h"

SymbolTable*
UDTMetaData::getAttributeSymbolTable()
{
    return attributes;
}

SymbolTable*
UDTMetaData::getMethodSymbolTable()
{
    return methods;
}

SymbolTable*
UDTMetaData::setMethodSymbolTable(SymbolTable* m)
{
    methods = m;
}
