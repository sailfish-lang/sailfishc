/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Scanner.h"
#include <fstream>
#include <string>

Scanner::Scanner(std::string filename)
{
    file.open(filename, std::fstream::in);
}

char
Scanner::getChar()
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

void
Scanner::putBackChar(char c)
{
    file.putback(c);
}