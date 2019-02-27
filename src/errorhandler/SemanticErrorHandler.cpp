/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticErrorHandler.h"
#include <iostream>

void
SemanticErrorHandler::handle(Error* err)
{
    // finish the initialization of Error
    err->setErrorType("Semantic");

    // collect the error
    errors.push_back(err);
}

void
SemanticErrorHandler::end()
{
    if (errors.size() == 0)
        std::cout << "Semantic error free!\n";
    else
    {
        std::cout
            << "Performed Semantuc Analysis and found one or more errors:\n";

        for (auto const error : errors)
        {
            std::cout << error->getPrettyMessage();
        }
    }
}
