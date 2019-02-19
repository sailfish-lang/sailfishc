/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <iostream>
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
  public:
    virtual std::string getType(){};
};
class SourcePart : Node
{
  public:
    // must defined a default value for the virtual function
    // https://stackoverflow.com/questions/307352/g-undefined-reference-to-typeinfo
    virtual std::string getType(){};
};
class Exportable
{
  public:
    virtual std::string getType(){};
};
class GeneralDefinition
{
  public:
    virtual std::string getType(){};
};
class Statement
{
  public:
    virtual std::string getType(){};
};
class SimpleStatement : public Statement
{
  public:
    // implement Statement
    /* virtual */ std::string
    getType()
    {
        return "SimpleStatement";
    }
};
class VariableDecleration
{
};
class Primary
{
  public:
    virtual std::string getType(){};
};
class Loop
{
  public:
    virtual std::string getType(){};
};

// -------       Basic Classes Needed by Alot        ------- //
class Identifier : Node, public Primary
{
  private:
    std::string identifier;

  public:
    // constructor
    Identifier(std::string);
    // implement Primary
    /* virtual */ std::string
    getType()
    {
        return "Identifier";
    }
    // get method
    std::string
    getValue()
    {
        return identifier;
    }
};
class Typename : Node
{
  private:
    std::string type;

  public:
    // constructor
    Typename(std::string);
    // get method
    std::string
    getType()
    {
        return type;
    }
};
class Variable : Node
{
  private:
    Typename* type;
    Identifier* name;

  public:
    // constructor
    Variable(Typename*, Identifier*);
    // get methods
    Typename*
    getType()
    {
        return type;
    }
    Identifier*
    getName()
    {
        return name;
    }
};
class Block : Node
{
  private:
    std::vector<Statement*> statements;

  public:
    // constructor
    Block(std::vector<ast::Statement*>);
    // get methods
    std::vector<ast::Statement*>
    getStatements()
    {
        return statements;
    }
};

class IntegerLiteral : Node, public Primary
{
  private:
    std::string num;

  public:
    // constructor
    IntegerLiteral(std::string);
    // implement Primary
    /* virtual */ std::string
    getType()
    {
        return "IntegerLiteral";
    }
    // get methods
    std::string
    getNum()
    {
        return num;
    }
};

// -------       Start Here       ------- //
class Source : public Node
{
  private:
    std::vector<SourcePart*> srcParts;

  public:
    // constructor
    Source(std::vector<SourcePart*>);
    // get methods
    std::vector<SourcePart*>
    getSourceParts()
    {
        return srcParts;
    }
};

class Start : public Node
{
  private:
    // FUTURE: imports
    Source* src;

  public:
    // constructor
    Start(Source*);
    // get methods
    Source*
    getSource()
    {
        return src;
    }
};

class ExportDefinition : public SourcePart, public SimpleStatement
{
  private:
    Exportable* exprt;

  public:
    // constructor
    ExportDefinition(Exportable*);
    // implementation for SourcePart/SimpleStatement
    /* virutal */ std::string
    getType()
    {
        return "ExportDefinition";
    }
    // get methods
    Exportable*
    getExport()
    {
        return exprt;
    }
};

class GeneralDecleration : public SourcePart, public Exportable
{
  private:
    GeneralDefinition* definition;

  public:
    // constructor
    GeneralDecleration(ast::GeneralDefinition*);
    // implementation for SourcePart
    /* virutal */ std::string
    getType()
    {
        return "GeneralDecleration";
    }
    // get methods
    ast::GeneralDefinition*
    getDefinition()
    {
        return definition;
    }
};

class ListDefinition : public Node, public GeneralDefinition
{
  private:
    Identifier* name;
    // optional
    Expression* expr;

  public:
    // constructor
    ListDefinition();
    // implement GeneralDefinition
    std::string
    getType()
    {
        return "ListDefinition";
    }
};
class DictionaryDefinition : public Node, public GeneralDefinition
{
  private:
    Identifier* name;
    // optional
    Expression* expr;

  public:
    // constructor
    DictionaryDefinition();
    // implement GeneralDefinition
    std::string
    getType()
    {
        return "DictionaryDefinition";
    }
};
class NewVariableDefinition : public GeneralDefinition,
                              public Node,
                              VariableDecleration
{
  private:
    Variable* var;
    Expression* expr;

  public:
    // constructor
    NewVariableDefinition(Variable*, Expression*);
    // implement GeneralDefinition
    std::string
    getType()
    {
        return "NewVariableDefinition";
    }
    // get methods
    Variable*
    getVariable()
    {
        return var;
    }
    Expression*
    getExpression()
    {
        return expr;
    }
};

class Input : Node
{
  private:
    Variable* input;

  public:
    // constructor
    Input(Variable*);
    // getMethods
    Variable*
    getInput()
    {
        return input;
    }
};

class Output : Node
{
  private:
    Typename* output;

  public:
    // constructor
    Output(Typename*);
    // get methods
    Typename*
    getOutput()
    {
        return output;
    }
};

