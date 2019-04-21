/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "ParserErrorHandler.h"

void
ParserErrorHandler::handle(std::unique_ptr<Error> err)
{
    // finish the initialization of Error
    err->setErrorType("PARSER");

    // throw an error since we stop at first error here
    err->displayMessage();
    throw "";
}
