/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "CommandLine.h"

const static std::string VERSION = "sailfishc 0.2.0 (marlin)";

void
helpMessage()
{
    Prettify::Formatter bold(Prettify::BOLD);
    Prettify::Formatter white(Prettify::FG_WHITE);
    Prettify::Formatter blue(Prettify::FG_LIGHT_BLUE);
    Prettify::Formatter normal(Prettify::RESET);

    std::cout << blue << "\t🐟 " << white << "Welcome" << blue << " to " << white
              << "the " << blue << "Sailfish " << white << "Compiler " << blue
              << "🐟\n"
              << normal
              << "The Sailfish is hungry and wants to compile some code.\n"
                 "To get started, consider one of the following commands:\n"
              << bold
              << "\n\tsailfishc [filename]\n"
                 "\n\tsailfishc --help\n"
                 "\n\tsailfishc --version\n"
              << normal;
}

void
versionInfo()
{
    Prettify::Formatter white(Prettify::FG_WHITE);
    Prettify::Formatter blue(Prettify::BG_BLUE);
    Prettify::Formatter normal(Prettify::RESET);

    std::cout << white << blue << VERSION << '\n' << normal;
}

void
fullCompilation(const std::string& filename)
{
    try
    {
        Prettify::Formatter white(Prettify::FG_WHITE);
        Prettify::Formatter green(Prettify::FG_GREEN);
        Prettify::Formatter blue(Prettify::FG_LIGHT_BLUE);
        Prettify::Formatter normal(Prettify::RESET);

        std::cout << blue << "\t🐟 " << white << "The Sailfish" << blue
                  << " Compiler " << white << VERSION << blue << " 🐟\n"
                  << normal;

        std::cout << "Compiling " << blue << filename << normal << ".\n";

        Parser* p = new Parser(filename);
        p->parse();

        std::cout << green << "Successfully compiled: " << normal << blue
                  << filename << "\n"
                  << normal;

        p->transpile();

        std::cout << green << "Successfully wrote compiled code to: " << normal
                  << blue << " out.c\n"
                  << normal;
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

    default:
        Prettify::Formatter blue(Prettify::FG_LIGHT_BLUE);
        Prettify::Formatter normal(Prettify::RESET);

        std::cout << "Unexpected number of arguments. "
                     "Try again or type "
                  << blue << "sailfishc" << normal
                  << " to see command options !\n ";
        return 0;
    }
}
