#pragma once
#include "../ast/Ast.h"
#include "../common/ReservedWords.h"
#include "../errorhandler/SemanticErrorHandler.h"
#include "SymbolTable.h"
#include "UDTTable.h"

ast::Expression* binaryExpressionHelper(ast::BinaryExpression* node);

std::string primaryHelper(ast::Primary*, SymbolTable*, SemanticErrorHandler*,
                          UDTTable*);

std::string expressionHelper(ast::Expression*, SymbolTable*,
                             SemanticErrorHandler*, UDTTable*);

std::string getRightExpressionType(ast::BinaryExpression*, SymbolTable*,
                                   SemanticErrorHandler*, UDTTable*);

bool isLegalGrouping(ast::BinaryExpression*);

std::vector<std::string> getFunctionParamTypes(std::string);
std::string getFunctionReturnType(std::string);
void compareFunctions(std::vector<std::string>, std::vector<ast::Primary*>,
                      std::string, SymbolTable*, SemanticErrorHandler*,
                      UDTTable*);
