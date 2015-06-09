# curses
This repository shows some possibilities of curses.

## Compilation on Mac OS X
ncurses should be preinstalled on Mac OS X. Compile with -lncurses.

    $ clang++ -o Snake -lncurses snake.cpp
    $ g++ -o Snake -lncurses snake.cpp

## Compilation on Windows
### Install pdcurses
MinGW must already be installed.

    $ mingw-get install mingw32-libpdcurses

### Compilation with g++

    $ g++ snake.cpp -o Snake.exe -lpdcurses -std=c++11
