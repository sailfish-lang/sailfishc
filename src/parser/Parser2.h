/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../errorhandler/Error2.h"
#include "../errorhandler/Parser2ErrorHandler.h"
#include "../lexar/Lexar2.h"
#include "../lexar/Token2.h"
#include "Lexeme.h"
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

class Parser2
{
  private:
    std::unique_ptr<Lexar2> lexar;
    std::unique_ptr<Token2> currentToken;
    std::unique_ptr<Parser2ErrorHandler> errorhandler;

    // helper for simplifying redundancy of recursive loops
    template <typename F>
    NodePtr
    getChain(bool eq, TokenKind k, OP op, const F& f)
    {
        if (((currentToken->kind == k) && (eq)) ||
            ((currentToken->kind != k) && (!eq)))
        {
            return makeNullNode();
        }
        else if (currentToken->kind == TokenKind::EOF_)
        {
            errorhandler->handle(std::make_unique<Error2>(
                Error2(currentToken->col, currentToken->line,
                       "Unexpected end of file.",
                       "Expected to receive an end of token delimiter such "
                       "as '(' or '}'",
                       "", "")));
            return makeNullNode(); // unreachable
        }
        else
        {
            auto childL = f();
            auto childR =
                getChain(eq, k, op, f); // call recursively instead of vectorize
            return makeNode(op, std::move(childL), std::move(childR));
        }
    }

    // helper method for easy token advancement and
    // catching errors
    void advanceAndCheckToken(const TokenKind&);

    // parse methods
    NodePtr parseProgram();
    NodePtr parseSource();
    NodePtr parseSourcePart();
    NodePtr parseImportInfo();
    LeafPtr parseUDName();
    LeafPtr parseLocation();
    NodePtr parseUDT();
    NodePtr parseUserDefinedType();
    NodePtr parseAttributes();
    NodePtr parseMethods();
    NodePtr parseMethodsRecurse();
    NodePtr parseScript();
    NodePtr parseScript_();
    NodePtr parseFunctionDefinition();
    NodePtr parseFunctionInfo();
    NodePtr parseFunctionInOut();
    NodePtr parseStart();
    NodePtr parseBlock();
    Lexeme parseStatement();
    NodePtr parseTree();
    NodePtr parseBranch();
    Lexeme parseGrouping();
    NodePtr parseReturn();
    NodePtr parseDeclaration();
    Lexeme parseE0();
    Lexeme parseE1(Lexeme);
    Lexeme parseE2(Lexeme);
    Lexeme parseE3(Lexeme);
    Lexeme parseE4(Lexeme);
    Lexeme parseE5(Lexeme);
    Lexeme parseE6(Lexeme);
    Lexeme parseE7(Lexeme);
    Lexeme parseE8(Lexeme);
    Lexeme parseE9(Lexeme);
    Lexeme parseE10(Lexeme);
    Lexeme parseE11(Lexeme);
    Lexeme parseE12(Lexeme);
    NodePtr parseMemberAccess();
    NodePtr parseAttributeAccess();
    NodePtr parseMethodAccess();
    NodePtr parseFunctionCall();
    NodePtr parseNew();
    NodePtr parseUDTDec();
    NodePtr parseUDTDecItem();
    Lexeme parseT();
    LeafPtr parsePrimary();
    NodePtr parseVariable();
    LeafPtr parseType();
    LeafPtr parseBoolean();
    LeafPtr parseNumber();
    LeafPtr parseInteger();
    LeafPtr parseFloat();
    LeafPtr parseString();
    LeafPtr parseIdentifier();
    LeafPtr parseList();
    LeafPtr parseListType();

    // helper method for retreiving the Leaf/Node ptr from a Lexeme variant
    template <typename F>
    void
    lexemeIt(Lexeme l, F f)
    {
        auto i = l.index();
        if (i == 0)
            postorder(std::move(std::get<LeafPtr>(l)), f);
        else if (i == 1)
            postorder(std::move(std::get<NodePtr>(l)), f);
    }

  public:
    Parser2(const std::string& filename);
    NodePtr parse();

    template <typename F>
    void
    postorder(NodePtr n, F f)
    {
        if (n != nullptr && n->op != OP::NULL_VAL)
        {
            // root
            f(disp(n->op));
            // left
            lexemeIt(std::move(n->left), f);
            // right
            lexemeIt(std::move(n->right), f);
        }
    }

    template <typename F>
    void
    postorder(LeafPtr n, F f)
    {
        f(n->value);
    }
};
