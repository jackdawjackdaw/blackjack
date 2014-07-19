BlackJack
=========

C.Coleman-Smith (chris.colemansmith@gmail.com)

A game of cards

## Requires:

* CMAKE [1]
* A C compiler
* libGSL
* libCheck - for unit tests

## Building & Installing

This project uses CMAKE to generate Makefiles and run tests, it is canonical to do out of place builds using cmake. An "out of place" builds puts all the temporary files and compiler junk into a directory that is outside the source tree.

From the project root do:

    mkdir ./build
    cd ./build
    cmake ..
    make && make install

Cmake defaults to installing things in /usr/local, if you don't want that you should set invoke cmake as

    cmake -DCMAKE_INSTALL_PREFIX:PATH=/your/install/path ..

Check backed unit tests can also be built by enabling the `BUILD_TESTING` cmake option, from the build directory 

    cmake -DBUILD_TESTING=on ..

The tests can be run from the `make test` target or by directly invoking `ctest` .

[1]: http://www.cmake.org/
