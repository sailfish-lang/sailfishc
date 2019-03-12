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
                 "\n\tsailfishc --lex_only [filename]\n"
                 "\n\tsailfishc --parse_only [filename]\n\n";
}

void
versionInfo()
{
    std::cout << "sailfishc 0.0.1\n";
}

void
fullCompilation(std::string filename)
{
    try
    {
        std::cout << "Compiling " << filename << ".\n\n";

        Parser* p = new Parser();
        ast::Start* root = p->parse(filename);

        SemanticAnalyzer* s = new SemanticAnalyzer(root);
        s->analyze();

        std::string cFilename = "out.c";

        Transpiler* t = new Transpiler(root, cFilename);
        t->transpile();
    }
    catch (const std::string msg)
    {
        std::cerr << msg;
    }
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
            fullCompilation(argv[1]);
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
                p->checkErrors();
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