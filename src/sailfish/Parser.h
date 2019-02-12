/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lexar.h"
#include "Token.h"
#include <string>
#include <vector>

enum NodeType
{
    Start,
    Source,
    ImportSection,
    Location,
    FilePath,
    SourcePart,
    ExportDefinition,
    Exportable,
    FunctionDefinition,
    FunctionName,
    FunctionInput,
    FunctionOutput,
    FunctionBody,
    InputList,
    Input,
    OutputList,
    Output,
    GeneralDecleration,
    GeneralDefinition,
    ListDefinition,
    DictionaryDefinition,
    NewVariableDefinition,
    RangeVariableDefinition,
    VariableAssignment,
    ShortVariableDefinition,
    VariableDecleration,
    Variable,
    UserDefinedTypeDefinition,
    UserDefinedTypeAttributes,
    UserDefinedTypeMethods,
    InitialExecutionBody,
    Block,
    Statement,
    IfStatement,
    LoopStatement,
    SimpleStatement,
    ReturnStatement,
    Expression,
    IndexAccess,
    MemberAccess,
    FunctionCall,
    FunctionCallArguments,
    ExpressionList,
    NameValueList,
    PrimaryExpression,
    BooleanLiteral,
    DictionaryLiteral,
    DictionaryItem,
    ListLiteral,
    ListItem,
    NumberLiteral,
    Decimal,
    StringLiteral,
    Identifier,
    TypenameExpression,
    Typename,
    Negation,
    UnaryAddition,
    UnarySubtraction,
    Exponentiation,
    Multiplication,
    Division,
    Modulo,
    Addition,
    Subtraction,
    BinaryGreaterThan,
    BinaryLessThan,
    BinaryGreaterThanOrEqual,
    BinaryLessThanOrEqual,
    EquivalenceComparison,
    NonEquivalenceComparison,
    AndComparison,
    OrComparison,
    Assignment
};

class Node
{
  private:
    NodeType type;
    std::string name;
    Node* ref;
    std::vector<Node*> list;

  public:
    // constructor
    Node(NodeType t, std::string n, Node* r, std::vector<Node*> l)
    {
        type = t;
        name = n;
        ref = r;
        list = l;
    };

    // basic get functions
    NodeType
    getType()
    {
        return type;
    }
    std::string
    getName()
    {
        return name;
    }
    Node*
    getRef()
    {
        return ref;
    }
    std::vector<Node*>
    getList()
    {
        return list;
    }

    // for debugging purposes
    std::string
    getTypeString()
    {
        switch (type)
        {
        case Start:
            return "start";
        case Source:
            return "Source";
        case SourcePart:
            return "SourcePart";
        case ExportDefinition:
            return "ExportDefinition";
        case GeneralDecleration:
            return "GeneralDecleration";
        case FunctionDefinition:
            return "FunctionDefinition";
        case InitialExecutionBody:
            return "InitialExecutionBody";
        case UserDefinedTypeAttributes:
            return "UserDefinedTypeAttributes";
        default:
            return "unknown type";
        }
    }
};

class Parser
{
  private:
    Lexar* lexar;
    Token* currentToken; // LL(1)
    // TODO: create some sort of error handler. Curretly just ignore errors.

    // Parsing methods
    // TODO: make pure
    Node* parseStart();
    Node* parseSource();
    Node* parseSourcePart();

    // helper functions
    Token* getNextToken();

  public:
    // constructor
    Parser();
    // actual parse function -- given filename returns the root of the ast
    Node* parse(const std::string);
};
