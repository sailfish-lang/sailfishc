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
    Scanner(const std::string filename)
    {
        file.open(filename, std::fstream::in);
    }
    // destructor
    ~Scanner(){};
    // retreive next character from the ifstream
    char
    getNextChar()
    {
        char ch;
        int c = file.peek();

        // possible cause for future errors here
        if (c == EOF)
        {
            ch = -1;
        }
        else
            file.get(ch);
        return ch;
    }
    // putback char
    void
    putBackChar(char c)
    {
        file.putback(c);
    }
};