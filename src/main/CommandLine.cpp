/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "CommandLine.h"

void
helpMessage()
{
    std::cout << "Welcome to the Sailfish Compiler!\n"
                 "The Sailfish is hungry and wants to compile some code.\n"
                 "To get started, consider one of the following commands:\n"
                 "\n\tsailfishc [filename]\n"
                 "\n\tsailfishc --help\n"
                 "\n\tsailfishc --version\n"
                 "\n\tsailfishc --lex [filename]\n"
                 "\n\tsailfishc --parse [filename]\n";
}

void
versionInfo()
{
    std::cout << "sailfishc 0.2.0 (marlin)\n";
}

int
handleCommandLine(int argc, char* const* argv)
{
    switch (argc)
    {
    case 1:
        helpMessage();
        return 0;

    case 2:
    {
        if (std::string("--help").compare(argv[1]) == 0)
        {
            helpMessage();
        }
        else if (std::string("--version").compare(argv[1]) == 0)
        {
            versionInfo();
        }

        return 0;
    }

    case 3:
        if (std::string("--lex").compare(argv[1]) == 0)
        {
            std::string filename = argv[2];
            std::cout << "Lexing " << filename << ".\n\n";

            Lexar2* l = new Lexar2(filename);
            std::unique_ptr<Token2> t = l->getNextToken();

            while (t->kind != TokenKind::EOF_)
            {
                std::cout << t->prettifyFormatToken();
                t = l->getNextToken();
            }
        }
        else if (std::string("--parse").compare(argv[1]) == 0)
        {

            try
            {
                auto filename = argv[2];
                std::cout << "Parsing " << filename << ".\n\n";

                Parser2* p = new Parser2(filename);
                auto n = p->parse();
                // p->getUDTTable()->dump();
                // p->getSymbolTable()->dump();
                // std::cout << '\n';
                // p->postorder(std::move(n), [](std::string s) -> void {
                //     std::cout << s << '\n';
                // });
                // p->display(std::move(n));
            }
            catch (const std::string msg)
            {
                std::cerr << msg;
            }
            catch (char const* msg)
            {
                std::cerr << msg;
            }
        }
        else
        {
            std::cout
                << "Unrecognized flag. "
                   "Try again or type sailfishc to see command options!\n";
        }

        return 0;

    default:
        std::cout << "Unexpected number of arguments. "
                     "Try again or type sailfishc to see command options!\n";
        return 0;
    }
}
