/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <string>

// abstract base class for symboks
class Symbol
{
  public:
    virtual std::string getType();
    // -------- methods for determining composition of subtype ------- //
    // for now only UDT's(U)ser (D)efined (T)ype
    virtual bool hasMethods() = 0;
    virtual bool hasAttributes() = 0;
    // for now only dictionaries
    virtual bool hasKeyType() = 0;
    virtual bool hasValueType() = 0;
}