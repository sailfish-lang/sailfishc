/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Error2.h"

class Parser2ErrorHandler
{
  public:
    Parser2ErrorHandler(){};
    void handle(Error2*);
};