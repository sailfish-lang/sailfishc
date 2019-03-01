/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "FunctionSymbol.h"

std::vector<std::string>
FunctionSymbol::getInputTypes()
{
    return inputs;
}

std::vector<std::string>
FunctionSymbol::getOutputTypes()
{
    return outputs;
}