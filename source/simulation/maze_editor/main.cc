#include "maze.hh"
#include <ncurses/curses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

micromouse::maze maze;
micromouse::maze::position_type position;
std::string opened_filename;
int max_y, max_x, beg_y, beg_x;

void initialize()
{
	initscr();
	resizeterm(80, 80);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	maze.width(16);
	position = { 0, 0 };
	maze.start({maze.width() - 1, 0});
	maze.goal({maze.width() - 1, maze.width() - 1});
}

void update_origins()
{
	getbegyx(stdscr, beg_y, beg_x);
	getmaxyx(stdscr, max_y, max_x);
}

void draw_menu()
{
	mvaddstr(beg_y, beg_x,
	"F1: NEW    F2: OPEN    F3: SAVE    F4: SAVE AS    F5: EXIT");
	mvaddstr(beg_y + 1, beg_x,
	"WASD: MOVE    IJKL: TOGGLE WALL    O: SET START    P: SET GOAL");
}

void draw_cell(const micromouse::maze::position_type& p)
{
	move(beg_y + 3 + p.first * 2, beg_x + p.second * 4);
	if (maze[p].test(micromouse::WALL_TOP)) addstr("+---+");
	else addstr("+   +");
	move(beg_y + 4 + p.first * 2, beg_x + p.second * 4);
	if (maze[p].test(micromouse::WALL_LEFT)) addstr("|   ");
	else addstr("    ");
	if (maze[p].test(micromouse::WALL_RIGHT)) addch('|');
	else addch(' ');
	move(beg_y + 5 + p.first * 2, beg_x + p.second * 4);
	if (maze[p].test(micromouse::WALL_BOTTOM)) addstr("+---+");
	else addstr("+   +");
}

void draw_maze()
{
	micromouse::maze::position_type p;

	if (maze.width() == 0) return;
	for (p.first = 0; p.first < maze.width(); ++p.first) {
		for (p.second = 0; p.second < maze.width(); ++p.second) {
			draw_cell(p);
		}
	}
}

void erase_maze()
{
	for (int y = beg_y + 3; y < beg_y + 4 + maze.width() * 2; ++y) {
		move(y, beg_x);
		clrtoeol();
	}
	refresh();
}

void draw_position(const micromouse::maze::position_type& p, char ch)
{
	mvaddch(beg_y + 4 + p.first * 2,
		beg_x + 2 + p.second * 4, ch);
}

void input_exit()
{
	endwin();
	exit(0);
}

std::string prompt(const std::string& msg)
{
	char buffer[80];

	mvaddstr(beg_y + 2, beg_x, msg.c_str());
	nocbreak();
	echo();
	getnstr(buffer, sizeof(buffer)-1);
	move(beg_y + 2, beg_x);
	clrtoeol();
	cbreak();
	noecho();
	refresh();
	return std::string{buffer};
}

void input_new()
{
	std::size_t width;

	width = std::stoi(prompt("Enter width: "));
	if (width == 0) return;
	erase_maze();
	maze.width(width);
	maze.clear();
	position = { 0, 0 };
	maze.start({maze.width() - 1, 0});
	maze.goal({maze.width() - 1, maze.width() - 1});
}

void input_open()
{
	std::string filename;
	std::ifstream ifs;

	filename = prompt("Enter filename: ");
	ifs.open(filename);
	if (ifs.is_open()) {
		erase_maze();
		ifs >> maze;
		position.first = 0;
		position.second = 0;
		opened_filename = filename;
	}
}

void input_save()
{
	std::ofstream ofs;

	ofs.open(opened_filename);
	if (ofs.is_open()) {
		ofs << maze;
		ofs.close();
	}
}

void input_save_as()
{
	std::string filename;

	filename = prompt("Enter filename: ");
	opened_filename = filename;
	input_save();
}

void input()
{
	int ch;

	ch = getch();
	switch (ch) {
	case KEY_F(5):
		input_exit();
		break;
	case KEY_F(1):
		input_new();
		break;
	case KEY_F(2):
		input_open();
		break;
	case KEY_F(3):
		if (opened_filename.empty()) input_save_as();
		else input_save();
		break;
	case KEY_F(4):
		input_save_as();
		break;
	case 'w':
		if (position.first > 0) --position.first;
		break;
	case 'a':
		if (position.second > 0) --position.second;
		break;
	case 's':
		if (position.first < maze.width() - 1) ++position.first;
		break;
	case 'd':
		if (position.second < maze.width() - 1) ++position.second;
		break;
	case 'i':
		maze[position].flip(micromouse::WALL_TOP);
		if (position.first > 0) {
			maze[{position.first - 1, position.second}].flip(
						micromouse::WALL_BOTTOM);
		}
		break;
	case 'j':
		maze[position].flip(micromouse::WALL_LEFT);
		if (position.second > 0) {
			maze[{position.first, position.second - 1}].flip(
						micromouse::WALL_RIGHT);
		}
		break;
	case 'k':
		maze[position].flip(micromouse::WALL_BOTTOM);
		if (position.first < maze.width() - 1) {
			maze[{position.first + 1, position.second}].flip(
						micromouse::WALL_TOP);
		}
		break;
	case 'l':
		maze[position].flip(micromouse::WALL_RIGHT);
		if (position.second < maze.width() - 1) {
			maze[{position.first, position.second + 1}].flip(
						micromouse::WALL_LEFT);
		}
		break;
	case 'o':
		maze.start(position);
		break;
	case 'p':
		maze.goal(position);
		break;
	default: break;
	}
}

void loop()
{
	while (true) {
		update_origins();
		draw_menu();
		draw_maze();
		draw_position(position, 'X');
		draw_position(maze.start(), 'S');
		draw_position(maze.goal(), 'G');
		refresh();
		input();
	}
}

int main(int argc, char* argv[])
{
	initialize();
	loop();
	return 0;
}

