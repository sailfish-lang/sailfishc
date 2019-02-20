/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
namespace ast
{
class Statement
{
  public:
    enum StatementType
    {
        IfStatement,
        LoopStatement,
        ReturnStatement,
        SimpleStatement,
        Break,
        Continue,
    };
    virtual StatementType getStatementType() = 0;
};
}