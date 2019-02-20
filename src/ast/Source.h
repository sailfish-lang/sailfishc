/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "SourcePart.h"
#include <vector>

namespace ast
{
class Source
{
  private:
    std::vector<SourcePart*> srcParts;

  public:
    // constructor
    Source(std::vector<SourcePart*> v)
    {
        srcParts = v;
    };
    // get methods
    std::vector<SourcePart*>
    getSourceParts()
    {
        return srcParts;
    }
};
}