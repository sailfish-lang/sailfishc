#include "Token2.h"

std::string
displayKind(const TokenKind& k)
{
    switch (k)
    {
    case TokenKind::START:
        return "START";
    case TokenKind::OWN_ACCESSOR:
        return "OWN";
    case TokenKind::TREE:
        return "TREE";
    case TokenKind::UAT:
        return "UAT";
    case TokenKind::UFN:
        return "UFN";
    case TokenKind::FUN:
        return "FUN";
    case TokenKind::DEC:
        return "DEC";
    case TokenKind::IMPORT:
        return "IMPORT";
    case TokenKind::RETURN:
        return "RETURN";
    case TokenKind::NEW:
        return "NEW";
    case TokenKind::IDENTIFIER:
        return "IDENTIFIER";
    case TokenKind::INTEGER:
        return "INTEGER";
    case TokenKind::FLOAT:
        return "FLOAT";
    case TokenKind::BOOL:
        return "BOOL";
    case TokenKind::STRING:
        return "STRING";
    case TokenKind::LIST:
        return "LIST";
    case TokenKind::MODULO:
        return "MODULO";
    case TokenKind::ADDITION:
        return "ADDITION";
    case TokenKind::ADDTO:
        return "ADDTO";
    case TokenKind::SUBTRACTION:
        return "SUBTRACTION";
    case TokenKind::SUBFROM:
        return "SUBFROM";
    case TokenKind::MULTIPLICATION:
        return "MULTIPLICATION";
    case TokenKind::MULTTO:
        return "MULTTO";
    case TokenKind::EXPONENTIATION:
        return "EXPONENTIATION";
    case TokenKind::DIVISION:
        return "DIVISION";
    case TokenKind::DIVFROM:
        return "DIVFROM";
    case TokenKind::GREATER_THAN:
        return "GREATER_THAN";
    case TokenKind::GREATER_THAN_OR_EQUALS:
        return "GREATER_THAN_OR_EQUALS";
    case TokenKind::LESS_THAN:
        return "LESS_THAN";
    case TokenKind::LESS_THAN_OR_EQUALS:
        return "LESS_THAN_OR_EQUALS";
    case TokenKind::AND:
        return "AND";
    case TokenKind::OR:
        return "OR";
    case TokenKind::EQUIVALENCE:
        return "EQUIVALENCE";
    case TokenKind::NONEQUIVALENCE:
        return "NONEQUIVALENCE";
    case TokenKind::ASSIGNMENT:
        return "ASSIGNMENT";
    case TokenKind::NEGATION:
        return "NEGATION";
    case TokenKind::UNARYADD:
        return "UNARYADD";
    case TokenKind::UNARYMINUS:
        return "UNARYMINUS";
    case TokenKind::COMMENT:
        return "COMMENT";
    case TokenKind::UNDERSCORE:
        return "UNDERSCORE";
    case TokenKind::COMMA:
        return "COMMA";
    case TokenKind::LCURLEY:
        return "LCURLEY";
    case TokenKind::RCURLEY:
        return "RCURLEY";
    case TokenKind::LPAREN:
        return "LPAREN";
    case TokenKind::RPAREN:
        return "RPAREN";
    case TokenKind::DOT:
        return "DOT";
    case TokenKind::TRIPLE_DOT:
        return "TRIPLE_DOT";
    case TokenKind::PIPE:
        return "PIPE";
    case TokenKind::COLON:
        return "COLON";
    case TokenKind::LISTTYPE:
        return "LISTTYPE";
    case TokenKind::ERROR:
        return "ERROR";
    case TokenKind::EOF_:
        return "EOF_";
    default:
        return "Unrecognized Token kind";
    }
}

std::string
Token2::prettifyFormatToken()
{
    return "[" + std::to_string(line) + ":" + std::to_string(col) +
           "]: " + displayKind(kind) + " - " + value + '\n';
}