class FunctionDefinition : public SourcePart, public Exportable
{
  private:
    Identifier* name;
    std::vector<Input*> inputList;
    std::vector<Output*> outputList;
    Block* body;

  public:
    // constructor
    FunctionDefinition(Identifier*, std::vector<Input*>, std::vector<Output*>,
                       Block*);
    // implementation for SourcePart
    /* virutal */ std::string
    getType()
    {
        return "FunctionDefinition";
    }
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<Input*>
    getInputList()
    {
        return inputList;
    }
    std::vector<Output*>
    getOutputList()
    {
        return outputList;
    }
    Block*
    getBody()
    {
        return body;
    }
};

class UserDefinedTypeAttributes : Node
{
  private:
    Identifier* name;
    std::vector<Variable*> attributes;

  public:
    // constructor
    UserDefinedTypeAttributes(Identifier*, std::vector<Variable*>);
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<Variable*>
    getAttributes()
    {
        return attributes;
    }
};

class UserDefinedTypeMethods : Node
{
  private:
    Identifier* name;
    std::vector<FunctionDefinition*> methods;

  public:
    // constructor
    UserDefinedTypeMethods(Identifier*, std::vector<FunctionDefinition*>);
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
    std::vector<FunctionDefinition*>
    getMethods()
    {
        return methods;
    }
};

class UserDefinedTypeDefinition : public SourcePart
{
  private:
    UserDefinedTypeAttributes* attributes;
    // methods are optional
    UserDefinedTypeMethods* methods;

  public:
    // constructor
    UserDefinedTypeDefinition(UserDefinedTypeAttributes*,
                              UserDefinedTypeMethods*);
    // implementation for SourcePart
    /* virutal */ std::string
    getType()
    {
        return "UserDefinedTypeDefinition";
    }
    // get methods
    UserDefinedTypeAttributes*
    getAttributes()
    {
        return attributes;
    }
    UserDefinedTypeMethods*
    getMethods()
    {
        return methods;
    }
};

class InitialExecutionBody : public SourcePart
{
    Block* body;

  public:
    // constructor
    InitialExecutionBody(Block*);
    // implementation for SourcePart
    /* virutal */ std::string
    getType()
    {
        return "InitialExecutionBody";
    }
    // get methods
    Block*
    getBody()
    {
        return body;
    }
};

class RangeVariableDefinition : Node, VariableDecleration
{
  private:
    Variable* var;
    Expression* expr;

  public:
    // constructor
    RangeVariableDefinition(Variable*, Expression*);
    // get methods
    Variable*
    getVar()
    {
        return var;
    }
    Expression*
    getExpr()
    {
        return expr;
    }
};
class VariableAssignment : Node, VariableDecleration
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

class IfStatement : Node, public Statement
{
  private:
    Expression* ifConditional;
    Block* ifStatements;
    Block* elseStatements;

  public:
    // constructor
    IfStatement(Expression*, Block*, Block*);
    // implement Statement
    /* virtual */ std::string
    getType()
    {
        return "IfStatement";
    }
    // get methods
    Expression*
    getIfConditional()
    {
        return ifConditional;
    }
    Block*
    getIfStatements()
    {
        return ifStatements;
    }
    Block*
    getElseStatements()
    {
        return elseStatements;
    }
};

class LoopStatement : Node, public Statement
{
  private:
    Loop* loop;

  public:
    // constructor
    LoopStatement();
    // implement Statement
    /* virtual */ std::string
    getType()
    {
        return "LoopStatement";
    }
};

class LoopRange : public Loop
{
  private:
    RangeVariableDefinition* rngVar;
    Block* body;

  public:
    // constructor
    LoopRange(RangeVariableDefinition*, Block*);
    // implement Loop
    /* virtual */ std::string
    getType()
    {
        return "LoopExpression";
    }
    // get methods
    RangeVariableDefinition*
    getRangeVariable()
    {
        return rngVar;
    }
    Block*
    getBody()
    {
        return body;
    }
};

class LoopExpression : public Loop
{
  private:
    Expression* loopCondition;
    Block* body;

  public:
    // constructor
    LoopExpression(Expression*, Block*);
    // implement Loop
    /* virtual */ std::string
    getType()
    {
        return "LoopExpression";
    }
    // get methods
    Expression*
    getCondition()
    {
        return loopCondition;
    }
    Block*
    getBody()
    {
        return body;
    }
};

class ExpressionStatement : public SimpleStatement
{
  private:
    Expression* Expr;

  public:
    // constructor
    ExpressionStatement();
    // implement SimpleStatement
    /* virtual */ std::string
    getType()
    {
        return "SimpleStatement";
    }
};

class ReturnStatement : Node, public Statement
{
  private:
    // optional
    Expression* expr;

  public:
    // constructor
    ReturnStatement(Expression*);
    // implement Statement
    /* virtual */ std::string
    getType()
    {
        return "ReturnStatement";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};

class IndexAccess : Node, public Expression
{
  private:
    IntegerLiteral* index;

