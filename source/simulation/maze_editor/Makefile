all : maze_editor

clean :
	rm maze_editor

maze_editor : main.cc maze.cc maze.hh
	g++ -std=c++11 -o maze_editor main.cc maze.cc -lncurses

