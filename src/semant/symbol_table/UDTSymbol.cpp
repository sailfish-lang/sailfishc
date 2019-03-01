/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "UDTSymbol.h"

SymbolTable*
UDTSymbol::getAttributes()
{
    return attributes;
}

SymbolTable*
UDTSymbol::getMethods()
{
    return methods;
}