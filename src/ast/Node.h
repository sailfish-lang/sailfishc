/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
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

class Start : public Node
{
  private:
    // FUTURE: imports
    Source src;

  public:
    // constructor
    Start();
};

class Source : public Node
{
  private:
    std::vector<SourcePart*> srcParts;

  public:
    // constructor
    Source();
};

// abstract class for source parts
class SourcePart : public Node
{
  public:
    // constructor
    SourcePart();
};

class ExportDefinition : public SourcePart, SimpleStatement
{
  private:
    Exportable* exp;

  public:
    // constructor
    ExportDefinition();
};

// abstract class used more as a rust-like trait here
class Exportable
{
};

class GeneralDecleration : public SourcePart, Exportable
{
  private:
    GeneralDefinition* definition;

  public:
    // constructor
    GeneralDecleration();
};

// abstract class used more as a rust-like trait here
class GeneralDefinition
{
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

class InitialExecutionBody : public SourcePart
{
    Block* body;

  public:
    // constructor
    InitialExecutionBody();
};

// abstract class used more like a rust tract here
class VariableDecleration
{
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

// abstract class used more like a rust trait here
class Statement
{
};

class IfStatement : Node, Statement
{
};

class LoopStatement : Node, Statement
{
};

// abstract class used more like a rust trait here
class SimpleStatement : Statement
{
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

// abstract class used more like a rust trait here
class Expression
{
};

class IndexAccess : Node, Expression
{
};

class MemberAccess : Node, Expression
{
};

class FunctionCall : Node, Expression
{
};

class PrimaryExpression : Node, Expression
{
};

class FunctionCallArguments : Node
{
};
class ExpressionList : Node
{
};
class NameValueList : Node
{
};

class BooleanLiteral : Node
{
};
class DictionaryLiteral : Node
{
};
class DictionaryItem : Node
{
};
class ListLiteral : Node
{
};
class ListItem : Node
{
};
class NumberLiteral : Node
{
};
class Decimal : Node
{
};
class StringLiteral : Node
{
};
class Identifier : Node
{
};
class TypenameExpression : Node
{
};
class Typename : Node
{
};
class Negation : Node
{
};
class Exponentiation : Node
{
};
class Multiplication : Node
{
};
class Division : Node
{
};
class Modulo : Node
{
};
class Addition : Node
{
};
class Subtraction : Node
{
};
class BinaryGreaterThan : Node
{
};
class BinaryLessThan : Node
{
};
class BinaryGreaterThanOrEqual : Node
{
};
class BinaryLessThanOrEqual : Node
{
};
class EquivalenceComparison : Node
{
};
class NonEquivalenceComparison : Node
{
};
class AndComparison : Node
{
};
class OrComparison : Node
{
};
class Assignment : Node
{
};
}
