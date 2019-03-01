/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "ScopeStack.h"
#include "SymbolTable.h"
#include <string>
#include <vector>

// abstract base class for symbols
class Symbol
{
  private:
    std::string type;
    ScopeStack* scopestack;

  public:
    // constructor
    Symbol(std::string t, ScopeStack* s)
    {
        type = t;
        scopestack = s;
    }
    // destructor
    ~Symbol()
    {
        delete scopestack;
    }
    // all related to scope
    virtual void
    popScope()
    {
        scopestack->pop();
    };
    virtual void
    pushScope(int level)
    {
        scopestack->push(level);
    };
    virtual int
    getScopeLevel()
    {
        return scopestack->peek();
    };
    // all values, even UDT's and collections must have a type
    virtual std::string
    getType()
    {
        return type;
    };
    // -------- methods for determining composition of subtype ------- //
    // for now only UDT's(U)ser (D)efined (T)ype
    virtual bool
    hasMethods()
    {
        return false;
    };
    virtual SymbolTable*
    getMethods()
    {
        return nullptr;
    }
    virtual bool
    hasAttributes()
    {
        return false;
    };
    virtual SymbolTable*
    getAttributes()
    {
        return nullptr;
    }
    // for now only dictionaries
    virtual bool
    hasKeyType()
    {
        return false;
    };
    virtual std::string
    getKeyType()
    {
        return "";
    }
    virtual bool
    hasValueType()
    {
        return false;
    };
    virtual std::string
    getValueType()
    {
        return "";
    }
    // for now only functions
    virtual bool
    hasInputTypes()
    {
        return false;
    };
    virtual std::vector<std::string>
    getInputTypes()
    {
        return {};
    }
    virtual bool
    hasOutputTypes()
    {
        return false;
    }
    virtual std::vector<std::string>
    getOutputTypes()
    {
        return {};
    }
};
