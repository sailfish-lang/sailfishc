/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Node.h"
#include "SourcePart.h"
#include <vector>

namespace ast
{
class Source : public Node
{
  private:
    std::vector<SourcePart*> srcParts;

  public:
    // constructor
    Source(std::vector<SourcePart*> v, int lineNum) : Node(lineNum)
    {
        srcParts = v;
    };
    // destructor
    ~Source(){};
    // get methods
    std::vector<SourcePart*>
    getSourceParts()
    {
        return srcParts;
    }
};
}