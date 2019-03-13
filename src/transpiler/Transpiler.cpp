/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Transpiler.h"

std::string
builtinFunctionTranslator(std::string name)
{
    if (name == "display_str")
        return "printf(\"%s\",";

    if (name == "display_int")
        return "printf(\"%d\",";

    if (name == "display_flt")
        return "printf(\"%f\",";

    else
        return name + "(";
}

std::string
builtinTypesTranslator(std::string type)
{
    if (type == "int")
        return "int";

    if (type == "flt")
        return "float";

    if (type == "str")
        return "char*";

    if (type == "bool")
        return "int";

    else
        return type;
}

void
Transpiler::transpile()
{
    // capture and open file
    std::ofstream myfile;
    myfile.clear();
    myfile.open(filename);

    // traverse the tree
    visit(root);

    // write out header
    myfile << fileBuffer;

    // close the file
    myfile.close();
}

/**
 * Initial Execution Body: mapped to classic c main function
 */
void
Transpiler::visit(ast::InitialExecutionBody* node)
{
    fileBuffer += "int\nmain()\n{\n";
    visit(node->getBody());
    fileBuffer += "\n}\n";
}

/**
 * Function Definition:
 *      Output
 *      Name(param,..., param)
 *      { body }
 */
void
Transpiler::visit(ast::FunctionDefinition* node)
{
    visit(node->getOutput());

    fileBuffer += "\n";

    visit(node->getName());

    fileBuffer += "(";

    for (int i = 0; i < node->getInputList().size(); i++)
    {
        visit(node->getInputList().at(i));

        // no comma following the last argument
        if (i != node->getInputList().size() - 1)
        {
            fileBuffer += ", ";
        }
    }

    fileBuffer += ")\n{\n";

    visit(node->getBody());

    fileBuffer += "\n}\n\n";
}

/**
 * Function Call: identifier with arguments surrounded in parenthesis
 */
void
Transpiler::visit(ast::FunctionCall* node)
{
    ast::Identifier* name = node->getName();
    std::vector<ast::Primary*> args = node->getArguments();

    // catch built in functions here and add the open/left paren
    // for now assume display prints exactly one variable
    std::string translatedName = builtinFunctionTranslator(name->getValue());
    fileBuffer += translatedName;

    for (int i = 0; i < args.size(); i++)
    {
        visit(args.at(i));

        // no comma following the last argument
        if (i != args.size() - 1)
        {
            fileBuffer += ", ";
        }
    }

    fileBuffer += ")";
}

/**
 * Identifier: mapped directly to identifier
 */
void
Transpiler::visit(ast::Identifier* node)
{
    fileBuffer += node->getValue();
}

/**
 * Boolean: mapped directly to boolean
 */
void
Transpiler::visit(ast::BooleanLiteral* node)
{
    fileBuffer += node->getValue() == "true" ? "1" : "0";
}

/**
 * Integer: mapped directly to integer
 */
void
Transpiler::visit(ast::IntegerLiteral* node)
{
    fileBuffer += node->getNum();
}
void
Transpiler::visit(ast::FloatLiteral* node)
{
    fileBuffer += node->getNum();
}

/**
 * String: mapped directly to string
 */
void
Transpiler::visit(ast::StringLiteral* node)
{
    fileBuffer += node->getString();
}

/**
 * Variable: put a space between name and type
 */
void
Transpiler::visit(ast::Variable* node)
{
    visit(node->getType());

    fileBuffer += " ";

    visit(node->getName());
}

/**
 * Typename: mapped directly to string
 */
void
Transpiler::visit(ast::Typename* node)
{
    fileBuffer += builtinTypesTranslator(node->getType());
}

/**
 * Return: just add a return in front of the variable and tack on a semi-colon
 * at the end
 */
void
Transpiler::visit(ast::ReturnStatement* node)
{
    fileBuffer += "return ";

    visit(node->getBinaryExpression());
}

