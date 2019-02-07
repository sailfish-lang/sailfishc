/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

#include "DFA.h"
#include "Scanner.h"
#include <string>

class Lexar
{
  private:
    Scanner* scanner;
    DFA* dfa;
    int currentState;
    std::string buffer;

  public:
    // constructor
    Lexar(std::string filename);
    // will return EOF_TOKEN indefinitely once done parsing
    // Token* getToken();
    void getToken();
};
