/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "FunctionSymbol.h"

bool
FunctionSymbol::hasInputTypes()
{
    return true;
}

std::vector<std::string>
FunctionSymbol::getInputTypes()
{
    return inputs;
}

bool
FunctionSymbol::hasOutputTypes()
{
    return true;
}

std::vector<std::string>
FunctionSymbol::getOutputTypes()
{
    return outputs;
}