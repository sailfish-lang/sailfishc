/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "UDTSymbol.h"

bool
UDTSymbol::hasAttributes()
{
    return true;
}

SymbolTable*
UDTSymbol::getAttributes()
{
    return attributes;
}

bool
UDTSymbol::hasMethods()
{
    return true;
}

SymbolTable*
UDTSymbol::getMethods()
{
    return methods;
}