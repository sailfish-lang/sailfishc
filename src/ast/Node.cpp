/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Node.h"

ast::Start::Start(Source* s)
{
    src = s;
}

ast::Source::Source(std::vector<SourcePart*> sps)
{
    srcParts = sps;
}

ast::ExportDefinition::ExportDefinition(Exportable* e)
{
    exprt = e;
}

ast::GeneralDecleration::GeneralDecleration(ast::GeneralDefinition* gd)
{
    definition = gd;
}

ast::ListDefinition::ListDefinition()
{
}

ast::DictionaryDefinition::DictionaryDefinition()
{
}

ast::NewVariableDefinition::NewVariableDefinition(ast::Variable* v,
                                                  ast::Expression* e)
{
    var = v;
    expr = e;
}

ast::FunctionDefinition::FunctionDefinition(Identifier* id,
                                            std::vector<Input*> i,
                                            std::vector<Output*> o, Block* b)
{
    name = id;
    inputList = i;
    outputList = o;
    body = b;
}

ast::Input::Input(Variable* v)
{
    input = v;
}

ast::Output::Output(Typename* t)
{
    output = t;
}

ast::UserDefinedTypeDefinition::UserDefinedTypeDefinition(
    ast::UserDefinedTypeAttributes* a, ast::UserDefinedTypeMethods* m)
{
    attributes = a;
    methods = m;
}

ast::UserDefinedTypeAttributes::UserDefinedTypeAttributes(
    Identifier* id, std::vector<Variable*> a)
{
    name = id;
    attributes = a;
}

ast::UserDefinedTypeMethods::UserDefinedTypeMethods(
    Identifier* id, std::vector<FunctionDefinition*> m)
{
    name = id;
    methods = m;
}

ast::InitialExecutionBody::InitialExecutionBody()
{
}

ast::RangeVariableDefinition::RangeVariableDefinition()
{
}

ast::VariableAssignment::VariableAssignment()
{
}

ast::ShortVariableDefinition::ShortVariableDefinition()
{
}

ast::Variable::Variable(Typename* t, Identifier* id)
{
    type = t;
    name = id;
}

ast::Block::Block(std::vector<ast::Statement*> s)
{
    statements = s;
}

ast::IfStatement::IfStatement()
{
}

ast::LoopStatement::LoopStatement()
{
}

ast::LoopRange::LoopRange()
{
}

ast::LoopExpression::LoopExpression()
{
}

ast::ExpressionStatement::ExpressionStatement()
{
}

ast::ReturnStatement::ReturnStatement(Expression* e)
{
    expr = e;
}

ast::IndexAccess::IndexAccess()
{
}

ast::MemberAccess::MemberAccess()
{
}

ast::FunctionCall::FunctionCall()
{
}

ast::PrimaryExpression::PrimaryExpression()
{
}

ast::BooleanLiteral::BooleanLiteral()
{
}

ast::DictionaryLiteral::DictionaryLiteral()
{
}

ast::DictionaryItem::DictionaryItem()
{
}

ast::ListLiteral::ListLiteral()
{
}

ast::ListItem::ListItem()
{
}

ast::NumberLiteral::NumberLiteral()
{
}

ast::StringLiteral::StringLiteral()
{
}

ast::Identifier::Identifier(std::string id)
{
    identifier = id;
}

ast::TypenameExpression::TypenameExpression()
{
}

ast::Typename::Typename(std::string t)
{
    type = t;
}

ast::Negation::Negation(Expression* e)
{
    expr = e;
}

ast::Exponentiation::Exponentiation(Expression* e)
{
    expr = e;
}

ast::Multiplication::Multiplication(Expression* e)
{
    expr = e;
}

ast::Modulo::Modulo(Expression* e)
{
    expr = e;
}

ast::Addition::Addition(Expression* e)
{
    expr = e;
}

ast::Subtraction::Subtraction(Expression* e)
{
    expr = e;
}

ast::Division::Division(Expression* e)
{
    expr = e;
}

ast::BinaryGreaterThan::BinaryGreaterThan(Expression* e)
{
    expr = e;
}

ast::BinaryLessThan::BinaryLessThan(Expression* e)
{
    expr = e;
}

ast::BinaryGreaterThanOrEqual::BinaryGreaterThanOrEqual(Expression* e)
{
    expr = e;
}

ast::BinaryLessThanOrEqual::BinaryLessThanOrEqual(Expression* e)
{
    expr = e;
}

ast::EquivalenceComparison::EquivalenceComparison(Expression* e)
{
    expr = e;
}

ast::NonEquivalenceComparison::NonEquivalenceComparison(Expression* e)
{
    expr = e;
}

ast::AndComparison::AndComparison(Expression* e)
{
    expr = e;
}

ast::OrComparison::OrComparison(Expression* e)
{
    expr = e;
}

ast::Break::Break()
{
}

ast::Continue::Continue()
{
}

ast::Assignment::Assignment(Expression* e)
{
    expr = e;
}