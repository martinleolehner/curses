/*
* A simple test program using curses.
* Copyright (C) 2015  Martin Lehner
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef _WIN32
	#include <curses.h>
#elif __APPLE__
	#include <ncurses.h>
#elif __linux__
	#include <ncurses.h>
#endif

int main()
{
	initscr();  // Initialize curses
	cbreak();  //  Disable line buffering to read characters immediately
	keypad(stdscr, TRUE);  // Extended keys (arrows etc.)
	noecho();  // No echo of the user input

	int ch;  // Character

	mvprintw(0, 0, "Press the right arrow key to continue.");  // Print at 0/0 (first line)

	timeout(-1);  // Wait indefinitely for input
	do
	{
		ch = getch();  // Get character
	}
	while(ch != KEY_RIGHT);

	// Clear first line
	move(0, 0);  // Move cursor to 0/0
	clrtoeol();  // Clear the line where the cursor is

	mvprintw(0, 0, "Press the left arrow key in the next 5 seconds.");

	timeout(5000);  // Wait maximum 5 seconds for input
	ch = getch();  // Get character

	// Clear first line
	move(0, 0);
	clrtoeol();

	// Check the character
	if(ch == ERR)  // No key was hit
	{
		mvprintw(0, 0, "You didn't hit a key.");
	}
	else if(ch == KEY_LEFT)
	{
		mvprintw(0, 0, "Congratulations, you hit the left arrow key.");
	}
	else
	{
		mvprintw(0, 0, "You hit another key.");
	}

	mvprintw(2, 0, "Hit a key to quit."); // Caution, first parameter is y, second is x

	timeout(-1); // Wait indefinitely for input
	ch = getch();  // Wait for the user to hit a key

	endwin();  // Quit curses mode

	return 0;
}
