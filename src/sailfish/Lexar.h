/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

#include "Scanner.h"
#include <string>

#include <iostream>

class Lexar
{
  private:
    Scanner* scanner;

  public:
    // constructor
    Lexar(std::string filename);
    // will return EOF_TOKEN indefinitely once done parsing
    // Token* getToken();
};
