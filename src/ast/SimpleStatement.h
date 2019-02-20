/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "Statement.h"

namespace ast
{
class SimpleStatement : public Statement
{
  public:
    enum SimpleStatementType
    {
        ExportDefinition,
        ExpressionStatement,
    };
    virtual SimpleStatementType getSimpleStatementType() = 0;
    // implement Statement
    /* virtual */ StatementType
    getStatementType()
    {
        return StatementType::IfStatement;
    }
};
}