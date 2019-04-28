/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * Simple utility class for formatting and displaying various types of errors.
 */
#pragma once
#include "../common/display.h"
#include <iostream>
#include <string>

class Error
{
  private:
    int col;
    int line;
    std::string msg;
    std::string left;
    std::string middle;
    std::string right;
    std::string errtype;
    std::string filename;

  public:
    Error(int c, int l, const std::string& m, const std::string& le,
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
    void
    setFilename(const std::string& f)
    {
        filename = f;
    }
    // helper method for pretty printing error message
    void
    displayMessage()
    {
        Prettify::Formatter red(Prettify::FG_RED);
        Prettify::Formatter def(Prettify::FG_DEFAULT);
        Prettify::Formatter underline(Prettify::UNDERLINE);
        Prettify::Formatter normal(Prettify::RESET);

        if (left == "" && middle == "" && right == "")
            std::cout << red << "[" << errtype << " ERROR at " + filename + " ["
                      << line << ":" << col << "]: " << msg << def << "\n";
        else
            std::cout << red << "[" << errtype << " ERROR at " + filename + " ["
                      << line << ":" << col << "]: " << msg << def << "\n\n\t"
                      << left << underline << middle << normal << right << "\n";
    }
    std::string
    getMsg()
    {
        return msg;
    }
};