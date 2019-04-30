# The Sailfishc Compiler

![sailfish jumping](https://media.giphy.com/media/l0fDZGf4DpQ5i/giphy.gif)

The Sailfish programming language is an experimental project I am undertaking during my Junior Spring as part of my compiler independent study under Professor Dale Skrien at Colby College. This is by no means something that should ever be used for anything that touches production. This is a toy language. Something that will often break, has no garauntees of being backwards compatible, and at any time may go unmaintained. That being said, if you're also a student or programming language enthusiast and want to work on something fun/cool, open an issue, or fork this repo. Let's have some fun!

***

## Installation

For now, you can either build from source:
1. clone this repo
2. `cd sailfishc`
3. `make`
4. `./sailfishc`

Or use the install script [here](https://github.com/sailfish-lang/sailfish-lang-install-script.git).

***

## Language Limitations

There exist a number of known issues with the Sailfishc compiler. Some of these issues are limitations of the languages, others are simply results of incorrect implementation. 

1. mutual recursion
2. importing udt's within udt's impossible
3. name collisions if udt's/script have similarly named attributes/methods/variables
4. No udt constructors
5. no udt static methods
6. double dispatch does not transpile properly
7. functions are not first class
8. declaring more than one attribute accessor within function call params does not transpile correctly


***

## Sailfish Wiki
I will be regularly updating the wiki with docs, grammar, code snippets, sailfish philosophy, version notes, etc. So travel on over [there](https://github.com/robertDurst/sailfish/wiki) to learn more.
