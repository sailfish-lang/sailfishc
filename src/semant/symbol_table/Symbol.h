/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
// forward decleration for circular dependency
// https://stackoverflow.com/questions/625799/resolve-build-errors-due-to-circular-dependency-amongst-classes
class SymbolTable;
#include <string>
#include <vector>

// abstract base class for symbols
class Symbol
{
  public:
    enum SymbolType
    {
        Function,
        Primitive,
        List,
        Dictionary,
        UDT,
    };

    // constructor
    Symbol(SymbolType st, std::string t)
    {
        symbolType = st;
        type = t;
    }
    // destructor
    ~Symbol()
    {
    }
    // all values, even UDT's and collections must have a type
    std::string
    getType()
    {
        return type;
    }
    SymbolType
    getSymbolType()
    {
        return symbolType;
    }
    // -------- methods for determining composition of subtype ------- //
    // for now only UDT's(U)ser (D)efined (T)ype
    virtual SymbolTable*
    getMethods()
    {
        return nullptr;
    }
    virtual SymbolTable*
    getAttributes()
    {
        return nullptr;
    }
    // for now only dictionaries
    virtual std::string
    getKeyType()
    {
        return "";
    }
    virtual std::string
    getValueType()
    {
        return "";
    }
    // for now only functions
    virtual std::vector<std::string>
    getInputTypes()
    {
        return {};
    }
    virtual std::vector<std::string>
    getOutputTypes()
    {
        return {};
    }

  private:
    SymbolType symbolType;
    std::string type;
};
