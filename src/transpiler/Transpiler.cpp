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

    if (name == "display_bool")
        return "print_bool(";

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
    std::ofstream output;
    output.clear();
    output.open(filename);

    // traverse the tree
    visit(root);

    // write out file
    output << fileBuffer;

    // close the file
    output.close();
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

    fileBuffer += "(" + udtTypeName + "* own";

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
        visit(node->getInputList().at(i));
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
    }

    fileBuffer += ")";

    visit(node->getBody());
}

/**
 * Function Call: identifier with arguments surrounded in parenthesis
 */
void
Transpiler::visit(ast::FunctionCall* node)
{
    auto name = node->getName();
    auto args = node->getArguments();

    // catch built in functions here and add the open/left paren
    // for now assume display prints exactly one variable
    auto translatedName = builtinFunctionTranslator(name->getValue());
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
    fileBuffer += node->getValue() == "void" ? "NULL" : node->getValue();
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
 * Variable: put a space between name and type and ignore voids
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
    if (udtTable->hasUDT(node->getType()))
    {
        fileBuffer += "*";
    }
}

/**
 * Return: just add a return in front of the variable
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
    auto statements = node->getStatements();
    for (int i = 0; i < statements.size(); i++)
    {
        ast::Statement* statement = statements.at(i);

        fileBuffer += "    ";

        ast::Statement::StatementType type = statement->getStatementType();

        switch (type)
        {
        case ast::Statement::IfStatement:
        {
            auto subnode = dynamic_cast<ast::IfStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::ReturnStatement:
        {
            auto subnode = dynamic_cast<ast::ReturnStatement*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BlockStatement:
        {
            auto subnode = dynamic_cast<ast::Block*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::GeneralDecleration:
        {
            auto subnode = dynamic_cast<ast::GeneralDecleration*>(statement);
            visit(subnode);
            break;
        }
        case ast::Statement::BinaryExpressionStatement:
        {
            auto subnode = dynamic_cast<ast::BinaryExpression*>(statement);
            visit(subnode);
            break;
        }
        }

        if (i != statements.size() - 1)
            fileBuffer += "\n";
    }
    fileBuffer += "\n}\n";
}

/**
 * If statement: provide structure, mostly just adding if/else keywords
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
 * UserDefinedTypeDefinition: this is a more complex mapping. Essentially we
 * create a few things. First, we create a structure, alias'd to take the name
 * of the given udt. Then, since this is not a ood language and frankly sailfish
 * is basically ood, we define a constructor method. This allows for
 * encapsulated allocation of emmeory and instantiation of attributes.
 * Attributes must be given in the correct order, something that is ensured by
 * the type checker. Then, a function for each internal method is created, such
 * that the udt type is the first param passed in.
 *
 * We use the name a____struct___generated, assuming no one will use this in
 * code.
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

        // for this edge case, we will manually extract
        if (udtTable->hasUDT(attribute->getType()->getType()))
        {
            fileBuffer += "struct _";
            fileBuffer += attribute->getType()->getType();
            fileBuffer += "_* ";
            fileBuffer += attribute->getName()->getValue();
        }
        else
            visit(attribute);

        fileBuffer += ";\n";
    }

    fileBuffer += "} ";
    visit(node->getName());
    fileBuffer += ";\n\n";

    // ---------        define a sort of constructor       -------- //
    /**
     * Looks like this:
     *      TYPENAME*
     *      construct_TYPENAME(attributes...) {
     *          // malloc a new TYPENAME
     *          // initialize attributes
     *      }
     */
    visit(node->getName());
    fileBuffer += "*\nconstruct_";
    visit(node->getName());
    fileBuffer += "(";

    int i = 0;
    for (auto const& attribute : node->getAttributes())
    {
        visit(attribute);
        // fileBuffer += "_";
        if (i != node->getAttributes().size() - 1)
            fileBuffer += ",";

        ++i;
    }
    fileBuffer += ")\n{\n    ";
    visit(node->getName());
    fileBuffer += "* a____struct___generated = (";
    visit(node->getName());
    fileBuffer += "*)malloc(sizeof(";
    visit(node->getName());
    fileBuffer += "));\n";
    for (auto const& attribute : node->getAttributes())
    {
        fileBuffer += "    a____struct___generated->";
        visit(attribute->getName());
        fileBuffer += " = ";
        visit(attribute->getName());
        fileBuffer += ";";
    }
    fileBuffer += "\n    return a____struct___generated;\n}\n\n";

    // ---------        generate the methods        -------- //
    for (auto const& method : node->getMethods())
        visit(method, node->getName()->getValue());
}

/**
 * AttributeAccess: since attributes are accessed via pointer, we add the
 * pointer and our own code gen underscores
 */
void
Transpiler::visit(ast::AttributeAccess* node)
{
    auto udtName = node->getUDT()->getValue();
    fileBuffer += udtName + "->";
    visit(node->getAttribute());
}

/**
 * AttributeMethodAccess: similar to attribute access and method access
 */
void
Transpiler::visit(ast::AttributeMethodAccess* node)
{
    visit(node->getFunctionCall()->getName());

    fileBuffer += "(";

    // visit(node->getUDT());

    // fileBuffer += ", ";

    visit(node->getAttribute());

    int numArgs = node->getFunctionCall()->getArguments().size();

    if (numArgs != 0)
    {
        fileBuffer += ", ";
    }

    for (int i = 0; i < numArgs; i++)
    {
        // no comma following the last argument
        if (i != numArgs - 1)
        {
            fileBuffer += ", ";
        }
        visit(node->getFunctionCall()->getArguments().at(i));
    }

    fileBuffer += ")";
}

/**
 * MethodAccess: similar to function call
 */
void
Transpiler::visit(ast::MethodAccess* node)
{
    visit(node->getFunctionCall()->getName());

    fileBuffer += "(";

    visit(node->getUDT());

    // fileBuffer += "_";

    int numArgs = node->getFunctionCall()->getArguments().size();

    if (numArgs != 0)
    {
        fileBuffer += ", ";
    }

    for (int i = 0; i < numArgs; i++)
    {
        // no comma following the last argument
        if (i != numArgs - 1)
        {
            fileBuffer += ", ";
        }
        visit(node->getFunctionCall()->getArguments().at(i));
    }

    fileBuffer += ")";
}

/**
 * UserDefinedType: for instantiation of a udt, we utlize the constructor method
 * from the code gen of the method above
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

        ++i;
    }
    fileBuffer += ");";
}
