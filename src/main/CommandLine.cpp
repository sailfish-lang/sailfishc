/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "../ast/Ast.h"
#include "../lexar/Lexar.h"
#include "../parser/Parser.h"
#include "../semant/SemanticAnalyzer.h"
#include "../visitor/InOrderTraversal.h"
#include <iostream>
#include <string>

void
helpMessage()
{
    std::cout << "Welcome to the Sailfish Compiler!\n"
                 "The Sailfish is hungry and wants to compile some code.\n"
                 "To get started, consider one of the following commands:\n"
                 "\n\tsailfishc [filename]\n"
                 "\n\tsailfishc --help\n"
                 "\n\tsailfishc --version\n"
                 "\n\tsailfishc --lex_only [filename]\n"
                 "\n\tsailfishc --parse_only [filename]\n\n";
}

void
versionInfo()
{
    std::cout << "sailfishc 0.0.1\n";
}

void
fullCompilation()
{
    std::cout << "Compilation not yet possible. It is the early days, the days "
                 "of cavemen, drawing images on cave walls.\n"
                 "These men and women have yet to see the light and yet to "
                 "find any oceans. Besides the worship of the water diety, \n"
                 "they have no concept of the mythical sailfish, the fastest "
                 "fish in the sea ++. One day, they will know, one day. \n"
                 "TLDR: not done yet, please check back later and leave a star "
                 "on the repo :).\n";
}

int
handleCommandLine(int argc, char* const* argv)
{
    switch (argc)
    {
    case 1:
        helpMessage();
        return 1;

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
        else
        {
            // default is to just try to compile, assuming received text is a
            // filename
            fullCompilation();
        }

        return 1;
    }

    case 3:
        if (std::string("--lex_only").compare(argv[1]) == 0)
        {
            std::string filename = argv[2];
            std::cout << "Lexing " << filename << ".\n\n";

            Lexar* l = new Lexar(filename);
            Token* t = l->getNextToken();

            while (!t->isEOF())
            {
                t->display();
                t = l->getNextToken();
            }
        }
        else if (std::string("--parse_only").compare(argv[1]) == 0)
        {

            try
            {
                std::string filename = argv[2];
                std::cout << "Parsing " << filename << ".\n\n";

                Parser* p = new Parser();
                ast::Start* root = p->parse(filename);
                InOrderTraversal* iot = new InOrderTraversal();
                for (auto const& n : iot->getInOrderTraversal(root))
                {
                    std::cout << n << "\n";
                }
            }
            catch (const std::string msg)
            {
                std::cerr << msg;
            }
        }
        else if (std::string("--semantic_analysis_only").compare(argv[1]) == 0)
        {
            try
            {
                std::string filename = argv[2];
                std::cout << "Analyzing " << filename << ".\n\n";

                Parser* p = new Parser();
                ast::Start* root = p->parse(filename);

                SemanticAnalyzer* s = new SemanticAnalyzer(root);
                s->analyze();
            }
            catch (const std::string msg)
            {
                std::cerr << msg;
            }
        }

        return 1;

    default:
        std::cout << "Unexpected number of arguments. "
                     "Try again or type sailfishc to see command opetions!\n";
        return 0;
    }
}