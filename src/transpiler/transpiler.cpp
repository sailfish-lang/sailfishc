/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "transpiler.h"

std::string
Transpiler::getTabs()
{
    std::string s = "";
    for (int i = 0; i < currentTabs; i++)
        s += "    ";
    return s;
}

int
Transpiler::occurences(const std::string& s, const std::string& target)
{
    int occurrences = 0;
    std::string::size_type pos = 0;
    while ((pos = s.find(target, pos)) != std::string::npos)
    {
        ++occurrences;
        pos += target.length();
    }

    return occurrences;
}

std::string
Transpiler::extractChainAAType(const std::string& x)
{
    auto baseType = x.substr(0, x.find_first_of("->"));
    auto chainedType = baseType;

    for (int i = 0; i < occurences(x, baseType) - 1; i++)
    {
        chainedType += "->" + baseType;
    }

    return chainedType;
}

std::string
Transpiler::builtinTypesTranslator(const std::string& type)
{
    if (type == "int")
        return "int";

    if (type == "flt")
        return "float";

    if (type == "str")
        return "char*";

    if (type == "bool")
        return "int";

    if (type == "[int]")
        return "int*";

    if (type == "[flt]")
        return "float*";

    if (type == "[str]")
        return "char**";

    if (type == "[bool]")
        return "int*";

    if (type == "own")
        return "this";

    else
        return type;
}

Transpiler::Transpiler()
{
    buffer = "";
    currentTabs = 0;
    decName = "";
    decType = "";
}

std::string
Transpiler::getBuffer()
{
    return buffer;
}

void
Transpiler::clearOpenBeginWriting()
{
    output.clear();
    output.open("out.c");
}

void
Transpiler::writeStandardLibrary()
{
    output << OUTPUT_HEADER << getStdLibC();
}

void
Transpiler::write(bool successfulAnalysis)
{
    if (successfulAnalysis)
    {
        clearOpenBeginWriting();
        writeStandardLibrary();
        output << buffer;
        output.close();
    }
    else
        throw "Cannot compile. Please fix semantic errors as described "
              "above.\n";
}

void
Transpiler::append(const std::string& buf)
{
    buffer += buf;
}

void
Transpiler::incrementTabs()
{
    ++currentTabs;
}

void
Transpiler::decrementTabs()
{
    --currentTabs;
}

void
Transpiler::setDecName(const std::string& dn)
{
    decName = dn;
}

void
Transpiler::setDecType(const std::string& dt)
{
    decType = dt;
}

std::string
Transpiler::getDecName()
{
    return decName;
}

std::string
Transpiler::getDecType()
{
    return decType;
}

void
Transpiler::pushMethod(const std::string& type, const std::string& name)
{
    methodAccessStack.push_back(std::make_tuple(type, name));
}

void
Transpiler::popMethod()
{
    methodAccessStack.pop_back();
}

void
Transpiler::genUDTHeader(const std::string& udtname)
{
    buffer += "\n//___________BEGIN_" + udtname +
              "_UDT_DEFINITION__________/_//\n\n" + "struct " + udtname +
              "\n{\n";
}

void
Transpiler::genUDTFooter(const std::string& udtname)
{
    buffer +=
        "//___________END_" + udtname + "_UDT_DEFINITION__________/_//\n\n";
}

void
Transpiler::genLeftCurley()
{
    buffer += "{";
}

void
Transpiler::genRightCurley()
{
    buffer += "}";
}

void
Transpiler::genLeftParen()
{
    buffer += "(";
}

void
Transpiler::genRightParen()
{
    buffer += ")";
}

void
Transpiler::genComma()
{
    buffer += ",";
}

void
Transpiler::genSemiColonAndNewline()
{
    buffer += ";\n";
}

void
Transpiler::genTypeAndName(const std::string& type, const std::string& name)
{
    buffer += type + " " + name;
}

void
Transpiler::genTypeAndNameNewLine(const std::string& type,
                                  const std::string& name)
{
    buffer += getTabs() + type + " " + name;
    genSemiColonAndNewline();
}

void
Transpiler::genFunctionEnd()
{
    buffer += "\n}\n\n";
}

void
Transpiler::genMainHeader()
{
    buffer += "int\nmain()\n{";
}

void
Transpiler::genMainFooter()
{
    buffer += "\n    return 1;\n}";
}

void
Transpiler::genStatementHeader()
{
    buffer += "\n" + getTabs();
}

void
Transpiler::genStatementFooter()
{
    genSemiColonAndNewline();
}

void
Transpiler::genIfHeader()
{
    buffer += "if";
}

