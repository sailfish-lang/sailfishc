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
            value->second->pop();

            // we will remove vars from table if their scope is empty
            if (value->second->isEmpty())
            {
                globalScopeTable.erase(varName);
            }
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

std::string
SymbolTable::getSymbolType(std::string varName)
{
    if (hasVariable(varName))
    {
        return globalScopeTable.find(varName)->second->peek()->getType();
    }

    return "";
}

int
SymbolTable::getSymbolScope(std::string varName)
{
    if (hasVariable(varName))
    {
        return globalScopeTable.find(varName)->second->peek()->getScopeLevel();
    }

    return -1;
}

bool
SymbolTable::addSymbol(std::string varName, std::string type)
{
    if (hasVariable(varName))
    {
        // ensure not adding a variable if already exists in this scope
        if (getSymbolScope(varName) == scopeLevel)
            return false;

        globalScopeTable.find(varName)->second->push(type, scopeLevel);
        localCache = addToLocalCache(varName, localCache);
        return true;
    }
    else
    {
        ScopeStack* ss = new ScopeStack();
        ss->push(type, scopeLevel);
        globalScopeTable.insert({varName, ss});
        localCache = addToLocalCache(varName, localCache);
        return true;
    }
}
