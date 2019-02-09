/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "../sailfish/Lexar.h"
#include <iostream>
#include <string>

int
handleCommandLine(int argc, char* const* argv)
{
    switch (argc)
    {
    case 1:
        std::cout << "Welcome to the Sailfish Compiler!\n"
                     "The Sailfish is hungry and wants to compile some code.\n"
                     "To get started, consider one of the following commands:\n"
                     "\n\tsailfishc [filename]\n\n";
        return 1;
    case 2:
    {
        std::string filename = argv[1];
        std::cout << "Compiling " << filename << ".\n\n";

        Lexar* lexar = new Lexar(filename);

        Token* t;
        t = lexar->getToken();
        while (!t->isEOF())
        {
            t->display();
            t = lexar->getToken();
        }

        return 1;
    }
    default:
        std::cout << "Unexpected number of arguments. "
                     "Try again or type sailfishc to see command opetions!\n";
        return 0;
    }
}