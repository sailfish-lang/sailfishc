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
getLastScopeCache(std::vector<std::string>& cache)
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
            value->second.pop();

            // we will remove vars from table if their scope is empty
            if (value->second.empty())
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
SymbolTable::hasVariable(const std::string varName)
{
    return globalScopeTable.find(varName) != globalScopeTable.end();
}

std::string
SymbolTable::getSymbolType(const std::string varName)
{
    if (hasVariable(varName))
    {
        return globalScopeTable.find(varName)->second.top()->getType();
    }

    return "";
}

int
SymbolTable::getSymbolScope(const std::string varName)
{
    if (hasVariable(varName))
    {
        return globalScopeTable.find(varName)->second.top()->getScopeLevel();
    }

    return -1;
}

bool
SymbolTable::addSymbol(const std::string varName, const std::string type)
{
    if (hasVariable(varName))
    {
        // ensure not adding a variable if already exists in this scope
        if (getSymbolScope(varName) == scopeLevel)
            return false;

        SymbolMetaData* smd = new SymbolMetaData(type, scopeLevel);
        globalScopeTable.find(varName)->second.push(smd);
        localCache = addToLocalCache(varName, localCache);
        return true;
    }
    else
    {
        std::stack<SymbolMetaData*> ss;
        SymbolMetaData* smd = new SymbolMetaData(type, scopeLevel);
        ss.push(smd);
        globalScopeTable.insert({varName, ss});
        localCache = addToLocalCache(varName, localCache);
        return true;
    }
}

void
SymbolTable::removeSymbol(const std::string& varName)
{
    if (hasVariable(varName))
    {
        globalScopeTable.erase(varName);
    }
}

// helper function for adding standard lib variables
void
SymbolTable::addStdlib(const std::string varName, const std::string type)
{
    std::stack<SymbolMetaData*> ss;
    SymbolMetaData* smd = new SymbolMetaData(type, scopeLevel);
    ss.push(smd);
    globalScopeTable.insert({varName, ss});
    localCache = addToLocalCache(varName, localCache);
}

void
SymbolTable::clear()
{
    globalScopeTable.clear();
}

void
SymbolTable::addBuiltins()
{
    addSymbol("appendListInt", "F(_[int]_[int]_int_int)[int]");
    addSymbol("appendListStr", "F(_[str]_[str]_int_int)[str]");
    addSymbol("appendListBool", "F(_[bool]_[bool]_int_int)[bool]");
    addSymbol("appendListFlt", "F(_[flt]_[flt]_int_int)[flt]");

    addSymbol("deleteAtIndexInt", "F(_[int]_int_int)[int]");
    addSymbol("deleteAtIndexStr", "F(_[str]_int_int)[str]");
    addSymbol("deleteAtIndexBool", "F(_[bool]_int_int)[bool]");
    addSymbol("deleteAtIndexFlt", "F(_[flt]_int_int)[flt]");

    addSymbol("getAtIndexInt", "F(_[int]_int)int");
    addSymbol("getAtIndexStr", "F(_[str]_int)str");
    addSymbol("getAtIndexBool", "F(_[bool]_int)bool");
    addSymbol("getAtIndexFlt", "F(_[flt]_int)flt");

    addSymbol("setAtIndexInt", "F(_[int]_int_int)[int]");
    addSymbol("setAtIndexStr", "F(_[str]_int_str)[str]");
    addSymbol("setAtIndexBool", "F(_[bool]_int_bool)[bool]");
    addSymbol("setAtIndexFlt", "F(_[flt]_int_flt)[flt]");

    addSymbol("printInt", "F(_int)void");
    addSymbol("printStr", "F(_str)void");
    addSymbol("printBool", "F(_bool)void");
    addSymbol("printFlt", "F(_flt)void");
}
