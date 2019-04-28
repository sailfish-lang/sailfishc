/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * CommandLine handles the basic command line utilities for the compiler.
 */
#pragma once
#include "../common/display.h"
#include "../lexar/Lexar.h"
#include "../sailfish/sailfishc.h"
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <string>

int handleCommandLine(int, char* const* argv);