
# MENACE

MENACE: the Machine Educable Noughts And Crosses Engine

MENACE is a command line application for playing noughts and crosses, inspired by Donald Michie's original programme, but does not use the same algorithm. It instead uses an implementation of the minimax algorithm with alpha-beta pruning.

The application was developed for Microsoft Windows only, and cannot be compiled on another system as it uses Windows specific terminal manipulation.

## Building

Note the following build commands require ```./bin/``` and ```./obj/``` directories to be created before executing either of the commands.

The programme can be built with the provided Makefile and nmake, with the following command:

```shell
nmake all
```

The tests can be built with the provided Makefile and nmake, with the following command:

```shell
nmake test
```

Further, the programme can be built using MSVC on the command line with the following command:

```shell
cl /EHsc /std:c++17 /Fo:./obj/ /Fe:./bin/menace /I/lib/boost/ /I./src/ ./src/*.cpp
```

The tests can be built on the command line with:

```shell
cl /EHsc /std:c++17 /Fo:./obj/ /Fe:./bin/test /I/lib/boost/ /I./src/ /I./test/ @./test/test_files.txt
```

In all instances, you may have to change the specification of the Boost library, given by ```/I/lib/boost/```, depending on where it is stored on your system.

## Dependencies

This project uses the Boost C++ libraries, version 1.71.0, which can be found at <https://www.boost.org/>.

## Tests

The source for testing this programme can be found in ```./test/``` and the tests can be run, once compiled, with ```./bin/test```.

Tests can take some time to run, as it involves MENACE playing games against itself. Runtime can be decreased by reducing the upperlimit of the for loop on lines 72 and 95 of ```./tests/test-player.hpp```.
