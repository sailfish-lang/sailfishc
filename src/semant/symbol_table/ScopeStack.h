/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <vector>

class ScopeStackNode
{
  private:
    int level;
    ScopeStackNode* prev;

  public:
    // constructor
    ScopeStackNode(int l)
    {
        level = l;
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
    push(int l)
    {
        ScopeStackNode* node = new ScopeStackNode(l);
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
    // getMethods
    int
    peek()
    {
        if (size == 0)
            return -1;
        return cur->getLevel();
    }
};
