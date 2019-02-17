/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <string>
#include <vector>

namespace ast
{
// abstract class for all nodes
class Node
{
  public:
    // utilize visitor design pattern
    // virtual void accept(class Visitor& v) = 0;
};

// -------       Abstract Classes       ------- //
class Expression
{
};
class SourcePart : public Node
{
  public:
    // constructor
    SourcePart();
};
class Exportable
{
};
class GeneralDefinition
{
};
class Statement
{
};
class SimpleStatement : Statement
{
};
class VariableDecleration
{
};
class Primary
{
};
class Loop
{
};

// -------       Basic Classes Needed by Alot        ------- //
class Identifier : Node
{
  private:
    std::string identifier;

  public:
    // constructor
    Identifier();
};
class Typename : Node
{
  private:
    Identifier* type;

  public:
    // constructor
    Typename();
};
class Variable : Node
{
  private:
    Typename* type;
    Identifier* name;

  public:
    // constructor
    Variable();
};
class Block : Node
{
  private:
    std::vector<Statement*> statements;

  public:
    // constructor
    Block();
};

// -------       Start Here       ------- //

class Source : public Node
{
  private:
    std::vector<SourcePart*> srcParts;

  public:
    // constructor
    Source();
};

class Start : public Node
{
  private:
    // FUTURE: imports
    Source src;

  public:
    // constructor
    Start();
};

class ExportDefinition : public SourcePart, SimpleStatement
{
  private:
    Exportable* exp;

  public:
    // constructor
    ExportDefinition();
};

class GeneralDecleration : public SourcePart, Exportable
{
  private:
    GeneralDefinition* definition;

  public:
    // constructor
    GeneralDecleration();
};

class ListDefinition : public Node, GeneralDefinition
{
  private:
    Identifier* name;
    // optional
    Expression* expr;

  public:
    // constructor
    ListDefinition();
};
class DictionaryDefinition : public Node, GeneralDefinition
{
  private:
    Identifier* name;
    // optional
    Expression* expr;

  public:
    // constructor
    DictionaryDefinition();
};
class NewVariableDefinition : public Node,
                              GeneralDefinition,
                              VariableDecleration
{
  private:
    Variable* var;
    Expression* expr;

  public:
    // constructor
    NewVariableDefinition();
};

class Input : Node
{
  private:
    Variable* input;

  public:
    // constructor
    Input();
};

class Output : Node
{
  private:
    Typename* type;

  public:
    // constructor
    Output();
};

class FunctionDefinition : public SourcePart, Exportable
{
  private:
    Identifier* name;
    std::vector<Input*> inputList;
    std::vector<Output*> outputList;
    Block* body;

  public:
    // constructor
    FunctionDefinition();
};

class UserDefinedTypeAttributes : Node
{
  private:
    Identifier* name;
    std::vector<Variable*> attributes;

  public:
    // constructor
    UserDefinedTypeAttributes();
};

class UserDefinedTypeMethods : Node
{
  private:
    Identifier* name;
    std::vector<FunctionDefinition*> methods;

  public:
    // constructor
    UserDefinedTypeMethods();
};

class UserDefinedTypeDefinition : public SourcePart
{
  private:
    Identifier* name;
    UserDefinedTypeAttributes attributes;
    // methods are optional
    UserDefinedTypeMethods methods;

  public:
    // constructor
    UserDefinedTypeDefinition();
};

class InitialExecutionBody : public SourcePart
{
    Block* body;

  public:
    // constructor
    InitialExecutionBody();
};

class RangeVariableDefinition : Node, VariableDecleration
{
  private:
    Variable* var;
    Expression* expr;

  public:
    // constructor
    RangeVariableDefinition();
};
class VariableAssignment : Node, VariableDecleration, SimpleStatement
{
  private:
    Identifier* name;
    Expression* expr;

  public:
    // constructor
    VariableAssignment();
};
class ShortVariableDefinition : Node
{
  private:
    Variable* var;

  public:
    // constructor
    ShortVariableDefinition();
};

class IfStatement : Node, Statement
{
  private:
    Expression* ifConditional;
    Statement* ifStatement;
    Statement* elseStatement;

