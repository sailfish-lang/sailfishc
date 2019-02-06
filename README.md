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
**Target(s):** TBD
**Compiled or Interpreted:** Compiled <br>
**Functional, Procedural, or Obect Oriented:** Hybrid <br>
**Model Languages:** Python with some fun stuff from Rust, Haskell and Go

***

## Code Snippets

**Hello World**
```
// start is like a main function in C
start {
    display ("Hello World");foo();
}
```

**Fizz Buzz**
```
start {
    /* 
        starts with 1, ends with 100; both sides are inclusive
        since we are using the common mathematical bracketing
        syntax. Thus (1,100) is exclusize on both sides
    */ 
    loop i := [1,100] {
        if   (i % 3 == 0)  { display ("Fizz") }
        elif (i % 5 == 0)  { display ("Buzz") }
        elif (i % 15 == 0) { display ("FizzBuzz") }
        else               { display (i) }
    }
}
```

**Example of Multiple Files**
*foo.fish*
```
// notes to export since functions within a file are unexported by default
exp fun foo {
    display ("Hello from another file");
}
```

*sail.fish*
```
// denotes find in relative file path
include ./foo

start {
    // utilizing the rust syntax here
    foo::foo();
}
```

***

## Wishlist

* Standard documentation tools
* Standard library
* Standard formatting like Go