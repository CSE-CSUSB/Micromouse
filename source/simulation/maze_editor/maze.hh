#ifndef MICROMOUSE_MAZE_HH
#define MICROMOUSE_MAZE_HH
#include <istream>
#include <ostream>
#include <iomanip>
#include <vector>
#include <array>
#include <bitset>
#include <utility>
#include <cmath>

namespace micromouse {

	enum wall_bits {
		WALL_TOP,
		WALL_RIGHT,
		WALL_BOTTOM,
		WALL_LEFT
	};

	enum corner_bits {
		CORNER_UPPER_LEFT,
		CORNER_UPPER_RIGHT,
		CORNER_LOWER_LEFT,
		CORNER_LOWER_RIGHT
	};

	enum cross_bits {
		CROSS_TOP,
		CROSS_RIGHT,
		CROSS_BOTTOM,
		CROSS_LEFT
	};

	struct cell {
		typedef std::bitset<4> cross_type;
		typedef std::array<cross_type, 4> corners_type;
		typedef std::bitset<4> walls_type;
		
		corners_type corners;
		walls_type walls;
	};

	class maze {
		friend std::istream& operator>>(std::istream& is, maze& m);
	public:
		typedef std::size_t size_type;
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
		
		const cell& operator[](const position_type& p) const;
		cell& operator[](const position_type& p);

		void clear();

		void update_all_corners();
		void update_corners(const position_type& p);
	private:
		position_type start_, goal_;
		std::vector< std::vector< cell > > cells_;

		void update_upper_left_corner(const position_type& p);
		void update_upper_right_corner(const position_type& p);
		void update_lower_left_corner(const position_type& p);
		void update_lower_right_corner(const position_type& p);
	};

	inline void swap(maze& a, maze& b) { a.swap(b); }

	std::istream& operator>>(std::istream& is, maze& m);
	std::ostream& operator<<(std::ostream& os, const maze& m);

};

#endif
