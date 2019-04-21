/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalyzerErrorHandler.h"

void
SemanticAnalyzerErrorHandler::handle(std::unique_ptr<Error> err)
{
    // finish the initialization of Error
    err->setErrorType("SEMANTIC");

    err->setFilename(filename);

    // throw an error since we stop at first error here
    err->displayMessage();
    // throw "";
}
