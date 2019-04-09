/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "display.h"
#include <iostream>
#include <string>

class Error2
{
  private:
    int col;
    int line;
    std::string msg;
    std::string left;
    std::string middle;
    std::string right;
    std::string errtype;

  public:
    Error2(int c, int l, const std::string& m, const std::string& le,
           const std::string& mid, const std::string& ri)
    {
        col = c;
        line = l;
        msg = m;
        left = le;
        middle = mid;
        right = ri;
    }
    // set method for receiving type to utilize internally
    void
    setErrorType(const std::string& e)
    {
        errtype = e;
    }
    // helper method for pretty printing error message
    void
    displayMessage()
    {
        Prettify::Formatter red(Prettify::FG_RED);
        Prettify::Formatter def(Prettify::FG_DEFAULT);
        Prettify::Formatter underline(Prettify::UNDERLINE);
        Prettify::Formatter normal(Prettify::RESET);

        std::cout << red << "[" << errtype << " ERROR at [" << line << ":"
                  << col << "]: " << msg << def << "\n\n\t" << left << underline
                  << middle << normal << right << "\n";
    }
};