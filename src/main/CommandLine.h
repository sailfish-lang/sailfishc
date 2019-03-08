/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Ast.h"
#include "../lexar/Lexar.h"
#include "../parser/Parser.h"
#include "../semant/SemanticAnalyzer.h"
#include "../visitor/InOrderTraversal.h"

int handleCommandLine(int, char* const* argv);