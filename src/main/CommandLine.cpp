/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "CommandLine.h"

const static std::string VERSION = "sailfishc 0.2.1 (marlin++)";

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

        sailfishc* sfc = new sailfishc(filename, true);
        sfc->parse();

        std::cout << green << "Successfully compiled: " << normal << blue
                  << filename << "\n"
                  << normal;

        sfc->transpile();

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
    case 3:
    {
        if (std::string("--test").compare(argv[1]) == 0)
        {
            try
            {
                std::vector<std::string> expected = {
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Mismatched types. Expected/LeftHand is: i.",
                    "Mismatched types. Expected/LeftHand is: flt.",
                    "Mismatched types. Expected/LeftHand is: int or flt.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Mismatched types. Expected/LeftHand is: int or flt.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Mismatched types. Expected/LeftHand is: int or flt.",
                    "Mismatched types. Expected/LeftHand is: int or flt.",
                    "Mismatched types. Expected/LeftHand is: bool.",
                    "Mismatched types. Expected/LeftHand is: bool.",
                    "Mismatched types. Expected/LeftHand is: int or flt.",
                    "Mismatched types. Expected/LeftHand is: int or flt.",
                    "Mismatched types. Expected/LeftHand is: bool.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Unexpected redeclaration of f, originally defined as type "
                    "flt.",
                    "Missing keys in udt initialization for type: Foo",
                    "Nonexistent member function.",
                    "Mismatched types. Expected/LeftHand is: flt.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Mismatched types. Expected/LeftHand is: flt.",
                    "Nonexistent attribute.",
                    "Mismatched types. Expected/LeftHand is: int.",
                    "Function input parameter type mismatch in function call "
                    "soFun",
                    "Mismatched types. Expected/LeftHand is: flt.",
                    "Mismatched list types. Expected is: [int].",
                    "Mismatched types. Expected/LeftHand is: bool.",
                };

                sailfishc* sfc = new sailfishc(argv[2], false);
                sfc->parse();
                auto errors = sfc->getErrors();

                int i = 0;
                for (auto const& e : errors)
                {
                    // std::cout << e->getMsg() << " " << expected[i] <<
                    // std::endl; assert(e->getMsg() == expected[i++]);
                    std::cout << e->getMsg() << std::endl;
                }
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
