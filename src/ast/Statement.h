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
        ReturnStatement,
        BlockStatement,
        GeneralDecleration,
        BinaryExpressionStatement,
    };
    virtual StatementType getStatementType() = 0;
};
}