/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Lists.h"
#include <string>

const static std::string stdlib_c_HEADER =
    "\n//___________BEGIN_STDLIB__________/_//\n\n";

const static std::string stdlib_c_FOOTER =
    "\n//___________END_STDLIB__________/_//\n\n";

std::string getStdLibC();