BlackJack
=========

C.Coleman-Smith (chris.colemansmith@gmail.com)

A simple implementation of a popular card game, aims to help you learn to count cards. 

## Requires:

* CMAKE [1]
* A C compiler
* GSL [2]
* Check [3] - for unit tests

## Building, Installing and Testing

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
[2]: http://www.gnu.org/software/gsl/
[3]: http://check.sourceforge.net/
