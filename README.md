Simple-Jack
=========

C.Coleman-Smith (chris.colemansmith@gmail.com)

A simple implementation of BlackJack game, aims to help you learn to count cards. You start with 100 currency
units and can keep on playing until you reach your inevitable gamblers ruin, or ctrl-c. 

The card count is updated each time a card is drawn, with the following rules:

    1 point 2-6
    0 points 7-9
    -1 points 10-A
   
the count is reset each time the deck is shuffled.

Cards are represented by their value and suit, tens are printed as T not 10.

## Requires:

* CMAKE [1]
* A C compiler
* Check [2] - for unit tests

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

## Playing the game

The first argument sets the number of packs in the deck (between 1 and 8).

## ToDo

* Change all the sprintf/strcat/... calls to something safer

        "The strcat() function is easily misused in a manner which enables malicious users to arbitrarily change
        a running program's functionality through a buffer overflow attack.  (See the FSA.)"
     
* some better checking for `hand.c:score_hand()`, something like quickcheck would be ideal.
* `game.c:play_round()` is way too long for a single function
* compute the probability of going bust given the current hand and the state of the deck
* Implement some actual tests for the game mode
* Add tests for player-actions
* Add a curses interface to make things pretty
* Support for other locales 

[1]: http://www.cmake.org/
[2]: http://check.sourceforge.net/

