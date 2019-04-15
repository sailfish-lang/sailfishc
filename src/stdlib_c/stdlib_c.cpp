/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "stdlib_c.h"

std::string
getStdLibC()
{
    return stdlib_c_HEADER + getListsStdLibC() + stdlib_c_FOOTER;
}