/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SymbolTable.h"

void
SymbolTable::exitScope(std::vector<std::string> localScopeCache)
{
    // loop over all variable names in the supplied local scope cache
    for (auto const& varName : localScopeCache)
    {
        auto value = globalScopeTable.find(varName);
        if (value != globalScopeTable.end())
        {
            // pop the stack cache
            value->second->popScope();
        }
    }

    --scopeLevelCounter;
}

void
SymbolTable::enterScope()
{
    ++scopeLevelCounter;
}

bool
SymbolTable::hasVariable(std::string varName)
{
    return globalScopeTable.find(varName) != globalScopeTable.end();
}

Symbol*
SymbolTable::getSymbol(std::string varName)
{
    if (hasVariable(varName))
    {
        return globalScopeTable.find(varName)->second;
    }

    return nullptr;
}

void
SymbolTable::addSymbol(std::string varName, std::string t,
                       Symbol::SymbolType st)
{
    if (hasVariable(varName))
    {
        getSymbol(varName)->pushScope(scopeLevelCounter);
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::Primitive:
    {
        Symbol* s = new PrimitiveSymbol(t);
        s->pushScope(scopeLevelCounter);
        globalScopeTable.insert({varName, s});
        break;
    }

    case Symbol::SymbolType::List:
    {
        Symbol* s = new ListSymbol(t);
        s->pushScope(scopeLevelCounter);
        globalScopeTable.insert({varName, s});
        break;
    }
    }
}

void
SymbolTable::addSymbol(std::string varName, std::string t, std::string keyType,
                       std::string valueType, Symbol::SymbolType st)
{
    if (hasVariable(varName))
    {
        getSymbol(varName)->pushScope(scopeLevelCounter);
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::Dictionary:
    {
        Symbol* s = new DictionarySymbol(t, keyType, valueType);
        s->pushScope(scopeLevelCounter);
        globalScopeTable.insert({varName, s});
    }
    }
}

void
SymbolTable::addSymbol(std::string varName, std::string t,
                       std::vector<std::string> inputs,
                       std::vector<std::string> outputs, Symbol::SymbolType st)
{
    if (hasVariable(varName))
    {
        getSymbol(varName)->pushScope(scopeLevelCounter);
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::Function:
    {
        Symbol* s = new FunctionSymbol(t, inputs, outputs);
        s->pushScope(scopeLevelCounter);
        globalScopeTable.insert({varName, s});
    }
    }
}

void
SymbolTable::addSymbol(std::string varName, std::string t,
                       SymbolTable* attributes, SymbolTable* methods,
                       Symbol::SymbolType st)
{
    switch (st)
    {
    case Symbol::SymbolType::UDT:
    {
        Symbol* s = new UDTSymbol(t, attributes, methods);
        s->pushScope(scopeLevelCounter);
        globalScopeTable.insert({varName, s});
    }
    }
}