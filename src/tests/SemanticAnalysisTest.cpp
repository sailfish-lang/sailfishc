/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SemanticAnalysisTest.h"

bool
SEMANTIC_ANALYSIS_TEST(const std::string& filename)
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

        sailfishc* sfc = new sailfishc(filename, false);
        sfc->parse();
        auto errors = sfc->getErrors();

        int i = 0;
        for (auto const& e : errors)
            assert(e->getMsg() == expected[i++]);

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