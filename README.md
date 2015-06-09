# curses
This repository shows some possibilities of curses.

## Compilation on Mac OS X
ncurses should be preinstalled on Mac OS X.

    $ clang++ -std=c++11 -stdlib=libc++ -lncurses -o Snake snake.cpp

## Compilation on Linux
### Install ncurses
    $ sudo apt-get install libncurses5-dev

### Compilation with g++

    $ g++ -std=c++0x -lncurses -o Snake snake.cpp

## Compilation on Windows
### Install pdcurses
MinGW must already be installed.

    $ mingw-get install mingw32-libpdcurses

### Compilation with g++

    $ g++ snake.cpp -o Snake.exe -lpdcurses -std=c++11
