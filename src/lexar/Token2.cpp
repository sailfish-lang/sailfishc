#include "Token2.h"

std::string
Tokenn::displayKind(const Kind& k)
{
    switch (k)
    {
    case START:
        return "START";
    case OWN_ACCESSOR:
        return "START";
    case TREE:
        return "TREE";
    case BRANCH:
        return "BRANCH";
    case UAT:
        return "UAT";
    case UFA:
        return "UFA";
    case FUN:
        return "FUN";
    case DEC:
        return "DEC";
    case EXP:
        return "EXP";
    case IMPORT:
        return "IMPORT";
    case RETURN:
        return "RETURN";
    case NEW:
        return "NEW";
    case IDENTIFIER:
        return "IDENTIFIER";
    case INTEGER:
        return "INTEGER";
    case FLOAT:
        return "FLOAT";
    case BOOL:
        return "BOOL";
    case STRING:
        return "STRING";
    case MODULO:
        return "MODULO";
    case ADDITION:
        return "ADDITION";
    case ADDTO:
        return "ADDTO";
    case SUBTRACTION:
        return "SUBTRACTION";
    case SUBFROM:
        return "SUBFROM";
    case MULTIPLICATION:
        return "MULTIPLICATION";
    case MULTTO:
        return "MULTTO";
    case DIVISION:
        return "DIVISION";
    case DIVFROM:
        return "DIVFROM";
    case GREATER_THAN:
        return "GREATER_THAN";
    case GREATER_THAN_OR_EQUALS:
        return "GREATER_THAN_OR_EQUALS";
    case LESS_THAN:
        return "LESS_THAN";
    case LESS_THAN_OR_EQUALS:
        return "LESS_THAN_OR_EQUALS";
    case AND:
        return "AND";
    case OR:
        return "OR";
    case EQUIVALENCE:
        return "EQUIVALENCE";
    case NONEQUIVALENCE:
        return "NONEQUIVALENCE";
    case ASSIGNMENT:
        return "ASSIGNMENT";
    case NEGATION:
        return "NEGATION";
    case COMMENT:
        return "COMMENT";
    case UNDERSCORE:
        return "UNDERSCORE";
    case COMMA:
        return "COMMA";
    case LCURLEY:
        return "LCURLEY";
    case RCURLEY:
        return "RCURLEY";
    case LPAREN:
        return "LPAREN";
    case RPAREN:
        return "RPAREN";
    case DOT:
        return "DOT";
    case TRIPLE_DOT:
        return "TRIPLE_DOT";
    case PIPE:
        return "PIPE";
    case COLON:
        return "COLON";
    case ERROR:
        return "ERROR";
    case EOF_:
        return "EOF_";
    default:
        return "Unrecognized Token kind";
    }
}

std::string
prettifyFormatToken(const Token2& t)
{
    return "[" + std::to_string(t.line) + ":" + std::to_string(t.col) +
           "]: " + displayKind(t.kind) + " - " + t.value + '\n';
}