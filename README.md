# The Sailfishc Compiler

![sailfish jumping](https://media.giphy.com/media/l0fDZGf4DpQ5i/giphy.gif)

The Sailfish programming language is an experimental project I am undertaking during my Junior Spring as part of my compiler independent study under Professor Dale Skrien at Colby College. This is the initial version, a dirty version that will be used for bootstrapping.

***

## Installation

For now, you can either build from source:
1. clone this repo
2. `cd sailfishc`
3. `make`
4. `./sailfishc`

Or use the install script [here](https://github.com/sailfish-lang/sailfish-lang-install-script.git).

## Development

To run tests, you'll need [GoogleTest](https://github.com/google/googletest) and [CMake](https://cmake.org). There is a nice tutorial on how to get GoogleTest up and running on Ubuntu [here](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/). Also check [this](https://github.com/AppImage/AppImageKit/issues/571#issuecomment-349471627) out if gtest isn't working In order to run the test suite, follow these steps:
```
cd test
cmake CMakeLists.txt
make
./sailfishc_tests
```

***

## Sailfish Wiki
I will be regularly updating the wiki with docs, grammar, code snippets, sailfish philosophy, version notes, etc. So travel on over [there](https://github.com/robertDurst/sailfish/wiki) to learn more.
