/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "SymbolTable.h"

// helper method to add a variable name to the cache
std::vector<std::string>
addToLocalCache(std::string varName, std::vector<std::string> localCache)
{
    localCache.push_back(varName);
    return localCache;
}

// helper method to get only this scope's local cache
std::vector<std::string>
getLastScopeCache(std::vector<std::string> cache)
{
    std::vector<std::string> localCache;
    while (cache.size() > 0)
    {
        std::string varName = cache[cache.size() - 1];
        cache.pop_back();

        // meet a scope break point
        if (varName == "|")
        {
            return localCache;
        }

        // add to local cache to return
        localCache.push_back(varName);
    }

    return localCache;
}

void
SymbolTable::exitScope()
{
    // loop over all variable names in the supplied local scope cache
    for (auto const& varName : getLastScopeCache(localCache))
    {
        auto value = globalScopeTable.find(varName);
        if (value != globalScopeTable.end())
        {
            globalScopeTable.erase(varName);
        }
    }

    --scopeLevel;
}

void
SymbolTable::enterScope()
{
    // hard code add a scope seperator
    localCache.push_back("|");
    ++scopeLevel;
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
SymbolTable::addSymbol(std::string varName, std::string type,
                       Symbol::SymbolType st)
{
    if (hasVariable(varName))
    {
        // ERROR
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::Primitive:
    {
        Symbol* s = new PrimitiveSymbol(type);
        globalScopeTable.insert({varName, s});
        break;
    }

    case Symbol::SymbolType::List:
    {
        Symbol* s = new ListSymbol(type);
        globalScopeTable.insert({varName, s});
        break;
    }
    }

    localCache = addToLocalCache(varName, localCache);
}

void
SymbolTable::addSymbol(std::string varName, std::string type,
                       std::string keyType, std::string valueType,
                       Symbol::SymbolType st)
{
    if (hasVariable(varName))
    {
        // ERROR
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::Dictionary:
    {
        Symbol* s = new DictionarySymbol(type, keyType, valueType);
        globalScopeTable.insert({varName, s});
        break;
    }
    }

    localCache = addToLocalCache(varName, localCache);
}

void
SymbolTable::addSymbol(std::string varName, std::string type,
                       std::vector<std::string> inputs,
                       std::vector<std::string> outputs, Symbol::SymbolType st)
{
    if (hasVariable(varName))
    {
        // ERROR
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::Function:
    {
        Symbol* s = new FunctionSymbol(type, inputs, outputs);
        globalScopeTable.insert({varName, s});
        break;
    }
    }

    localCache = addToLocalCache(varName, localCache);
}

void
SymbolTable::addSymbol(std::string varName, std::string type,
                       SymbolTable* attributes, SymbolTable* methods,
                       Symbol::SymbolType st)
{

    if (hasVariable(varName))
    {
        // ERROR
        return;
    }

    switch (st)
    {
    case Symbol::SymbolType::UDT:
    {
        Symbol* s = new UDTSymbol(type, attributes, methods);
        globalScopeTable.insert({varName, s});
        break;
    }
    }

    localCache = addToLocalCache(varName, localCache);
}