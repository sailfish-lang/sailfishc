/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../ast/Node.h"
#include "../lexar/Lexar.h"
#include "../lexar/Token.h"
#include <string>
#include <vector>

class Parser
{
  private:
    Lexar* lexar;
    Token* currentToken; // LL(1)

    // helper function for ignoring extraneous tokens
    Token* getNextUsefulToken();

    // parse methods
    ast::Start* parseStart();

  public:
    // constructor
    Parser();
    // actual parse function -- given filename returns the root of the ast
    ast::Start* parse(const std::string);
};
