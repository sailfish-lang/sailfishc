
#include "./TypeChecker.h"
#include "../errorhandler/Error.h"
#include "SymbolTable.h"

// ------- Ugly/Hacky Helper Functions ------- //

ast::Expression*
binaryExpressionHelper(ast::BinaryExpression* node)
{
    switch (node->getBinaryExpressionType())
    {
    case ast::BinaryExpression::Exponentiation:
    {
        ast::Exponentiation* subnode = dynamic_cast<ast::Exponentiation*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::Multiplication:
    {
        ast::Multiplication* subnode = dynamic_cast<ast::Multiplication*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::Division:
    {
        ast::Division* subnode = dynamic_cast<ast::Division*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::Modulo:
    {
        ast::Modulo* subnode = dynamic_cast<ast::Modulo*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::Addition:
    {
        ast::Addition* subnode = dynamic_cast<ast::Addition*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::Subtraction:
    {
        ast::Subtraction* subnode = dynamic_cast<ast::Subtraction*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::BinaryGreaterThan:
    {
        ast::BinaryGreaterThan* subnode =
            dynamic_cast<ast::BinaryGreaterThan*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::BinaryLessThan:
    {
        ast::BinaryLessThan* subnode = dynamic_cast<ast::BinaryLessThan*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::BinaryGreaterThanOrEqual:
    {
        ast::BinaryGreaterThanOrEqual* subnode =
            dynamic_cast<ast::BinaryGreaterThanOrEqual*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::BinaryLessThanOrEqual:
    {
        ast::BinaryLessThanOrEqual* subnode =
            dynamic_cast<ast::BinaryLessThanOrEqual*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::EquivalenceComparison:
    {
        ast::EquivalenceComparison* subnode =
            dynamic_cast<ast::EquivalenceComparison*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::NonEquivalenceComparison:
    {
        ast::NonEquivalenceComparison* subnode =
            dynamic_cast<ast::NonEquivalenceComparison*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::AndComparison:
    {
        ast::AndComparison* subnode = dynamic_cast<ast::AndComparison*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::OrComparison:
    {
        ast::OrComparison* subnode = dynamic_cast<ast::OrComparison*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::Assignment:
    {
        ast::Assignment* subnode = dynamic_cast<ast::Assignment*>(node);
        return subnode->getLeftExpr();
    }
    case ast::BinaryExpression::ExpressionOnlyStatement:
    {
        ast::ExpressionOnlyStatement* subnode =
            dynamic_cast<ast::ExpressionOnlyStatement*>(node);
        return subnode->getExpression();
    }
    case ast::BinaryExpression::FunctionCallExpression:
    {
        ast::FunctionCall* subnode = dynamic_cast<ast::FunctionCall*>(node);
        return subnode->getExpr();
    }
    }
}

std::string
primaryHelper(ast::Primary* primary)
{
    ast::Primary::PrimaryType type = primary->getPrimaryType();

    switch (type)
    {
    case ast::Primary::Identifier:
    {
        ast::Identifier* subnode = dynamic_cast<ast::Identifier*>(primary);
        return subnode->getValue();
    }
    case ast::Primary::StringLiteral:
    {
        ast::StringLiteral* subnode =
            dynamic_cast<ast::StringLiteral*>(primary);
        return "String";
    }
    case ast::Primary::BooleanLiteral:
    {
        ast::BooleanLiteral* subnode =
            dynamic_cast<ast::BooleanLiteral*>(primary);
        return "Boolean";
    }
    case ast::Primary::IntegerLiteral:
    {
        ast::IntegerLiteral* subnode =
            dynamic_cast<ast::IntegerLiteral*>(primary);
        return "Integer";
    }
    case ast::Primary::FloatLiteral:
    {
        ast::FloatLiteral* subnode = dynamic_cast<ast::FloatLiteral*>(primary);
        return "Float";
    }
    }
}

std::string
expressionHelper(ast::Expression* node, ErrorHandler* seh)
{
    switch (node->getExpressionType())
    {
    case ast::Expression::GroupingExpression:
    {
        ast::GroupingExpression* subnode =
            dynamic_cast<ast::GroupingExpression*>(node);

        seh->handle(new Error(subnode->getLineNum(),
                              "Unexpected grouping inside binary expression. "
                              "Groupings cannot be nested."));
        // return list type to continue semantic analysis
        return "List";
    }
    case ast::Expression::ArrayExpression:
    {
        ast::ArrayExpression* subnode =
            dynamic_cast<ast::ArrayExpression*>(node);

        seh->handle(new Error(subnode->getLineNum(),
                              "Unexpected list inside binary expression."));
        // return list type to continue semantic analysis
        return "List";
    }
    case ast::Expression::PrimaryExpression:
    {
        ast::PrimaryExpression* subnode =
            dynamic_cast<ast::PrimaryExpression*>(node);

        return primaryHelper(subnode->getPrimary());
    }
    case ast::Expression::UnaryExpression:
    {
        ast::UnaryExpression* subnode =
            dynamic_cast<ast::UnaryExpression*>(node);

        // must be a boolean so fine to return boolean here
        return "Boolean";
    }
    }
}

std::string
getRightExpressionType(ast::ExpressionStatement* node,
                       ErrorHandler* errorHandler)
{
    return expressionHelper(binaryExpressionHelper(node->getBinaryExpression()),
                            errorHandler);
}

// ------- END ------- //

void
TypeChecker::check(ast::Start* root)
{
    visit(root);
}

void
TypeChecker::visit(ast::NewVariableDefinition* node)
{
    // capture variable information to add to the symbol table
    ast::Variable* var = node->getVariable();
    std::string name = var->getName()->getValue();
    std::string type = var->getType()->getType();

    bool isUnique =
        symbolTable->addSymbol(name, type, Symbol::SymbolType::Primitive);

    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            var->getName()->getLineNum(),
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpressionStatement());
}

void
TypeChecker::visit(ast::ListDefinition* node)
{
    std::string name = node->getName()->getValue();
    std::string type = node->getType()->getType();

    bool isUnique =
        symbolTable->addSymbol(name, type + "[]", Symbol::SymbolType::List);

    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getName()->getLineNum(),
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpression());
}

void
TypeChecker::visit(ast::DictionaryDefinition* node)
{
    std::string name = node->getName()->getValue();
    std::string keyType = node->getKeyType()->getType();
    std::string valueType = node->getValueType()->getType();

    bool isUnique = symbolTable->addSymbol(
        name, "dictionary", keyType, valueType, Symbol::SymbolType::Dictionary);

    if (!isUnique)
    {
        symbolTableErrorHandler->handle(new Error(
            node->getName()->getLineNum(),
            "Invalid redecleration of a variable with name: " + name + "."));
    }

    // visit expression
    visit(node->getExpression());
}

void
TypeChecker::visit(ast::FunctionDefinition* node)
{

    // ensure scope is global since functions cannot be in blocks
    if (!symbolTable->isGlobalScope())
    {
        symbolTableErrorHandler->handle(
            new Error(node->getName()->getLineNum(),
                      "Functions can only be declared at the global level, "
                      "i.e. cannot be nested!"));
    }

    std::string name = node->getName()->getValue();

    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    //  capture for the addition of the function to the symbol table
    for (auto const& input : node->getInputList())
    {
        ast::Variable* var = input->getInput();
        std::string inp_type = var->getType()->getType();

        // add to list of function inputs
        inputs.push_back(inp_type);
    }

    // capture for the addition of the function to the symbol table
    for (auto const& output : node->getOutputList())
    {
        ast::Typename* var = output->getOutput();
        std::string out_type = var->getType();

        outputs.push_back(out_type);
    }

    symbolTable->addSymbol(name, "function", inputs, outputs,
                           Symbol::SymbolType::Function);

    // enter a scope for this function, starting with the parameters
    symbolTable->enterScope();

    // TODO: don't visit each param for a second time here!
    //  add each input to the nested scope for the function
    for (auto const& input : node->getInputList())
    {
        ast::Variable* var = input->getInput();
        std::string inp_name = var->getName()->getValue();
        std::string inp_type = var->getType()->getType();

        // add to the symbol table
        bool isUnique = symbolTable->addSymbol(inp_name, inp_type,
                                               Symbol::SymbolType::Dictionary);

        if (!isUnique)
        {
            symbolTableErrorHandler->handle(new Error(
                node->getName()->getLineNum(),
                "Invalid redecleration of a variable with name: " + name +
                    "."));
        }
    }

    // visit the function body
    visit(node->getBody());

    // exit scope once we exit the body
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::InitialExecutionBody* node)
{
    // enter and exit scope for initial execution body
    symbolTable->enterScope();
    visit(node->getBody());
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::IfStatement* node)
{
    visit(node->getIfConditional());

    // enter and exit scope for if statement body
    symbolTable->enterScope();
    visit(node->getIfStatements());
    symbolTable->exitScope();

    // enter and exit scope for else statement body
    symbolTable->enterScope();
    visit(node->getElseStatements());
    symbolTable->exitScope();
}

void
TypeChecker::visit(ast::Negation* node)
{
    visit(node->getExpressionStatement());

    // ensure that all negations are boolean
    std::string type = getRightExpressionType(node->getExpressionStatement(),
                                              semanticErrorHandler);
    if (type != "Boolean")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected boolean type following negation. Instead received: " +
                type + "."));
    }
}

void
TypeChecker::visit(ast::BinaryGreaterThan* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Expected the same types on each side of greater than "
                      "comparison. Instead recevied: " +
                          lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of greater than "
                                "comparison. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Expected boolean type on right side of greater than "
                      "comparison. Instead received: " +
                          rType + "."));
    }
}
void
TypeChecker::visit(ast::BinaryLessThan* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Expected the same types on each side of less than "
                      "comparison. Instead recevied: " +
                          lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of less than "
                                "comparison. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Expected boolean type on right side of less than "
                      "comparison. Instead received: " +
                          rType + "."));
    }
}
void
TypeChecker::visit(ast::BinaryGreaterThanOrEqual* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected the same types on each side of greater than or equal to "
            "comparison. Instead recevied: " +
                lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of greater than or equal to "
                                "comparison. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected boolean type on right side of greater than or equal to "
            "comparison. Instead received: " +
                rType + "."));
    }
}
void
TypeChecker::visit(ast::BinaryLessThanOrEqual* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected the same types on each side of less than or equal to "
            "comparison. Instead recevied: " +
                lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of less than or equal to "
                                "comparison. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(),
            "Expected boolean type on right side of less than or equal to "
            "comparison. Instead received: " +
                rType + "."));
    }
}
void
TypeChecker::visit(ast::EquivalenceComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Expected the same types on each side of equivalence "
                      "comparison. Instead recevied: " +
                          lType + " and " + rType + "."));
    }
}
void
TypeChecker::visit(ast::NonEquivalenceComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(),
                      "Expected the same types on each side of nonequivalence "
                      "comparison. Instead recevied: " +
                          lType + " and " + rType + "."));
    }
}
void
TypeChecker::visit(ast::AndComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != "Boolean")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected boolean type on left side of and "
                                "comparison. Instead received: " +
                                    lType + "."));
    }

    else if (rType != "Boolean")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected boolean type on right side of and "
                                "comparison. Instead received: " +
                                    rType + "."));
    }
}
void
TypeChecker::visit(ast::OrComparison* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != "Boolean")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected boolean type on left side of or "
                                "comparison. Instead received: " +
                                    lType + "."));
    }

    else if (rType != "Boolean")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected boolean type on right side of or "
                                "comparison. Instead received: " +
                                    rType + "."));
    }
}

