/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SymbolTable.h"
#include "UDTMetaData.h"
#include <memory>
#include <string>
#include <unordered_map>

class UDTTable
{
  private:
    std::unordered_map<std::string, UDTMetaData*> udtTable;

  public:
    // constructor
    UDTTable()
    {
        udtTable.clear();
    }
    // destructor
    ~UDTTable(){};

    // check if a UDT is in the UDT table
    bool
    hasUDT(const std::string name)
    {
        return udtTable.find(name) != udtTable.end();
    }

    // retreive a UDT's attribute Symbol Table from the UDT table
    std::shared_ptr<SymbolTable>
    getAttributeSymbolTable(const std::string name)
    {
        if (hasUDT(name))
            return udtTable.at(name)->getAttributeSymbolTable();

        return nullptr;
    }

    // retreive a UDT's method Symbol Table from the UDT table
    std::shared_ptr<SymbolTable>
    getMethodSymbolTable(const std::string name)
    {
        if (hasUDT(name))
            return udtTable.at(name)->getMethodSymbolTable();

        return nullptr;
    }

    // returns true if added or false if already exists
    bool
    addUDT(const std::string name, std::shared_ptr<SymbolTable> attributes,
           std::shared_ptr<SymbolTable> methods)
    {
        if (hasUDT(name))
            return false;

        UDTMetaData* udtmd = new UDTMetaData(attributes, methods);
        udtTable.insert({name, udtmd});
        return true;
    }

    // returns true if added or false if already exists
    bool
    addUDT(const std::string name)
    {
        if (hasUDT(name))
            return false;

        UDTMetaData* udtmd =
            new UDTMetaData(std::make_shared<SymbolTable>(SymbolTable()),
                            std::make_shared<SymbolTable>(SymbolTable()));
        udtTable.insert({name, udtmd});
        return true;
    }

    // allow us to update the table after udt key aleady added
    bool
    updateUDT(const std::string name, std::shared_ptr<SymbolTable> attributes,
              std::shared_ptr<SymbolTable> methods)
    {
        if (!hasUDT(name))
            return false;

        udtTable.at(name)->setAttributeSymbolTable(attributes);
        udtTable.at(name)->setMethodSymbolTable(methods);
        return true;
    }

    void
    dump()
    {
        std::cout << "\nDUMPING UDTTable: \n";
        for (auto const& element : udtTable)
        {
            std::cout << "UDTNAME: " << element.first << std::endl;
            element.second->getAttributeSymbolTable()->dump(4);
            element.second->getMethodSymbolTable()->dump(4);
        }
        std::cout << "\n";
    }
};