void
Transpiler::genElseHeader()
{
    buffer += "\n " + getTabs() + " else if ";
}

void
Transpiler::genBranchHeader()
{
    buffer += "\n" + getTabs();
    genLeftCurley();
}

void
Transpiler::genBranchFooter()
{
    buffer += "\n" + getTabs();
    genRightCurley();
}

void
Transpiler::genReturn()
{
    buffer += "return ";
}

void
Transpiler::genOperator(const std::string& op)
{
    buffer += " " + op + " ";
}

void
Transpiler::genAttributeAccess(bool nextIsTripleDot, bool udtNameIsUDT,
                               const std::string& udtname,
                               const std::string& attribute)
{
    if (!nextIsTripleDot && methodAccessStack.size() == 0)
    {
        if (!udtNameIsUDT) // bandaid solution
        {
            if (udtname != "own")
                buffer += udtname;
            else
                buffer += "this";
        }
        buffer += "->" + builtinTypesTranslator(attribute);
    }
    else
    {
        if (udtNameIsUDT)
        {
            auto top = attributeAccessStack.at(attributeAccessStack.size() - 1);
            attributeAccessStack.push_back(std::make_tuple(
                std::get<0>(top) + "->" + extractChainAAType(std::get<1>(top)),
                attribute));
        }
        else
            attributeAccessStack.push_back(std::make_tuple(udtname, attribute));
    }
}

void
Transpiler::genUDTDecInit(const std::string& udtname)
{
    buffer +=
        "(struct " + udtname + "*)malloc(sizeof(struct " + udtname + "));\n";
}

void
Transpiler::genUDTDecItem(const std::string& attributeName)
{
    buffer += getTabs() + decName + "->" + attributeName + " = ";
}

void
Transpiler::genLiteral(const std::string& literal)
{
    buffer += literal;
}

void
Transpiler::genListInit(const std::string& type, const std::string size)
{
    buffer += "(" + builtinTypesTranslator(type) + "*)malloc(sizeof(" +
              builtinTypesTranslator(type) + ") * " + size + ");\n";
}

void
Transpiler::genListItem(const std::string& index, const std::string& value)
{
    buffer += getTabs() + decName + "[" + index + "] = " + value + ";\n";
}

void
Transpiler::genPrimary(bool isNotBeforeMemberAccessor, bool isNotUdtName,
                       const std::string& type)
{
    if (methodAccessStack.size() == 0 ||
        (methodAccessStack.size() != 0 && type != "void"))
    {
        if (isNotBeforeMemberAccessor)
        {
            if (isNotUdtName)
            {
                buffer += "struct " + type + "*";
            }
            else
            {
                buffer += builtinTypesTranslator(type);
            }
        }
        else
        {
            if (attributeAccessStack.size() != 0)
            {
                buffer += std::get<0>(
                    attributeAccessStack.at(attributeAccessStack.size() - 1));
                attributeAccessStack.pop_back();
            }
        }
    }
}

void
Transpiler::genFinalFunctionCallArg(bool noVoids, bool isUdt)
{
    if (noVoids)
    {
        if (attributeAccessStack.size() != 0 && isUdt)
        {
            buffer += std::get<1>(
                attributeAccessStack.at(attributeAccessStack.size() - 1));
            attributeAccessStack.pop_back();
        }
        else if (attributeAccessStack.size() != 0 && !isUdt)
        {
            buffer +=
                std::get<0>(
                    attributeAccessStack.at(attributeAccessStack.size() - 1)) +
                "->" +
                std::get<1>(
                    attributeAccessStack.at(attributeAccessStack.size() - 1));
            attributeAccessStack.pop_back();
        }
        else if (methodAccessStack.size() != 0)
            buffer +=
                std::get<0>(methodAccessStack.at(methodAccessStack.size() - 1));
    }
    else
    {
        if (attributeAccessStack.size() != 0 && isUdt)
        {
            buffer += ", " + std::get<1>(attributeAccessStack.at(
                                 attributeAccessStack.size() - 1));
            attributeAccessStack.pop_back();
        }
        else if (attributeAccessStack.size() != 0 && !isUdt)
        {
            buffer +=
                ", " +
                std::get<0>(
                    attributeAccessStack.at(attributeAccessStack.size() - 1)) +
                "->" +
                std::get<1>(
                    attributeAccessStack.at(attributeAccessStack.size() - 1));
            attributeAccessStack.pop_back();
        }
        else if (methodAccessStack.size() != 0)
            buffer +=
                ", " +
                std::get<0>(methodAccessStack.at(methodAccessStack.size() - 1));
    }
}