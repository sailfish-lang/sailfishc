# The Sailfish Programming Language

![sailfish jumping](https://media.giphy.com/media/l0fDZGf4DpQ5i/giphy.gif)

The Sailfish programming language is an experimental project I am undertaking during my Junior Spring as part of my compiler independent study under Professor Dale Skrien at Colby College. 

***

## Overview
The intention is for this language to be simple and usable by the end of the semester. Since I am also writing a compiler in another class this semester as part of a normal class, this compiler needs to be jazzed up to qualify for four credits under an independent study. After some debate between whether to target the Ethereum Virtual Machine and write a smart contract language, or target something else with the goal of a self compiling language, I chose the latter. Ultimately this came down to three reasons:

1. The EVM is in a state of flux with WASM coming into ETH 2.0.
2. My ultimate goal is to pull up a terminal and bang out some code during my final presentation. Having to preface this with a WTF is a blockchain and a smart contract conversation would be cumbersome.
3. It'll be cool to say, yeah not only did I just write some sailfish in this demo, the entire compiler is also written in sailfish...

![mic drop](https://media.giphy.com/media/15BuyagtKucHm/giphy.gif)

***

## Basics

**Source language:** Sailfish <br>
**Implementation language:** C++ (original version) <br>
**Target(s):** TBD <br>
**Compiled or Interpreted:** Compiled <br>
**Functional, Procedural, or Obect Oriented:** Hybrid <br>
**Model Languages:** Python with some fun stuff from Rust, Haskell and Go

***

## Code Snippets

**Hello World**
```
// start is like a main function in C
start {
    display ("Hello World")
}
```

**Example of Multiple Files**
*foo.fish*
```
// notes to export since functions within a file are unexported by default
exp fun foo {
    display ("Hello from another file")
}
```

*sail.fish*
```
// denotes find in relative file path
include ./foo

start {
    // utilizing the rust syntax here
    foo::foo()
}
```

***

## Development

To run tests, you'll need [GoogleTest](https://github.com/google/googletest) and [CMake](https://cmake.org). There is a nice tutorial on how to get GoogleTest up and running on Ubuntu [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/). In order to run the test suite, follow these steps:
```
cd test
cmake CMakeLists.txt
make
./sailfishc_tests
```

***

## Grammar (WIP)
*WIP since this probably won't be v1 finalized until after frontend of compiler is complete.*

Start  := Source <br>
Source := SourcePart*  <br>

SourcePart := ExportDefinition | 
              FunctionDefinition |
              GeneralDecleration |
              UserDefinedTypeDefinition |
              InitialExecutionBody  <br>

ExportDefinition := 'exp' Exportable  <br>
Exportable := FunctionDefinition | GeneralDecleration  <br>

FunctionDefinition := FunctionName FunctionInput FunctionOutput FunctionBody  <br>
FunctionName := 'fun' Identifier  <br>
FunctionInput := '<-' InputList  <br>
FunctionOutput := '->' OutputList  <br>
FunctionBody := Block  <br>

InputList := Input (',' Input)*  <br>
Input := Variable  <br>
OutputList := Output (',' Output)*  <br>
Output := Typename  <br>

GeneralDecleration := 'dec' GeneralDefinition  <br>
GeneralDefinition := ListDefinition | DictionaryDefinition | NewVariableDefinition  <br>
ListDefinition := 'list' Identifier '=' Expression  <br>
DictionaryDefinition := ''dictionary' Identifier  '=' Expression<br>
NewVariableDefinition := Variable '=' Expression  <br>
Variable := TypeName Identifier  <br>

UserDefinedTypeDefinition := 'Cat' UserDefinedTypeAttributes [UserDefinedTypeMethods]  <br>
UserDefinedTypeAttributes := Identifier '{' Variable* '}'  <br>
UserDefinedTypeMethods := 'Cfn' Identifier '{' FunctionDefinition* '}'  <br>

InitialExecutionBody := 'start' 'Block  <br>

Block := '{' Statement* '}'  <br>

Statement := IfStatement |  Block | SimpleStatement  | 'continue' | 'break' | ReturnStatement  <br>
IfStatement := 'if' Expression Block 'else' Block  <br>
SimpleStatement := GeneralDefinition | ExpressionStatement  <br>
ExpressionStatement := Expression  <br>
ReturnStatement := 'return' Expression  <br>

Expression := NewExpression |
              IndexAccess |
              MemberAccess |
              FunctionCall |
              '!' Expression |
               '**' Expression |
               ('*' | '/' | '%') Expression |
               ('+' | '-') Expression|
               ('>' | '<' | '>=' | '<=') Expression |
               ('==' | '!=') Expression|
               '&&' Expression |
               '||' Expression |
               '=' Expression | 
              PrimaryExpression  <br>

NewExpression := New
New := ListLiteral | DictionaryLiteral

IndexAccess := '[' IntegerLiteral ']'  <br>
MemberAccess := '.' Identifier  <br>

FunctionCall := '(' [Expression] (',' Expression)*')'  <br>

PrimaryExpression := Primary  <br>

Primary := BooleanLiteral |
           NumberLiteral |
           StringLiteral |
           Identifier  <br>

BooleanLiteral := 'true' | 'false'  <br>
DictionaryLiteral := '{' [DictionaryItem (',' DictionaryItem)] '}'  <br>
DictionaryItem := Identifier ':' Identifier  <br>
ListLiteral := '[' [ListItem (',' ListItem)] ']'  <br>
ListItem := Identifier  <br>
NumberLiteral := Integer | Decimal  <br>
Integer := [0-9]+  <br>
Decimal  := [0-9]+.[0-9]+  <br>
StringLiteral := "' ([^"\r\n\\] | '\\' .)* '"'  <br>
Identifier := [a-zA-Z] [a-zA-Z_0-9]*  <br>
Typename := 'bool' | 'str' | 'int' | 'flt' | 'void'  <br>

## Notes

* Cannot assign unnamed lists or dictionaries to variables (commit: `f0e6239f6999bcc6dd9cbf2e90689801b0fb3c8d`)
* Lists and dictionaries must be initialized when declared

## Wishlist

* Standard documentation tools
* Standard library
* Standard formatting like Go