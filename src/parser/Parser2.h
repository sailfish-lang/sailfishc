/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include "../errorhandler/Error2.h"
#include "../errorhandler/Parser2ErrorHandler.h"
#include "../errorhandler/SemanticAnalyzerErrorHandler.h"
#include "../lexar/Lexar2.h"
#include "../lexar/Token2.h"
#include "../semantics/SymbolTable.h"
#include "../semantics/UDTTable.h"
#include "Lexeme.h"
#include <cstdarg>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <tuple>
#include <variant>
#include <vector>

using LSandFlag = std::tuple<Lexeme, std::string, std::string>;
using SandS = std::tuple<std::string, std::string>;
using LandS = std::tuple<Lexeme, std::string>;
using LandV = std::tuple<Lexeme, std::variant<LIT, OP>>;
using LandSt = std::tuple<Lexeme, std::shared_ptr<SymbolTable>>;
using LSandS = std::tuple<Lexeme, std::string, std::string>;
using UdtAndFlag = std::tuple<std::unique_ptr<UDTTable>, bool>;

class Parser2
{
  private:
    std::unique_ptr<Lexar2> lexar;
    std::unique_ptr<Token2> currentToken;
    std::unique_ptr<Parser2ErrorHandler> errorhandler;
    std::unique_ptr<SemanticAnalyzerErrorHandler> semanticerrorhandler;
    std::shared_ptr<SymbolTable> symboltable;
    std::unique_ptr<UDTTable> udttable;
    std::string filename;
    bool isUdt;

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
    LIT kindToLIT(TokenKind);
    std::string tokenToType(const TokenKind&, const std::string&);

    // some work to simplify all the expression parsing
    template <typename G>
    LandS
    simpleExpr(TokenKind tk, OP op, const std::string& T0, const G& g)
    {
        advanceAndCheckToken(tk); // consume token
        auto a = parseE0();
        auto e0 = std::move(std::get<0>(a));
        auto type = std::get<1>(a);

        auto actualType = g(T0, type);

        return std::make_tuple(makeNode(op, T0, std::move(e0)), actualType);
    }

    template <typename F, typename G>
    LandS
    parseExpr(const std::string& T0, const F& f, const G& g)
    {

        auto b = f(T0);
        auto t0 = std::move(std::get<0>(b));
        auto type = std::get<1>(b);

        return std::make_tuple(std::move(t0), type);
    }

    template <typename F, typename G, typename T, typename... Rest>
    LandS
    parseExpr(const std::string& T0, const F& f, const G& g, T t, Rest... rest)
    {
        auto tk = std::get<0>(t);
        auto op = std::get<1>(t);

        if (currentToken->kind == tk)
            return simpleExpr(tk, op, T0, g);

        return parseExpr(T0, f, g, rest...);
    }

    // extracts type from lexemes
    LandS extractType(Lexeme l);

    // semantic checker methods
    void checkType(const std::string&, const std::string&);
    void checkUnique(const std::string&);
    void checkUnique(const std::string&, std::shared_ptr<SymbolTable>);
    void checkExists(const std::string&);
    void checkExists(const std::string&, std::shared_ptr<SymbolTable>);
    void checkUDTExists(const std::string&);
    LandS checkFunctionCall(const std::string&, std::shared_ptr<SymbolTable>);
    std::string parseFunctionReturnType(const std::string& s);
    std::vector<std::string> parseFunctionInputTypes(const std::string&);

    // parse methods
    NodePtr parseProgram();
    NodePtr parseSource();
    NodePtr parseSourcePart();
    NodePtr parseImportInfo();
    LeafPtr parseUDName();
    LeafPtr parseLocation();
    NodePtr parseUDT();
    NodePtr parseUserDefinedType();
    LandSt parseAttributes();
    LandSt parseMethods();
    NodePtr parseMethodsRecurse();
    NodePtr parseScript();
    NodePtr parseScript_();
    NodePtr parseFunctionDefinition();
    LandS parseFunctionInfo();
    LandS parseFunctionInOut();
    NodePtr parseStart();
    LandS parseBlock();
    LSandFlag parseStatement();
    NodePtr parseTree();
    NodePtr parseBranch();
    Lexeme parseGrouping();
    LandS parseReturn();
    LandS parseDeclaration();
    LandS parseE0();
    LandS parseE1(const std::string&);
    LandS parseE2(const std::string&);
    LandS parseE3(const std::string&);
    LandS parseE4(const std::string&);
    LandS parseE5(const std::string&);
    LandS parseE6(const std::string&);
    LandS parseE7(const std::string&);
    LandS parseE8(const std::string&);
    LandS parseE9(const std::string&);
    LandS parseE10(const std::string&);
    LandS parseE11(const std::string&);
    LandS parseE12(const std::string&);
    LandS parseE13(const std::string&);
    LandS parseMemberAccess(const std::string&);
    LandS parseAttributeAccess(const std::string&);
    LandS parseMethodAccess(const std::string&);
    LandS parseFunctionCall();
    LandS parseNew();
    LandS parseUDTDec();
    LandS parseT();
    LandS parsePrimary();
    LSandS parseVariable();
    LeafPtr parseType();
    LeafPtr parseBoolean();
    LeafPtr parseNumber();
    LeafPtr parseInteger();
    LeafPtr parseFloat();
    LeafPtr parseString();
    LeafPtr parseIdentifier();
    LandS parseList();
    std::tuple<LeafPtr, std::string> parseListType();

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

    void
    display(LeafPtr n, int indent)
    {
        std::cout << std::setw(indent) << disp(n->lit) << ": " << n->value
                  << std::endl;
    }
    void
    lexemeIt(Lexeme l, int indent)
    {
        auto i = l.index();
        if (i == 0)
            display(std::move(std::get<LeafPtr>(l)), indent);
        else if (i == 1)
            display(std::move(std::get<NodePtr>(l)), indent);
    }

    void
    display(NodePtr n, int indent = 0)
    {
        if (n != nullptr && n->op != OP::NULL_VAL)
        {
            // root
            std::cout << std::setw(indent) << disp(n->op) << std::endl;
            // left
            lexemeIt(std::move(n->left), indent + 4);
            // right
            lexemeIt(std::move(n->right), indent + 4);
        }
    }

    template <typename F>
    void
    postorder(LeafPtr n, F f)
    {
        f(n->value);
    }

    std::shared_ptr<SymbolTable>
    getSymbolTable()
    {
        return symboltable;
    }

    std::unique_ptr<UDTTable>
    getUDTTable()
    {
        return std::move(udttable);
    }

    bool
    getIsUDTFlag()
    {
        return isUdt;
    }
};
