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
                 "\n\tsailfishc --parse_only [filename]\n"
                 "\n\tsailfishc --semantic_analysis_only [filename]\n"
                 "\n\tsailfishc --compile_c [filename]\n"
                 "\n\tsailfishc --compile_and_execute [filename]\n\n";
}

void
versionInfo()
{
    std::cout << "sailfishc 0.2.0 (dorsalfin)\n";
}

bool
compileC()
{
    // check to see if we can use system
    if (!system(NULL))
    {
        std::cout << "System command processor doesn't exist. Please compile "
                     "sailfishc generated C code yourself.\n";
        return false;
    }

    std::cout << "EXECUTING: gcc out.c\n";
    system("gcc out.c");

    std::cout << "gcc compiled out.c to: a.out\n";

    return true;
}

void
executeBinary()
{
    std::cout << "EXECUTING: ./a.out.\n";
    system("./a.out");
}

bool
fullCompilation(const std::string filename)
{
    try
    {
        std::cout << "Compiling " << filename << ".\n";

        Parser* p = new Parser();
        ast::Start* root = p->parse(filename);

        SemanticAnalyzer* s = new SemanticAnalyzer(root);
        s->analyze();

        std::string cFilename = "out.c";

        Transpiler* t = new Transpiler(root, cFilename, s->getUDTTable(),
                                       s->getSymbolTable());
        t->transpile();

        std::cout << "Success. sailfishc compiled " + filename + " to: out.c\n";

        return true;
    }
    catch (const std::string msg)
    {
        std::cerr << msg;
        return false;
    }
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
        else
        {
            fullCompilation(argv[1]);
        }

        return 0;
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
        else if (std::string("--test_lex").compare(argv[1]) == 0)
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
        else if (std::string("--parse_only").compare(argv[1]) == 0)
        {

            try
            {
                std::string filename = argv[2];
                std::cout << "Parsing " << filename << ".\n\n";

                Parser* p = new Parser();
                ast::Start* root = p->parse(filename);
            }
            catch (const std::string msg)
            {
                std::cerr << msg;
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
                p->postorder(n);
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
        else if (std::string("--compile_c").compare(argv[1]) == 0)
        {
            // compile sailfish
            if (fullCompilation(argv[2]))
            {
                // compile c
                compileC();
            }
        }
        else if (std::string("--compile_and_execute").compare(argv[1]) == 0)
        {
            // compile sailfish
            if (fullCompilation(argv[2]))
            {
                // compile c
                if (compileC())
                {
                    // execute gcc generated binary
                    executeBinary();
                }
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
