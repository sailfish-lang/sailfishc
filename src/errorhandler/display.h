/*
 * Robert Durst 2019
 * Sailfish Programming Language
 *
 * Base structure and credit:
 * https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
 */

#include <ostream>
namespace Prettify
{
enum Code
{
    RESET = 0,
    UNDERLINE = 4,
    FG_RED = 31,
    FG_DEFAULT = 39,
};
class Formatter
{
    Code code;

  public:
    Formatter(Code c) : code(c){};
    friend std::ostream&
    operator<<(std::ostream& os, const Formatter& mod)
    {
        return os << "\033[" << mod.code << "m";
    }
};
}