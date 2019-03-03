/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SymbolTableErrorHandler.h"
#include <iostream>

void
SymbolTableErrorHandler::handle(Error* err)
{
    // finish the initialization of Error
    err->setErrorType("Symbol Table");

    foundError = true;

    // throw an error since we stop at first error here
    std::string msg = err->getPrettyMessage();
    throw msg;
}

void
SymbolTableErrorHandler::end()
{
    if (!foundError)
        std::cout << "Symbal Table creation finished error free!\n";
    else
        std::cout
            << "Symbol Table creation stopped and found one or more errors.\n";
}