void
TypeChecker::visit(ast::Exponentiation* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != "Integer")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Integer type on left "
                                "side of exponentiation. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Integer type on left "
                                "side of exponentiation. Instead received: " +
                                    rType + "."));
    }
}
void
TypeChecker::visit(ast::Multiplication* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected the same types on each side of "
                                "multiplication. Instead recevied: " +
                                    lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of multiplication. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of multiplication. Instead received: " +
                                    rType + "."));
    }
}
void
TypeChecker::visit(ast::Division* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected the same types on each side of "
                                "division. Instead recevied: " +
                                    lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of division. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of division. Instead received: " +
                                    rType + "."));
    }
}
void
TypeChecker::visit(ast::Modulo* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != "Integer")
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(), "Integer type on left "
                                          "side of modulo. Instead received: " +
                                              lType + "."));
    }
    else if (rType != "Integer")
    {
        semanticErrorHandler->handle(
            new Error(node->getLineNum(), "Integer type on left "
                                          "side of modulo. Instead received: " +
                                              rType + "."));
    }
}
void
TypeChecker::visit(ast::Addition* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected the same types on each side of "
                                "addition. Instead recevied: " +
                                    lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of addition. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of addition. Instead received: " +
                                    rType + "."));
    }
}
void
TypeChecker::visit(ast::Subtraction* node)
{
    visit(node->getLeftExpr());
    visit(node->getRightExpr());

    std::string lType =
        expressionHelper(node->getLeftExpr(), semanticErrorHandler);
    std::string rType =
        getRightExpressionType(node->getRightExpr(), semanticErrorHandler);

    if (lType != rType)
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected the same types on each side of "
                                "subtraction. Instead recevied: " +
                                    lType + " and " + rType + "."));
    }
    else if (lType != "Integer" && lType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of subtraction. Instead received: " +
                                    lType + "."));
    }
    else if (rType != "Integer" && rType != "Float")
    {
        semanticErrorHandler->handle(new Error(
            node->getLineNum(), "Expected either float or integer type on left "
                                "side of subtraction. Instead received: " +
                                    rType + "."));
    }
}