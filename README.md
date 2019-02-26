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

## Development

To run tests, you'll need [GoogleTest](https://github.com/google/googletest) and [CMake](https://cmake.org). There is a nice tutorial on how to get GoogleTest up and running on Ubuntu [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/). Make sure it is in accessible directory. In order to run the test suite, follow these steps:
```
cd test
cmake CMakeLists.txt
make
./sailfishc_tests
```

***

## Sailfish Wiki
I will be regularly updating the wiki with docs, grammar, code snippets, sailfish philosophy, version notes, etc. So travel on over [there](https://github.com/robertDurst/sailfish/wiki) to learn more.