  public:
    // constructor
    IfStatement();
};

class LoopStatement : Node, Statement
{
  private:
    Loop* loop;

  public:
    // constructor
    LoopStatement();
};

class LoopRange
{
  private:
    RangeVariableDefinition* rngVar;
    Block* body;

  public:
    // constructor
    LoopRange();
};

class LoopExpression
{
  private:
    Expression* loopCondition;
    Block* body;

  public:
    // constructor
    LoopExpression();
};

class ExpressionStatement : SimpleStatement
{
  private:
    Expression* Expr;

  public:
    // constructor
    ExpressionStatement();
};

class ReturnStatement : Node, Statement
{
  private:
    // optional
    Expression* expr;

  public:
    // constructor
    ReturnStatement();
};

class IndexAccess : Node, Expression
{
  private:
    Expression* expr;
    Expression* index;

  public:
    // constructor
    IndexAccess();
};

class MemberAccess : Node, Expression
{
  private:
    Expression* expr;
    Identifier* member;

  public:
    // constructor
    MemberAccess();
};

class FunctionCall : Node, Expression
{
  private:
    Identifier* name;
    std::vector<Identifier*> args;

  public:
    // constructor
    FunctionCall();
};

class PrimaryExpression : Node, Expression
{
  private:
    Primary* expr;

  public:
    // constructor
    PrimaryExpression();
};

class BooleanLiteral : Node, Primary
{
  private:
    std::string value;

  public:
    // constructor
    BooleanLiteral();
};

class DictionaryItem : Node
{
  private:
    Identifier* key;
    Identifier* value;

  public:
    // constructor
    DictionaryItem();
};

class ListItem : Node
{
  private:
    Identifier* name;

  public:
    // constructor
    ListItem();
};
class DictionaryLiteral : Node, Primary
{
  private:
    Identifier* name;
    std::vector<DictionaryItem*> dictionaryItems;

  public:
    // constructor
    DictionaryLiteral();
};

class ListLiteral : Node, Primary
{
  private:
    Identifier* name;
    std::vector<ListItem*> listItems;

  public:
    // constructor
    ListLiteral();
};

class NumberLiteral : Node, Primary
{
  private:
    std::string number;

  public:
    // constructor
    NumberLiteral();
};

class StringLiteral : Node, Primary
{
  private:
    std::string string;

  public:
    // constructor
    StringLiteral();
};

class TypenameExpression : Node, Primary
{
  private:
    ast::Typename* type;

  public:
    // constructor
    TypenameExpression();
};

class Negation : Node
{
  public:
    // constructor
    Negation();
};
class Exponentiation : Node
{
  public:
    // constructor
    Exponentiation();
};
class Multiplication : Node
{
  public:
    // constructor
    Multiplication();
};
class Division : Node
{
  public:
    // constructor
    Division();
};
class Modulo : Node
{
  public:
    // constructor
    Modulo();
};
class Addition : Node
{
  public:
    // constructor
    Addition();
};
class Subtraction : Node
{
  public:
    // constructor
    Subtraction();
};
class BinaryGreaterThan : Node
{
  public:
    // constructor
    BinaryGreaterThan();
};
class BinaryLessThan : Node
{
  public:
    // constructor
    BinaryLessThan();
};
class BinaryGreaterThanOrEqual : Node
{
  public:
    // constructor
    BinaryGreaterThanOrEqual();
};
class BinaryLessThanOrEqual : Node
{
  public:
    // constructor
    BinaryLessThanOrEqual();
};
class EquivalenceComparison : Node
{
  public:
    // constructor
    EquivalenceComparison();
};
class NonEquivalenceComparison : Node
{
  public:
    // constructor
    NonEquivalenceComparison();
};
class AndComparison : Node
{
  public:
    // constructor
    AndComparison();
};
class OrComparison : Node
{
  public:
    // constructor
    OrComparison();
};
class Assignment : Node
{
  private:
    Identifier* name;
    Expression* expr;

  public:
    // constructor
    Assignment();
};
}
