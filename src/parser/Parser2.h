/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
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
            // error
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
    std::shared_ptr<NodeLexeme> parseFunctionInputs();
    std::shared_ptr<LeafLexeme> parseFunctionOutput();
    std::shared_ptr<NodeLexeme> parseStart();
    std::shared_ptr<NodeLexeme> parseBlock();
    std::shared_ptr<NodeLexeme> parseStatement();
    std::shared_ptr<NodeLexeme> parseTree();
    std::shared_ptr<NodeLexeme> parseBranch();
    std::shared_ptr<NodeLexeme> parseGrouping();
    std::shared_ptr<NodeLexeme> parseReturn();
    std::shared_ptr<NodeLexeme> parseDeclaration();
    std::shared_ptr<NodeLexeme> parseE0();
    std::shared_ptr<NodeLexeme> parseE1(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE2(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE3(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE4(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE5(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE6(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE7(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE8(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE9(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE10(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseE11(std::shared_ptr<NodeLexeme>);
    std::shared_ptr<NodeLexeme> parseMemberAccess();
    std::shared_ptr<NodeLexeme> parseAttributeAccess();
    std::shared_ptr<NodeLexeme> parseMethodAccess();
    std::shared_ptr<NodeLexeme> parseFunctionCall();
    std::shared_ptr<NodeLexeme> parseNew();
    std::shared_ptr<NodeLexeme> parseUDTDec();
    std::shared_ptr<NodeLexeme> parseUDTDecItem();
    std::shared_ptr<NodeLexeme> parseT();
    std::shared_ptr<LeafLexeme> parsePrimary();
    std::shared_ptr<NodeLexeme> parseVariable();
    std::shared_ptr<LeafLexeme> parseType();
    std::shared_ptr<LeafLexeme> parseBoolean();
    std::shared_ptr<LeafLexeme> parseNumber();
    std::shared_ptr<LeafLexeme> parseInteger();
    std::shared_ptr<LeafLexeme> parseFloat();
    std::shared_ptr<LeafLexeme> parseString();
    std::shared_ptr<LeafLexeme> parseIdentifier();

  public:
    Parser2(const std::string& filename);

    std::shared_ptr<NodeLexeme> parse();

    void
    postorder(std::shared_ptr<NodeLexeme> p, int indent = 0)
    {
        if (p != NULL && p->op != OP::NULL_VAL)
        {
            if (indent)
            {
                std::cout << std::setw(indent) << ' ';
            }
            std::cout << disp(p->op) << "\n ";
            auto il = p->left.index();

            if (il == 0)
            {
                auto w = std::get<std::shared_ptr<NodeLexeme>>(p->left);
                if (w)
                    postorder(w, indent + 4);
            }
            else if (il == 1)
            {
                auto w = std::get<std::shared_ptr<LeafLexeme>>(p->left);
                if (w)
                    postorder(w, indent + 4);
            }

            auto ir = p->right.index();
            if (ir == 0)
            {
                auto w = std::get<std::shared_ptr<NodeLexeme>>(p->right);
                if (w)
                    postorder(w, indent + 4);
            }
            else if (ir == 1)
            {
                auto w = std::get<std::shared_ptr<LeafLexeme>>(p->right);
                if (w)
                    postorder(w, indent + 4);
            }
        }
    }
    void
    postorder(std::shared_ptr<LeafLexeme> p, int indent = 0)
    {
        if (indent)
        {
            std::cout << std::setw(indent) << ' ';
        }
        std::cout << disp(p->lit) << ": " << p->value << "\n ";
    }
};
