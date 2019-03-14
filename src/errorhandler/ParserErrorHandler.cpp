/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "ParserErrorHandler.h"
#include <iostream>

void
ParserErrorHandler::handle(Error* err)
{
    // finish the initialization of Error
    err->setErrorType("Parser");

    // throw an error since we stop at first error here
    throw err->getPrettyMessage();
}
