/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <fstream>
#include <string>

class Scanner
{
  private:
    std::ifstream file;

  public:
    // constructor takes in a filename
    Scanner(std::string);
    // destructor
    ~Scanner(){};
    // retreive next character from the ifstream
    char getNextChar();
    // putback char
    void putBackChar(char);
};