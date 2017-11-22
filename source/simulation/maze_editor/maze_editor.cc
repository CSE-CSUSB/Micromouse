#include "maze.hh"
#include <ncurses/curses.h>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <cstdlib>

static micromouse::maze maze;
static std::vector<micromouse::maze::position_type> path;
static micromouse::maze::position_type position;
static std::string maze_filename;
static std::pair<int ,int> window_min;

static void draw_menu()
{
	mvprintw(window_min.first, window_min.second,
		"%-15s%-15s%-15s%-15s%-15s", "F1: NEW", "F2: OPEN",
		"F3: SAVE", "F4: SAVE AS", "F5: EXIT");
	mvprintw(window_min.first + 1, window_min.second,
		"%-15s%-15s%-15s%-15s%-15s", "1: SET START", "2: SET GOAL",
		"3: VIEW PATH", "WASD: MOVE", "IJKL: EDIT WALL");
}

static void draw_cell_wall_top(const micromouse::maze::position_type& p)
{
	move(window_min.first + 4 + p.first * 2,
		window_min.second + p.second * 4 + 1);
	if (maze[p].walls.test(micromouse::WALL_TOP)) {
		addch(ACS_HLINE);
		addch(ACS_HLINE);
		addch(ACS_HLINE);
	} else {
		addch(' ');
		addch(' ');
		addch(' ');
	}
}

static void draw_cell_wall_left(const micromouse::maze::position_type& p)
{
	move(window_min.first + 5 + p.first * 2,
		window_min.second + p.second * 4);
	if (maze[p].walls.test(micromouse::WALL_LEFT)) {
		addch(ACS_VLINE);
	} else {
		addch(' ');
	}
}

static void draw_cell_wall_right(const micromouse::maze::position_type& p)
{
	move(window_min.first + 5 + p.first * 2,
		window_min.second + p.second * 4 + 4);
	if (maze[p].walls.test(micromouse::WALL_RIGHT)) {
		addch(ACS_VLINE);
	} else {
		addch(' ');
	}
}

static void draw_cell_wall_bottom(const micromouse::maze::position_type& p)
{
	move(window_min.first + 6 + p.first * 2,
		window_min.second + p.second * 4 + 1);
	if (maze[p].walls.test(micromouse::WALL_BOTTOM)) {
		addch(ACS_HLINE);
		addch(ACS_HLINE);
		addch(ACS_HLINE);
	} else {
		addch(' ');
		addch(' ');
		addch(' ');
	}
}

static void draw_cell_middle(const micromouse::maze::position_type& p)
{
	move(window_min.first + 5 + p.first * 2,
		window_min.second + p.second * 4 + 1);
	addch(' ');
	addch(' ');
	addch(' ');
}

static int cell_corner_to_character(const micromouse::cell::cross_type& c)
{
	std::string s;

	s = c.to_string();
	if (s == "0000") return ACS_BULLET;
	else if (s == "0001") return ' '; 
	else if (s == "0010") return ' ';
	else if (s == "0011") return ACS_LLCORNER;
	else if (s == "0100") return ' ';
	else if (s == "0101") return ACS_VLINE;
	else if (s == "0110") return ACS_ULCORNER;
	else if (s == "0111") return ACS_LTEE;
	else if (s == "1000") return ' ';
	else if (s == "1001") return ACS_LRCORNER;
	else if (s == "1010") return ACS_HLINE;
	else if (s == "1011") return ACS_BTEE;
	else if (s == "1100") return ACS_URCORNER;
	else if (s == "1101") return ACS_RTEE;
	else if (s == "1110") return ACS_TTEE;
	else if (s == "1111") return ACS_PLUS;
	return ACS_BULLET;
}

static void draw_cell_upper_left_corner(const micromouse::maze::position_type& p)
{
	move(window_min.first + 4 + p.first * 2,
		window_min.second + p.second * 4);
	addch(cell_corner_to_character(maze[p].corners[micromouse::CORNER_UPPER_LEFT]));
}

static void draw_cell_upper_right_corner(const micromouse::maze::position_type& p)
{
	move(window_min.first + 4 + p.first * 2,
		window_min.second + p.second * 4 + 4);
	addch(cell_corner_to_character(maze[p].corners[micromouse::CORNER_UPPER_RIGHT]));
}

static void draw_cell_lower_left_corner(const micromouse::maze::position_type& p)
{
	move(window_min.first + 6 + p.first * 2,
		window_min.second + p.second * 4 + 4);
	addch(cell_corner_to_character(maze[p].corners[micromouse::CORNER_LOWER_LEFT]));
}

static void draw_cell_lower_right_corner(const micromouse::maze::position_type& p)
{
	move(window_min.first + 6 + p.first * 2,
		window_min.second + p.second * 4 + 4);
	addch(cell_corner_to_character(maze[p].corners[micromouse::CORNER_LOWER_RIGHT]));
}

static void draw_cell_corners(const micromouse::maze::position_type& p)
{
	draw_cell_upper_left_corner(p);
	draw_cell_upper_right_corner(p);
	draw_cell_lower_left_corner(p);
	draw_cell_lower_right_corner(p);
}

static void draw_cell(const micromouse::maze::position_type& p)
{
	draw_cell_corners(p);
	draw_cell_wall_top(p);
	draw_cell_wall_left(p);
	draw_cell_middle(p);
	draw_cell_wall_right(p);
	draw_cell_wall_bottom(p);
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

static std::string prompt(const std::string& message)
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
	maze_filename.clear();
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
	maze[position].walls.flip(micromouse::WALL_TOP);
	if (position.first > 0) {
		maze[{position.first - 1, position.second}].walls.flip(
					micromouse::WALL_BOTTOM);
	}
	maze.update_all_corners();
}

static void command_wall_left()
{
	maze[position].walls.flip(micromouse::WALL_LEFT);
	if (position.second > 0) {
		maze[{position.first, position.second - 1}].walls.flip(
					micromouse::WALL_RIGHT);
	}
	maze.update_all_corners();
}

static void command_wall_down()
{
	maze[position].walls.flip(micromouse::WALL_BOTTOM);
	if (position.first < maze.width() - 1) {
		maze[{position.first + 1, position.second}].walls.flip(
						micromouse::WALL_TOP);
	}
	maze.update_all_corners();
}

static void command_wall_right()
{
	maze[position].walls.flip(micromouse::WALL_RIGHT);
	if (position.second < maze.width() - 1) {
		maze[{position.first, position.second + 1}].walls.flip(
						micromouse::WALL_LEFT);
	}
	maze.update_all_corners();
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

