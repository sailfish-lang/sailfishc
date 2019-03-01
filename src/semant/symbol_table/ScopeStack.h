/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <string>
#include <vector>

class ScopeStackNode
{
  private:
    int level;
    std::string type;
    ScopeStackNode* prev;

  public:
    // constructor
    ScopeStackNode(int l, std::string t)
    {
        level = l;
        type = t;
        prev = nullptr;
    }
    // destructor
    ~ScopeStackNode()
    {
        delete prev;
    }
    // helper methods for nicer code
    bool
    hasPrev()
    {
        return prev != nullptr;
    }
    // set methods
    void
    setPrev(ScopeStackNode* p)
    {
        prev = p;
    }
    // get methods
    int
    getLevel()
    {
        return level;
    }
    std::string
    getType()
    {
        return type;
    }
    ScopeStackNode*
    getPrev()
    {
        return prev;
    }
};

class ScopeStack
{
  private:
    ScopeStackNode* cur;
    int size;

  public:
    // constructor
    ScopeStack()
    {
        cur = nullptr;
        size = 0;
    }
    // destructor
    ~ScopeStack()
    {
        delete cur;
    }
    // helper methods for nicer code
    // append a node to the end
    void
    push(int l, std::string t)
    {
        ScopeStackNode* node = new ScopeStackNode(l, t);
        node->setPrev(cur);
        cur = node;

        ++size;
    }
    // remove top node from the stack
    void
    pop()
    {
        if (size != 0)
        {
            ScopeStackNode* prev = cur->getPrev();
            cur = prev; // memory leak?

            --size;
        }
    }
    // remove everything from stack
    void
    clear()
    {
        cur = nullptr; // memory leak?
        size = 0;
    }
    // helper method for nice code
    bool
    isEmpty()
    {
        return cur == nullptr;
    }
    // getMethods
    ScopeStackNode*
    peek()
    {
        // unnecessary check, but in the future if we were to write better code,
        // this would be a useful check
        if (size == 0)
            return nullptr;
        return cur;
    }
};
