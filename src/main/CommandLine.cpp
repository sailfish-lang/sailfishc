/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "CommandLine.h"

const static std::string VERSION = "sailfishc 0.3.0 (Istiophoriformes)";

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

        sailfishc* sfc = new sailfishc(filename, true);
        sfc->parse();

        std::cout << green << "Successfully compiled: " << normal << blue
                  << filename << "\n"
                  << normal;

        std::cout << green << "Successfully wrote compiled code to: " << normal
                  << blue << " out.c\n"
                  << normal;
        return true;
    }
    catch (const std::string msg)
    {
        std::cerr << msg;
        return false;
    }
    catch (char const* msg)
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
    {
        if (std::string("--test").compare(argv[1]) == 0)
        {
            Prettify::Formatter red(Prettify::FG_RED);
            Prettify::Formatter green(Prettify::FG_GREEN);
            Prettify::Formatter normal(Prettify::RESET);

            bool result = SEMANTIC_ANALYSIS_TEST(argv[2]);
            if (result)
                std::cout << green << "SUCCESSFUL TEST!" << '\n' << normal;
            else
                std::cout << red << "TEST FAILED!" << '\n' << normal;
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
        break;
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
