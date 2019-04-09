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
    std::shared_ptr<NodeLexeme>
    getChain(bool eq, TokenKind k, OP op, const F& f)
    {
        if (((currentToken->kind == k) && (eq)) ||
            ((currentToken->kind != k) && (!eq)))
        {
            return makeNullNode();
        }
        else if (currentToken->kind == TokenKind::EOF_)
        {
            errorhandler->handle(
                new Error2(currentToken->col, currentToken->line,
                           "Unexpected end of file.",
                           "Expected to receive an end of token delimiter such "
                           "as '(' or '}'",
                           "", ""));
            return makeNullNode(); // unreachable
        }
        else
        {
            auto childL = f();
            auto childR =
                getChain(eq, k, op, f); // call recursively instead of vectorize
            return makeNode(op, childL, childR);
        }
    }

    // helper method for easy token advancement and
    // catching errors
    void advanceAndCheckToken(const TokenKind&);

    // parse methods
    std::shared_ptr<NodeLexeme> parseProgram();
    std::shared_ptr<NodeLexeme> parseSource();
    std::shared_ptr<NodeLexeme> parseSourcePart();
    std::shared_ptr<NodeLexeme> parseImportInfo();
    std::shared_ptr<LeafLexeme> parseUDName();
    std::shared_ptr<LeafLexeme> parseLocation();
    std::shared_ptr<NodeLexeme> parseUDT();
    std::shared_ptr<NodeLexeme> parseUserDefinedType();
    std::shared_ptr<NodeLexeme> parseAttributes();
    std::shared_ptr<NodeLexeme> parseMethods();
    std::shared_ptr<NodeLexeme> parseMethodsRecurse();
    std::shared_ptr<NodeLexeme> parseScript();
    std::shared_ptr<NodeLexeme> parseScript_();
    std::shared_ptr<NodeLexeme> parseFunctionDefinition();
    std::shared_ptr<NodeLexeme> parseFunctionInfo();
    std::shared_ptr<NodeLexeme> parseFunctionInOut();
    std::shared_ptr<NodeLexeme> parseStart();
    std::shared_ptr<NodeLexeme> parseBlock();
    Lexeme parseStatement();
    std::shared_ptr<NodeLexeme> parseTree();
    std::shared_ptr<NodeLexeme> parseBranch();
    Lexeme parseGrouping();
    std::shared_ptr<NodeLexeme> parseReturn();
    std::shared_ptr<NodeLexeme> parseDeclaration();
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
    std::shared_ptr<NodeLexeme> parseMemberAccess();
    std::shared_ptr<NodeLexeme> parseAttributeAccess();
    std::shared_ptr<NodeLexeme> parseMethodAccess();
    std::shared_ptr<NodeLexeme> parseFunctionCall();
    std::shared_ptr<NodeLexeme> parseNew();
    std::shared_ptr<NodeLexeme> parseUDTDec();
    std::shared_ptr<NodeLexeme> parseUDTDecItem();
    Lexeme parseT();
    std::shared_ptr<LeafLexeme> parsePrimary();
    std::shared_ptr<NodeLexeme> parseVariable();
    std::shared_ptr<LeafLexeme> parseType();
    std::shared_ptr<LeafLexeme> parseBoolean();
    std::shared_ptr<LeafLexeme> parseNumber();
    std::shared_ptr<LeafLexeme> parseInteger();
    std::shared_ptr<LeafLexeme> parseFloat();
    std::shared_ptr<LeafLexeme> parseString();
    std::shared_ptr<LeafLexeme> parseIdentifier();
    std::shared_ptr<LeafLexeme> parseList();
    std::shared_ptr<LeafLexeme> parseListType();

  public:
    Parser2(const std::string& filename);
    std::shared_ptr<NodeLexeme> parse();

    template <typename F>
    void
    lexemeIt(Lexeme l, F f)
    {
        auto i = l.index();
        if (i == 0)
            postorder(std::get<std::shared_ptr<NodeLexeme>>(l), f);
        else if (i == 1)
            postorder(std::get<std::shared_ptr<LeafLexeme>>(l), f);
    }

    template <typename F>
    void
    postorder(std::shared_ptr<NodeLexeme> n, F f)
    {
        if (n != nullptr && n->op != OP::NULL_VAL)
        {
            // root
            f(disp(n->op));
            // left
            lexemeIt(n->left, f);
            // right
            lexemeIt(n->right, f);
        }
    }

    template <typename F>
    void
    postorder(std::shared_ptr<LeafLexeme> n, F f)
    {
        f(n->value);
    }
};
