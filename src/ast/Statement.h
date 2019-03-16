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
        BinaryExpressionStatement,
        BlockStatement,
        GeneralDecleration,
        IfStatement,
        ReturnStatement,
    };
    virtual StatementType getStatementType() = 0;
};
}