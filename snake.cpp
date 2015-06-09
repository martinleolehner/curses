/*
* A simple snake game using curses.
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
#endif
#include <list>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <chrono>
#include <unistd.h>

class Point
{
private:
	int x;
	int y;
public:
	Point(int valueX, int valueY)
	{
		x = valueX;
		y = valueY;
	}
	int getX(){ return x; }
	int getY(){ return y; }
	bool operator==(Point p)
	{
		bool result = false;
		if(this->x == p.getX() && this->y == p.getY()) result = true;
		return result;
	}
};

// Functions
void initialize();
void clearLine();
void createApple();
long getCurrentMSecs();
void startGame();

long sysTime = 0;

// Variables and Objects
WINDOW *myWindow;
int ch = ERR;
int ch2 = ERR;
int startY = 2;
int startX = 5;
int height = 10;
int width = 10;
bool quit = false;
bool quitAndExit = false;
bool paused = false;
std::list<Point> snake;
std::list<Point>::iterator it;
Point apple = Point(0,0);
bool appleOnSnake = false;
int direction = 2;  // right
bool biteHerSelf = false;
int points = 0;
bool gameOver = false;

// Main
int main()
{
	// Initialize ncurses, window and colors
	initialize();

	// Start the game
	do
	{
		startGame();
		if(quitAndExit) break;

		timeout(-1);
		do
		{
			ch = getch();
		}
		while(ch != 'q' && ch != ' ');

		if(ch == 'q') break;
	}
	while(ch == ' ');

	// Quit game
	endwin();
	return 0;
}

void initialize()
{
	// Initialize ncurses
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	refresh();

	// Seed
	srand(time(0));

	// Get window dimensions
	height = LINES - startY - 1;
	width = COLS - 2*startX;

	// Window to small?
	if(width < 20 || height < 20)
	{
		move(0,0);
		clrtoeol();
		mvprintw(0, startX, "Your window ist too small. Resize the window if possible and try again.");
		quit = true;
	}

	// Create the window
	myWindow = newwin(height, width, startY, startX);
	box(myWindow, 0, 0);
	wrefresh(myWindow);

	// Create color pairs
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
}

void clearLine()
{
	move(0,0);
	clrtoeol();
}

void createApple()
{
	// Create the apple
	do
	{
		appleOnSnake = false;
		apple = Point(rand()%(width-2)+1,rand()%(height-2)+1);
		for(it = snake.begin(); it != snake.end(); ++it)
		{
			if(apple == *it) appleOnSnake = true;
		}
	} while(appleOnSnake);
}

long getCurrentMSecs()
{
	long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return milliseconds;
}

void startGame()
{
	// Initialize
	direction = 2;  // right
	biteHerSelf = false;
	points = 0;
	gameOver = false;
	quit = false;
	quitAndExit = false;
	paused = false;
	appleOnSnake = false;

	// Create the snake
	snake.clear();
	for(int i=0; i<5; i++)
	{
		snake.push_front(Point(5+i, height/2));
	}

	// Create the apple
	createApple();

	// Clear the window
	wclear(myWindow);
	box(myWindow, 0, 0);
	wrefresh(myWindow);

	// Wait for the user to start the game
	clearLine();
	mvprintw(0, startX, "Ready? Press 'q' to exit, 'p' to pause, space to start.");
	timeout(-1);
	do
	{
		ch = getch();
		if(ch == 'q')
		{
			quit = true;
			break;
		}
	}
	while(ch != ' ');

	clearLine();
	mvprintw(0, startX, "Good Luck!");

	// Main program procedure
	while(!gameOver && !quitAndExit)
	{
		// Read a character or continue when time runs out
		int delay = 90 - points;
		if(points >= 70) delay = 20;

		// Get the current time when starting to read a k
		long startTime = getCurrentMSecs();

		timeout(delay);
		ch = getch();

		if(ch == ERR) ch = ch2;
		ch2 = ERR;

		timeout(5);
		int tempCh = ERR;
		while(ch != ERR && getCurrentMSecs() - startTime < delay)
		{
			tempCh = getch();
			if(tempCh != ERR) ch2 = tempCh;
		}

		if(ch == 'p')
		{
			paused = !paused;
			if(!paused)
			{
				clearLine();
			}
		}
		else if(ch == 'q') quit = true;

		if(quit)
		{
			if(ch == 'y')
			{
				quitAndExit = true;
				break;
			}
			else if(ch == 'n')
			{
				paused = true;
				quit = false;
				continue;
			}
			else
			{
				clearLine();
				mvprintw(0,startX, "Do you really want to quit? Press 'y' to quit, 'n' to continue.");
				continue;
			}
		}
		else if(paused){
			clearLine();
			mvprintw(0,startX, "Paused, press 'p' to continue.");
			continue;
		}

		// Check if a key was pressed
		if(ch != ERR)
		{
			// Get the pressed key
			switch(ch)
			{
				case(KEY_UP):
					if(direction != 3) direction = 1;
				break;
				case(KEY_RIGHT):
					if(direction != 4) direction = 2;
				break;
				case(KEY_DOWN):
					if(direction != 1) direction = 3;
				break;
				case(KEY_LEFT):
					if(direction != 2) direction = 4;
				break;
			}
		}

		// Clear the window before redrawing everything
		wclear(myWindow);

		// Check direction
		if(direction >= 1 && direction <= 4)
		{
			// Move the head of the snake
			int x = snake.begin()->getX();
			int y = snake.begin()->getY();
			if(direction == 1) y--;
			else if(direction == 2) x++;
			else if(direction == 3) y++;
			else if(direction == 4) x--;

			if(*snake.begin() == apple)
			{
				// Snake eats the apple and becomes one part longer
				++points;
				createApple();
			}
			else
			{
				// Snake didn't eat the apple and stays the same length
				snake.pop_back();
			}

			// Check collision with snake
			for(it = snake.begin(); it!= snake.end(); ++it)
			{
				if(it->getX() == x && it->getY() == y) biteHerSelf = true;
			}

			// Move the head forward
			snake.push_front(Point(x,y));

			// Check collision with walls
			if(x >= width-1 || x <= 0 || y >= height-1 || y <= 0 || biteHerSelf)
			{
				// Game Over
				gameOver = true;
			}
			else
			{
				// Draw the apple
				mvwaddch(myWindow, apple.getY(), apple.getX(), 'O' | COLOR_PAIR(1));

				// Draw the snake
				for(it = snake.begin(); it != snake.end(); ++it)
				{
					mvwaddch(myWindow, it->getY(), it->getX(), 'O' | COLOR_PAIR(3));
				}

				// Redraw the window and its border
				box(myWindow, 0, 0);
				wrefresh(myWindow);

				// Show points at top right
				int number = points;
				int numberDigits = 0;
				do
				{
					numberDigits++;
					number /= 10;
				}while(number != 0);
				std::ostringstream oss;
				oss << points;
				mvprintw(0, COLS-1-numberDigits-startX, oss.str().c_str());
			}
		}
		else
		{
			clearLine();
			attron(COLOR_PAIR(1));
			mvprintw(0,startX, "Uuuups, a strange error occured...");
			attroff(COLOR_PAIR(1));
		}
	}

	if(gameOver)
	{
		clearLine();
		std::stringstream gameOverString;
		std::ostringstream ss;
		ss << points;
		gameOverString << "GAME OVER. Your Points: " << ss.str() << " Press 'q' to exit, space to restart.";
		mvprintw(0,startX, gameOverString.str().c_str());
	}
}
