/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "LexarErrorHandler.h"
#include <iostream>

void
LexarErrorHandler::handle(Error* err)
{
    // finish the initialization of Error
    err->setErrorType("Lexar");

    // throw an error since we stop at first error here
    throw err->getPrettyMessage();
}

void
LexarErrorHandler::end()
{
    if (!foundError)
        std::cout << "Lexed error free!\n";
    else
        std::cout << "Lexed and found one or more errors.\n";
}
