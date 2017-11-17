#ifndef MICROMOUSE_MAZE_HH
#define MICROMOUSE_MAZE_HH
#include <istream>
#include <ostream>
#include <iomanip>
#include <vector>
#include <bitset>
#include <utility>
#include <cmath>

namespace micromouse {

	enum wall {
		WALL_TOP,
		WALL_RIGHT,
		WALL_BOTTOM,
		WALL_LEFT
	};
	
	class maze {
	public:
		typedef std::size_t size_type;
		typedef std::bitset<4> cell_type;
		typedef std::pair<size_type, size_type> position_type;

		maze() = default;
		maze(const maze&) = default;
		maze(maze&&) = default;
		
		maze& operator=(const maze&) = default;
		maze& operator=(maze&&) = default;

		void swap(maze& other);

		size_type width() const;
		void width(size_type new_width);

		const position_type& start() const;
		void start(const position_type& new_start);

		const position_type& goal() const;
		void goal(const position_type& new_goal);
		
		const cell_type& operator[](const position_type& p) const;
		cell_type& operator[](const position_type& p);

		void clear();
	private:
		position_type start_, goal_;
		std::vector< std::vector< std::bitset<4> > > cells_;
	};

	inline void swap(maze& a, maze& b) { a.swap(b); }

	std::istream& operator>>(std::istream& is, maze& m);
	std::ostream& operator<<(std::ostream& os, const maze& m);

};

#endif
