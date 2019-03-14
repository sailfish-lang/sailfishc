/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Transpiler.h"

std::string
builtinFunctionTranslator(std::string name)
{
    if (name == "display_str")
        return "print_str(";

    if (name == "display_int")
        return "print_int(";

    if (name == "display_flt")
        return "print_flt(";

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
    fileBuffer += "int\nmain()";
    visit(node->getBody());
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

    fileBuffer += ")";

    visit(node->getBody());

    fileBuffer += "";
}

/**
 * Function Definition (special):
 *      Output
 *      Name(UDT*, param,..., param)
 *      { body }
 */
void
Transpiler::visit(ast::FunctionDefinition* node, std::string udtTypeName)
{
    visit(node->getOutput());

    fileBuffer += "\n";

    visit(node->getName());

    fileBuffer += "(" + udtTypeName + "* _own_";

    // if only one non udt name given param and the param is not void add a
    // comma
    if (1 < node->getInputList().size() ||
        (node->getInputList().size() == 1 &&
         node->getInputList().at(0)->getInput()->getType()->getType() !=
             "void"))
    {
        fileBuffer += ", ";
    }

    for (int i = 0; i < node->getInputList().size(); i++)
    {
        /*
            no comma following the last argument and no comma if we get a void
            type and also no repeating the initial comma we placed above, so
            start with i at 1. Do not need a check for void here since void
            should not be possible as anything but the first and only param
        */
        if (i != 0 && i < node->getInputList().size())
        {
            fileBuffer += ", ";
        }

        visit(node->getInputList().at(i));
    }

    fileBuffer += ")";

    visit(node->getBody());

    fileBuffer += "";
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
    if (node->getType()->getType() != "void")
    {

        visit(node->getType());

        fileBuffer += " ";

        visit(node->getName());
    }
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
 * Return: just add a return in front of the variable and tack on a
 * semi-colon at the end
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
    fileBuffer += "\n{\n";
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
    fileBuffer += "\n}\n";
}

/**
 * If statement: provide curleys and structure
 */
void
Transpiler::visit(ast::IfStatement* node)
{
    fileBuffer += "if";

    visit(node->getIfConditional());

    visit(node->getIfStatements());

    fileBuffer += "else ";

    visit(node->getElseStatements());
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
 * Primitive Decleration: add an equals sign in between and a semi-colon at
 * the end
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

/**
 * UserDefinedTypeDefinition:
 */
void
Transpiler::visit(ast::UserDefinedTypeDefinition* node)
{
    // ---------        generate the structure        -------- //
    fileBuffer += "typedef struct _";
    visit(node->getName());
    fileBuffer += "_ {\n";

    for (auto const& attribute : node->getAttributes())
    {
        fileBuffer += "    ";
        visit(attribute);
        fileBuffer += ";\n";
    }

    fileBuffer += "} ";
    visit(node->getName());
    fileBuffer += ";\n\n";

    // ---------        define a sort of constructor       -------- //
    visit(node->getName());
    fileBuffer += "*\nconstruct_";
    visit(node->getName());
    fileBuffer += "(";

    int i = 0;
    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute);
        fileBuffer += "_";
        if (i != node->getAttributes().size() - 1)
            fileBuffer += ",";
    }
    fileBuffer += ")\n{\n    ";
    visit(node->getName());
    fileBuffer += "* a = (";
    visit(node->getName());
    fileBuffer += "*)malloc(sizeof(";
    visit(node->getName());
    fileBuffer += "));\n";
    for (auto const& attribute : node->getAttributes())
    {
        fileBuffer += "    a->";
        visit(attribute->getName());
        fileBuffer += " = ";
        visit(attribute->getName());
        fileBuffer += "_;";
    }
    fileBuffer += "\n    return a;\n}\n\n";

    // ---------        generate the methods        -------- //
    for (auto const& method : node->getMethods())
    {
        visit(method, node->getName()->getValue());
    }
}

/**
 * AttributeAccess:
 */
void
Transpiler::visit(ast::AttributeAccess* node)
{
    std::string udtName = node->getUDT()->getValue();
    fileBuffer += "_" + udtName + "_->";
    visit(node->getAttribute());
}

/**
 * MethodAccess:
 */
void
Transpiler::visit(ast::MethodAccess* node)
{
    visit(node->getFunctionCall()->getName());

    fileBuffer += "(";

    visit(node->getUDT());

    // fileBuffer += ",";

    for (int i = 0; i < node->getFunctionCall()->getArguments().size(); i++)
    {
        // no comma following the last argument
        if (i != node->getFunctionCall()->getArguments().size() - 1)
        {
            fileBuffer += ", ";
        }
        visit(node->getFunctionCall()->getArguments().at(i));
    }

    fileBuffer += ")";
}

/**
 * UserDefinedType:
 */
void
Transpiler::visit(ast::UserDefinedType* node)
{
    fileBuffer += " = construct_";

    visit(node->getName());

    fileBuffer += "(";

    int i = 0;
    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute->getValue());
        if (i != node->getAttributes().size() - 1)
            fileBuffer += ",";
    }
    fileBuffer += ");";
}

/**
 * NewUDTDefinition:
 */
void
Transpiler::visit(ast::NewUDTDefinition* node)
{
    visit(node->getVariable()->getType());

    fileBuffer += "* ";

    visit(node->getVariable()->getName());
    visit(node->getExpression());
}
