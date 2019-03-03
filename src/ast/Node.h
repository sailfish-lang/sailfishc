/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once

class Node
{
  private:
    int lineNum;

  public:
    // constructor
    Node(int l)
    {
        lineNum = l;
    }
    // destructor
    ~Node()
    {
    }
    // get methods
    int
    getLineNum()
    {
        return lineNum;
    }
};