  public:
    // constructor
    IndexAccess(IntegerLiteral*);
    // implement Expression
    std::string
    getType()
    {
        return "IndexAccess";
    }
    // get method
    IntegerLiteral*
    getIndex()
    {
        return index;
    }
};

class MemberAccess : Node, public Expression
{
  private:
    Identifier* member;

  public:
    // constructor
    MemberAccess(Identifier*);
    // implement Expression
    std::string
    getType()
    {
        return "MemberAccess";
    }
    // get method
    Identifier*
    getMember()
    {
        return member;
    }
};

class FunctionCall : Node, public Expression
{
  private:
    std::vector<Expression*> args;

  public:
    // constructor
    FunctionCall();
    // implement Expression
    std::string
    getType()
    {
        return "FunctionCall";
    }
};

class PrimaryExpression : Node, public Expression
{
  private:
    Primary* primary;

  public:
    // constructor
    PrimaryExpression(Primary*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "PrimaryExpression";
    }
    // get methods
    Primary*
    getPrimary()
    {
        return primary;
    }
};

class BooleanLiteral : Node, public Primary
{
  private:
    std::string value;

  public:
    // constructor
    BooleanLiteral(std::string);
    // implement Primary
    /* virtual */ std::string
    getType()
    {
        return "BooleanLiteral";
    }
    // get methods
    std::string
    getValue()
    {
        return value;
    }
};

class DictionaryItem : Node
{
  private:
    Identifier* key;
    Identifier* value;

  public:
    // constructor
    DictionaryItem(Identifier*, Identifier*);
    // get methods
    Identifier*
    getKey()
    {
        return key;
    }
    Identifier*
    getValue()
    {
        return value;
    }
};

class ListItem : Node
{
  private:
    Identifier* name;

  public:
    // constructor
    ListItem(Identifier*);
    // get methods
    Identifier*
    getName()
    {
        return name;
    }
};
class DictionaryLiteral : Node, public Primary
{
  private:
    std::vector<DictionaryItem*> dictionaryItems;

  public:
    // constructor
    DictionaryLiteral(std::vector<DictionaryItem*>);
    // implement Primary
    /* virtual */ std::string
    getType()
    {
        return "DictionaryLiteral";
    }
    // get methods
    std::vector<DictionaryItem*>
    getItems()
    {
        return dictionaryItems;
    }
};

class ListLiteral : Node, public Primary
{
  private:
    std::vector<ListItem*> listItems;

  public:
    // constructor
    ListLiteral(std::vector<ListItem*>);
    // implement Primary
    /* virtual */ std::string
    getType()
    {
        return "DictionaryLiteral";
    }
    // get methods
    std::vector<ListItem*>
    getItems()
    {
        return listItems;
    }
};

class FloatLiteral : Node, public Primary
{
  private:
    std::string num;

  public:
    // constructor
    FloatLiteral(std::string);
    // implement Primary
    /* virtual */ std::string
    getType()
    {
        return "FloatLiteral";
    }
    // get methods
    std::string
    getNum()
    {
        return num;
    }
};

class StringLiteral : Node, public Primary
{
  private:
    std::string string;

  public:
    // constructor
    StringLiteral(std::string);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "StringLiteral";
    }
    // get methods
    std::string
    getString()
    {
        return string;
    }
};

class Negation : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Negation(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Negation";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Exponentiation : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Exponentiation(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Exponentiation";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Multiplication : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Multiplication(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Multiplication";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Division : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Division(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Division";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Modulo : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Modulo(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Modulo";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Addition : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Addition(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Addition";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Subtraction : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Subtraction(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Subtraction";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class BinaryGreaterThan : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryGreaterThan(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "BinaryGreaterThan";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class BinaryLessThan : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryLessThan(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "BinaryLessThan";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class BinaryGreaterThanOrEqual : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryGreaterThanOrEqual(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "BinaryGreaterThan";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class BinaryLessThanOrEqual : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    BinaryLessThanOrEqual(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "BinaryLessThan";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class EquivalenceComparison : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    EquivalenceComparison(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "EquivalenceComparison";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class NonEquivalenceComparison : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    NonEquivalenceComparison(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "NonEquivalenceComparison";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class AndComparison : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    AndComparison(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "AndComparison";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class OrComparison : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    OrComparison(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "OrComparison";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
class Break : Node, public Statement
{
  public:
    // constructor
    Break();
    // implement Statement
    /* virtual */ std::string
    getType()
    {
        return "Break";
    }
};
class Continue : Node, public Statement
{
  public:
    // constructor
    Continue();
    // implement Statement
    /* virtual */ std::string
    getType()
    {
        return "Continue";
    }
};
class Assignment : Node, public Expression
{
  private:
    Expression* expr;

  public:
    // constructor
    Assignment(Expression*);
    // implement Expression
    /* virtual */ std::string
    getType()
    {
        return "Assignment";
    }
    // get methods
    Expression*
    getExpr()
    {
        return expr;
    }
};
}
