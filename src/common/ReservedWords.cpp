/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "ReservedWords.h"

bool
isKeyword(const std::string v)
{
    if (v == "fun" || v == "Cat" || v == "Cfn" || v == "dec" || v == "exp" ||
        v == "if" || v == "else" || v == "return" || v == "import" ||
        v == "over" || v == "new")
        return true;

    return false;
}

bool
isPrimitive(const std::string type)
{
    if (type == "int" || type == "flt" || type == "str" || type == "bool" ||
        type == "void")
        return true;

    return false;
}