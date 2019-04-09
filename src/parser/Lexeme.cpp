#include "Lexeme.h"

std::shared_ptr<NodeLexeme>
makeNode(OP o, Lexeme l, Lexeme r)
{
    return std::make_shared<NodeLexeme>(o, l, r);
}

std::shared_ptr<NodeLexeme>
makeNode(OP o, Lexeme l)
{
    return std::make_shared<NodeLexeme>(o, l, makeNullNode());
}

std::string
disp(OP o)
{
    switch (o)
    {
    case OP::SOURCE:
        return "Source";
    case OP::SOURCE_PART:
        return "Source Part";
    case OP::IMPORT:
        return "Import";
    case OP::UDT:
        return "Udt";
    case OP::UDA:
        return "Uda";
    case OP::START:
        return "Start";
    case OP::SCRIPT:
        return "Script";
    case OP::VARIABLE:
        return "Variable";
    case OP::FUNCTION:
        return "Function";
    case OP::FUNCTION_INFO:
        return "Function Info";
    case OP::FUNCTION_IN_OUT:
        return "Function In Out";
    case OP::TREE:
        return "Tree";
    case OP::BRANCH:
        return "Branch";
    case OP::E0:
        return "E0";
    case OP::EXPONENT:
        return "Exponent";
    case OP::MULTIPLICATION:
        return "Multiplication";
    case OP::DIVISION:
        return "Division";
    case OP::MODULO:
        return "Modulo";
    case OP::ADDITION:
        return "Addition";
    case OP::SUBTRACTION:
        return "Subtraction";
    case OP::LESS_THAN:
        return "Less than";
    case OP::LESS_THAN_OR_EQUALS:
        return "Less than or equals";
    case OP::GREATER_THAN:
        return "Greater than";
    case OP::GREATER_THAN_OR_EQUALS:
        return "Greater than or equals";
    case OP::EQUIVALENCE:
        return "Equivalence";
    case OP::NONEQUIVALENCE:
        return "Nonequivalence";
    case OP::AND:
        return "And";
    case OP::OR:
        return "Or";
    case OP::ASSIGNMENT:
        return "Assignment";
    case OP::NEGATION:
        return "Negation";
    case OP::END:
        return "End";
    case OP::PRIMARY:
        return "Primary";
    case OP::DECLARATION:
        return "Declaration";
    case OP::RETURN:
        return "Return";
    case OP::ATTRIBUTE:
        return "Attribute";
    case OP::METHOD:
        return "Method";
    case OP::FUNCTION_INPUT:
        return "Function Input";
    case OP::STATEMENT:
        return "Statement";
    case OP::BLOCK:
        return "Block";
    case OP::NULL_VAL:
        return "null";
    case OP::MEMBER:
        return "Member";
    case OP::ATTRIBUTE_ACCESS:
        return "Attribute Access";
    case OP::METHOD_ACCESS:
        return "Method Access";
    case OP::INPUT:
        return "Input";
    case OP::FUNCTION_CALL:
        return "Function Call";
    case OP::UDTDEC:
        return "UDT dec";
    case OP::UDTDECITEM:
        return "UDT dec item";
    case OP::UNARYADD:
        return "Unary add";
    case OP::UNARYMINUS:
        return "Unary minus";
    case OP::ADDTO:
        return "Add to";
    case OP::SUBFROM:
        return "Sub from";
    case OP::DIVFROM:
        return "Div from";
    case OP::MULTTO:
        return "Mult to";
    default:
        return "Unknown";
    }
}

std::string
disp(LIT o)
{
    switch (o)
    {
    case LIT::IDENTIFIER:
        return "Identifier";
    case LIT::STRING:
        return "String";
    case LIT::BOOLEAN:
        return "Boolean";
    case LIT::INTEGER:
        return "Integer";
    case LIT::FLOAT:
        return "Float";
    case LIT::LIST:
        return "List";
    case LIT::LISTTYPE:
        return "List type";
    default:
        return "Unknown";
    }
}

std::shared_ptr<LeafLexeme>
makeLeaf(LIT l, std::string v)
{
    return std::make_shared<LeafLexeme>(l, v);
}

std::shared_ptr<NodeLexeme>
makeNullNode()
{
    return makeNode(OP::NULL_VAL, makeLeaf(LIT::IDENTIFIER, ""),
                    makeLeaf(LIT::IDENTIFIER, ""));
}
