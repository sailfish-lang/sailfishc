/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Parser2ErrorHandler.h"
#include <iostream>

void
Parser2ErrorHandler::handle(Error2* err)
{
    // finish the initialization of Error
    err->setErrorType("PARSER");

    // throw an error since we stop at first error here
    err->displayMessage();
    throw "";
}
