#include "maze.hh"
#include <ncurses/curses.h>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <fstream>

static micromouse::maze maze;
static micromouse::maze::position_type current_position;
static std::string maze_filename;
static std::pair<int, int> window_min, window_max;

static void initialize();
static void input();
static void draw();
static std::string prompt(const std::string& message);
static void save(std::string filename);

int main()
{
	initialize();
	while (true) {
		draw();
		input();
	}
	return 0;
}

static void initialize()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
}

static void draw()
{
	getbegyx(stdscr, window_min.first, window_min.second);
	getmaxyx(stdscr, window_max.first, window_max.second);

	auto draw_menu =
	[&]()
	{
		mvaddstr(window_min.first, window_min.second,
			"F1: NEW | F2: OPEN | F3: SAVE | F4: SAVE AS |"
			" F5: EXIT");
		mvaddstr(window_min.first + 1, window_min.second,
			"WASD: MOVE | IJKL: TOGGLE WALL | O: SET START |"
			" P: SET GOAL");
	};

	auto draw_maze =
	[&]()
	{
		micromouse::maze::position_type p;

		for (p.first = 0; p.first < maze.width(); ++p.first) {
			for (p.second = 0; p.second < maze.width();
							++p.second) {
				move(window_min.first + 3 + p.first * 2,
					window_min.second + p.second * 4);
				if (maze[p].test(micromouse::WALL_TOP))
					addstr("+---+");
				else
					addstr("+   +");
				move(window_min.first + 4 + p.first * 2,
					window_min.second + p.second * 4);
				if (maze[p].test(micromouse::WALL_LEFT))
					addstr("|   ");
				else
					addstr("    ");
				if (maze[p].test(micromouse::WALL_RIGHT))
					addch('|');
				else
					addch(' ');
				move(window_min.first + 5 + p.first * 2,
					window_min.second + p.second * 4);
				if (maze[p].test(micromouse::WALL_BOTTOM))
					addstr("+---+");
				else
					addstr("+   +");
			}
		}
	};

	auto draw_marker =
	[&](const micromouse::maze::position_type& p, int ch)
	{
		mvaddch(window_min.first + 4 + p.first * 2,
			window_min.second + 2 + p.second * 4, ch);
	};

	draw_menu();
	if (maze.width() != 0) {
		draw_maze();
		draw_marker(maze.start(), 'S');
		draw_marker(maze.goal(), 'G');
		draw_marker(current_position, 'X');
	}
	refresh();
}

static std::string prompt(const std::string& message)
{
	char buffer[80];

	mvaddstr(window_min.first + 2, window_min.second, message.c_str());
	nocbreak();
	echo();
	getnstr(buffer, sizeof(buffer) - 1);
	move(window_min.first + 2, window_min.second);
	clrtoeol();
	cbreak();
	noecho();
	refresh();
	return buffer;
}

static void save(std::string filename)
{
	std::ofstream ofs;

	if (filename.empty())
		filename = prompt("Enter filename: ");
	if (filename.empty()) return;
	ofs.open(filename);
	if (!ofs.is_open()) return;
	ofs << maze;
	ofs.close();
}

static const std::map< int, std::function<void()> > input_handlers{{
	{ KEY_F(1),
		[&]()
		{
			std::size_t width;

			width = std::stoi(prompt("Enter width: "));
			if (width == 0) return;
			clear();
			maze.width(width);
			maze.clear();
			current_position = { 0, 0 };
			maze.start({maze.width() - 1, 0});
			maze.goal({maze.width() - 1, maze.width() - 1});
		}
	},
	{ KEY_F(2),
		[&]()
		{
			std::string filename;
			std::ifstream ifs;

			filename = prompt("Enter filename: ");
			if (filename.empty()) return;
			ifs.open(filename);
			if (!ifs.is_open()) return;
			clear();
			ifs >> maze;
			current_position = { 0, 0 };
			maze_filename = filename;
		}
	},
	{ KEY_F(3),
		[&]()
		{
			save(maze_filename);
		}
	},
	{ KEY_F(4),
		[&]()
		{
			save("");
		}
	},
	{ KEY_F(5),
		[&]()
		{
			endwin();
			std::exit(0);
		}
	},
	{ 'w',
		[&]()
		{
			if (current_position.first > 0)
				--current_position.first;
		}
	},
	{ 'a',
		[&]()
		{
			if (current_position.second > 0)
				--current_position.second;
		}
	},
	{ 's',
		[&]()
		{
			if (current_position.first < maze.width() - 1)
				++current_position.first;
		}
	},
	{ 'd',
		[&]()
		{
			if (current_position.second < maze.width() - 1)
				++current_position.second;
		}
	},
	{ 'i',
		[&]()
		{
			maze[current_position].flip(micromouse::WALL_TOP);
			if (current_position.first > 0) {
				maze[{current_position.first - 1,
					current_position.second}].flip(
					micromouse::WALL_BOTTOM);
			}
		}
	},
	{ 'j',
		[&]()
		{
			maze[current_position].flip(micromouse::WALL_LEFT);
			if (current_position.second > 0) {
				maze[{current_position.first,
					current_position.second - 1}].flip(
					micromouse::WALL_RIGHT);
			}
		}
	},
	{ 'k',
		[&]()
		{
			maze[current_position].flip(micromouse::WALL_BOTTOM);
			if (current_position.first < maze.width() - 1) {
				maze[{current_position.first + 1,
					current_position.second}].flip(
					micromouse::WALL_TOP);
			}
		}
	},
	{ 'l',
		[&]()
		{
			maze[current_position].flip(micromouse::WALL_RIGHT);
			if (current_position.second < maze.width() - 1) {
				maze[{current_position.first,
					current_position.second + 1}].flip(
					micromouse::WALL_LEFT);
			}
		}
	},
	{ 'o',
		[&]()
		{
			maze.start(current_position);
		}
	},
	{ 'p',
		[&]()
		{
			maze.goal(current_position);
		}
	}
}};

static void input()
{
	auto handler = input_handlers.find(getch());
	if (handler != input_handlers.end()) handler->second();
}
