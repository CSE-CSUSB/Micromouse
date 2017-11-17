#include "maze.hh"
#include <ncurses/curses.h>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <cstdlib>

micromouse::maze maze;
std::vector<micromouse::maze::position_type> path;
micromouse::maze::position_type position;
std::string maze_filename;
std::pair<int ,int> window_min;

static void draw_menu()
{
	mvprintw(window_min.first, window_min.second,
		"%-15s%-15s%-15s%-15s%-15s", "F1: NEW", "F2: OPEN",
		"F3: SAVE", "F4: SAVE AS", "F5: EXIT");
	mvprintw(window_min.first + 1, window_min.second,
		"%-15s%-15s%-15s%-15s%-15s", "1: SET START", "2: SET GOAL",
		"3: VIEW PATH", "WASD: MOVE", "IJKL: EDIT WALL");
}

static void draw_cell(const micromouse::maze::position_type& p)
{
	move(window_min.first + 4 + p.first * 2,
		window_min.second + p.second * 4);
	if (maze[p].test(micromouse::WALL_TOP))
		addstr("+---+");
	else
		addstr("+   +");
	move(window_min.first + 5 + p.first * 2,
		window_min.second + p.second * 4);
	if (maze[p].test(micromouse::WALL_LEFT))
		addstr("|   ");
	else
		addstr("    ");
	if (maze[p].test(micromouse::WALL_RIGHT))
		addch('|');
	else
		addch(' ');
	move(window_min.first + 6 + p.first * 2,
		window_min.second + p.second * 4);
	if (maze[p].test(micromouse::WALL_BOTTOM))
		addstr("+---+");
	else
		addstr("+   +");
}

static void draw_maze()
{
	micromouse::maze::position_type p;

	for (p.first = 0; p.first < maze.width(); ++p.first) {
		for (p.second = 0; p.second < maze.width(); ++p.second) {
			draw_cell(p);
		}
	}
}

static void draw_marker(const micromouse::maze::position_type& p, int ch)
{
	mvaddch(window_min.first + 5 + p.first * 2,
		window_min.second + 2 + p.second * 4, ch);
}

static void draw_path()
{
	for (const auto& p  : path) {
		draw_marker(p, 'o');
	}
}

static void draw()
{
	draw_menu();
	if (maze.width() != 0) {
		draw_maze();
		if (!path.empty()) draw_path();
		draw_marker(maze.start(), 'S');
		draw_marker(maze.goal(), 'G');
		draw_marker(position, 'X');
	}
	refresh();
}

std::string prompt(const std::string& message)
{
	char buffer[80];

	mvaddstr(window_min.first + 3, window_min.second, message.c_str());
	nocbreak();
	echo();
	getnstr(buffer, sizeof(buffer) - 1);
	move(window_min.first + 3, window_min.second);
	clrtoeol();
	cbreak();
	noecho();
	refresh();
	return buffer;
}

static void command_new()
{
	std::size_t width;

	width = std::stoi(prompt("Enter width: "));
	if (width == 0) return;
	clear();
	maze.width(width);
	maze.clear();
	position = { 0, 0 };
	maze.start(position);
	maze.goal(position);
}

static void command_open()
{
	std::string filename;
	std::ifstream ifs;

	filename = prompt("Enter filename: ");
	if (filename.empty()) return;
	ifs.open(filename);
	if (!ifs.is_open()) return;
	clear();
	ifs >> maze;
	position = { 0, 0 };
	maze_filename = filename;
}

static void command_save(bool save_as)
{
	std::string filename;
	std::ofstream ofs;
	
	if (save_as || maze_filename.empty()) {
		filename = prompt("Enter filename: ");
		if (filename.empty()) return;
		maze_filename = filename;
	}
	ofs.open(maze_filename);
	if (!ofs.is_open()) return;
	ofs << maze;
	ofs.close();
}

static void command_exit()
{
	endwin();
	std::exit(0);
}

static void command_move_up()
{
	if (position.first > 0)
		--position.first;
}

static void command_move_left()
{
	if (position.second > 0)
		--position.second;
}

static void command_move_down()
{
	if (position.first < maze.width() - 1)
		++position.first;
}

static void command_move_right()
{
	if (position.second < maze.width() - 1)
		++position.second;
}

static void command_wall_up()
{
	maze[position].flip(micromouse::WALL_TOP);
	if (position.first > 0) {
		maze[{position.first - 1, position.second}].flip(
					micromouse::WALL_BOTTOM);
	}
}

static void command_wall_left()
{
	maze[position].flip(micromouse::WALL_LEFT);
	if (position.second > 0) {
		maze[{position.first, position.second - 1}].flip(
					micromouse::WALL_BOTTOM);
	}
}

static void command_wall_down()
{
	maze[position].flip(micromouse::WALL_BOTTOM);
	if (position.first < maze.width() - 1) {
		maze[{position.first + 1, position.second}].flip(
						micromouse::WALL_TOP);
	}
}

static void command_wall_right()
{
	maze[position].flip(micromouse::WALL_RIGHT);
	if (position.second < maze.width() - 1) {
		maze[{position.first, position.second + 1}].flip(
						micromouse::WALL_LEFT);
	}
}

static void command_set_start()
{
	maze.start(position);
}

static void command_set_goal()
{
	maze.goal(position);
}

static void command_view_path()
{
	std::string filename;
	std::ifstream ifs;
	micromouse::maze::size_type length, index;

	if (!path.empty()) {
		path.clear();
		return;
	}
	filename = prompt("Enter filename: ");
	if (filename.empty()) return;
	ifs.open(filename);
	if (!ifs.is_open()) return;
	ifs >> length;
	path.resize(length);
	for (std::size_t i = 0; i < length; ++i) {
		ifs >> index;
		path[i].first = index / maze.width();
		path[i].second = index % maze.width();
	}
	ifs.close();
}

static void input()
{
	switch (getch()) {
	case KEY_F(1): command_new(); break;
	case KEY_F(2): command_open(); break;
	case KEY_F(3): command_save(false); break;
	case KEY_F(4): command_save(true); break;
	case KEY_F(5): command_exit(); break;
	case 'w': command_move_up(); break;
	case 'a': command_move_left(); break;
	case 's': command_move_down(); break;
	case 'd': command_move_right(); break;
	case 'i': command_wall_up(); break;
	case 'j': command_wall_left(); break;
	case 'k': command_wall_down(); break;
	case 'l': command_wall_right(); break;
	case '1': command_set_start(); break;
	case '2': command_set_goal(); break;
	case '3': command_view_path(); break;
	default: break;
	}
}

static void initialize()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
}

int main()
{
	initialize();
	while (true) {
		getbegyx(stdscr, window_min.first, window_min.second);
		draw();
		input();
	}
	return 0;
}