/**
 * Block: properly space the statements
 */
void
Transpiler::visit(ast::Block* node)
{
    std::vector<ast::Statement*> statements = node->getStatements();
    for (int i = 0; i < statements.size(); i++)
    {
        ast::Statement* statement = statements.at(i);

        fileBuffer += "    ";

        ast::Statement::StatementType type = statement->getStatementType();

        switch (type)
        {
        case ast::Statement::IfStatement:
        {
            ast::IfStatement* subnode =
                dynamic_cast<ast::IfStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::ReturnStatement:
        {
            ast::ReturnStatement* subnode =
                dynamic_cast<ast::ReturnStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BlockStatement:
        {
            ast::Block* subnode = dynamic_cast<ast::Block*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::GeneralDecleration:
        {
            ast::GeneralDecleration* subnode =
                dynamic_cast<ast::GeneralDecleration*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BinaryExpressionStatement:
        {
            ast::BinaryExpression* subnode =
                dynamic_cast<ast::BinaryExpression*>(statement);
            visit(subnode);
            break;
        }
        }

        if (i != statements.size() - 1)
        {
            fileBuffer += "\n";
        }
    }
}

/**
 * If statement: provide curleys and structure
 */
void
Transpiler::visit(ast::IfStatement* node)
{
    fileBuffer += "if";

    visit(node->getIfConditional());

    fileBuffer += "\n    {\n    ";

    visit(node->getIfStatements());

    fileBuffer += "\n    } else {\n    ";

    visit(node->getElseStatements());

    fileBuffer += "\n    }";
}

void
Transpiler::visit(ast::BinaryExpression* node)
{
    switch (node->getBinaryExpressionType())
    {
    case ast::BinaryExpression::BinaryCompOrArith:
    {
        ast::BinaryCompOrArith* subnode =
            dynamic_cast<ast::BinaryCompOrArith*>(node);
        visit(subnode);
        break;
    }
    case ast::BinaryExpression::Assignment:
    {
        ast::Assignment* subnode = dynamic_cast<ast::Assignment*>(node);
        visit(subnode);
        break;
    }
    case ast::BinaryExpression::ExpressionOnlyStatement:
    {
        ast::ExpressionOnlyStatement* subnode =
            dynamic_cast<ast::ExpressionOnlyStatement*>(node);
        visit(subnode);
        break;
    }
    }
}

/**
 * Binary Comparison or Arithmetic: put a sign and spacing in the middle
 */
void
Transpiler::visit(ast::BinaryCompOrArith* node)
{
    visit(node->getLeftExpr());

    fileBuffer += node->getOperation();

    visit(node->getRightExpr());
}

void
Transpiler::visit(ast::Assignment* node)
{
    visit(node->getLeftExpr());

    fileBuffer += " = ";

    visit(node->getRightExpr());
}

void
Transpiler::visit(ast::Negation* node)
{
    fileBuffer += "!";
    visit(node->getBinaryExpression());
}

/**
 * Grouping Statement: surround with parenthesis
 */
void
Transpiler::visit(ast::GroupingExpression* node)
{
    fileBuffer += "(";
    visit(node->getBinaryExpressionList());
    // TODO: fix hack for getting rid of inappropriate semi-colon
    fileBuffer = fileBuffer.substr(0, fileBuffer.length() - 1);
    fileBuffer += ")";
}

/**
 * Primitive Decleration: add an equals sign in between and a semi-colon at the
 * end
 */
void
Transpiler::visit(ast::PrimitiveDefition* node)
{
    visit(node->getVariable());

    fileBuffer += " = ";

    visit(node->getBinaryExpression());
}

/**
 * Expression Only: the tail end of binary expressions where a semi-colon is
 * added
 */
void
Transpiler::visit(ast::ExpressionOnlyStatement* node)
{
    visit(node->getLeftExpr());
    fileBuffer += ";";
}
