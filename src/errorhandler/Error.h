/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <string>

// data structure describing an error
class Error
{
  private:
    int line;
    int col;
    std::string msg;
    std::string errtype;

  public:
    // constructor
    Error(int l, int c, std::string m)
    {
        line = l;
        col = c;
        msg = m;
    }
    // destructor
    ~Error()
    {
    }
    // set method for receiving type to utilize internally
    void
    setErrorType(std::string e)
    {
        errtype = e;
    }
    // helper method for pretty printing error message with all (well, not
    // actually all...) information
    std::string
    getPrettyMessage()
    {
        return "[" + errtype + " Error at line: " + std::to_string(line) +
               " ; column: " + std::to_string(col) + "]: " + msg + "\n";
    }
}