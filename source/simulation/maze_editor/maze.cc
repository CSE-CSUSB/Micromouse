#include "maze.hh"

void micromouse::maze::swap(maze& other)
{
	using std::swap;
	swap(start_, other.start_);
	swap(goal_, other.goal_);
	swap(cells_, other.cells_);
}

micromouse::maze::size_type micromouse::maze::width() const
{
	return cells_.size();
}

void micromouse::maze::width(size_type new_width)
{
	cells_.resize(new_width);
	for (auto& row : cells_) row.resize(new_width);
	update_all_corners();
}

const micromouse::maze::position_type& micromouse::maze::start() const
{
	return start_;
}

void micromouse::maze::start(const position_type& new_start)
{
	start_ = new_start;
}

const micromouse::maze::position_type& micromouse::maze::goal() const
{
	return goal_;
}

void micromouse::maze::goal(const position_type& new_goal)
{
	goal_ = new_goal;
}

const micromouse::cell& micromouse::maze::operator[](const position_type& p) const
{
	return cells_[p.first][p.second];
}

micromouse::cell& micromouse::maze::operator[](const position_type& p)
{
	return cells_[p.first][p.second];
}

void micromouse::maze::clear()
{
	cell empty_cell;

	for (auto& row : cells_)
		std::fill(row.begin(), row.end(), empty_cell);
	update_all_corners();
}

void micromouse::maze::update_upper_left_corner(const position_type& p)
{
	micromouse::cell::cross_type* cross;

	cross = &operator[](p).corners[CORNER_UPPER_LEFT];
	cross->set(CROSS_BOTTOM, operator[](p).walls.test(
					micromouse::WALL_LEFT));
	cross->set(CROSS_RIGHT, operator[](p).walls.test(
					micromouse::WALL_TOP));
	cross->set(CROSS_TOP, p.first > 0 &&
				operator[]({p.first-1, p.second}).walls.test(
				micromouse::WALL_LEFT));
	cross->set(CROSS_LEFT, p.second > 0 &&
				operator[]({p.first, p.second-1}).walls.test(
				micromouse::WALL_TOP));
}

void micromouse::maze::update_upper_right_corner(const position_type& p)
{
	micromouse::cell::cross_type* cross;

	cross = &operator[](p).corners[CORNER_UPPER_RIGHT];
	cross->set(CROSS_BOTTOM, operator[](p).walls.test(
				micromouse::WALL_RIGHT));
	cross->set(CROSS_LEFT, operator[](p).walls.test(
				micromouse::WALL_TOP));
	cross->set(CROSS_TOP, p.first > 0 &&
				operator[]({p.first-1, p.second}).walls.test(
				micromouse::WALL_RIGHT));
	cross->set(CROSS_RIGHT, p.second < width()-1 &&
				operator[]({p.first, p.second+1}).walls.test(
				micromouse::WALL_TOP));
}

void micromouse::maze::update_lower_left_corner(const position_type& p)
{
	micromouse::cell::cross_type* cross;

	cross = &operator[](p).corners[CORNER_LOWER_LEFT];
	cross->set(CROSS_TOP, operator[](p).walls.test(micromouse::WALL_LEFT));
	cross->set(CROSS_RIGHT, operator[](p).walls.test(
				micromouse::WALL_BOTTOM));
	cross->set(CROSS_LEFT, p.second > 0 &&
				operator[]({p.first, p.second-1}).walls.test(
				micromouse::WALL_BOTTOM));
	cross->set(CROSS_BOTTOM, p.first < width()-1 &&
				operator[]({p.first+1, p.second}).walls.test(
				micromouse::WALL_LEFT));
}

void micromouse::maze::update_lower_right_corner(const position_type& p)
{
	micromouse::cell::cross_type* cross;

	cross = &operator[](p).corners[CORNER_LOWER_RIGHT];
	cross->set(CROSS_TOP, operator[](p).walls.test(micromouse::WALL_RIGHT));
	cross->set(CROSS_LEFT, operator[](p).walls.test(micromouse::WALL_BOTTOM));
	cross->set(CROSS_RIGHT, p.second < width()-1 &&
				operator[]({p.first, p.second+1}).walls.test(
				micromouse::WALL_BOTTOM));
	cross->set(CROSS_BOTTOM, p.first < width()-1 &&
				operator[]({p.first+1, p.second}).walls.test(
				micromouse::WALL_RIGHT));
}

void micromouse::maze::update_corners(const position_type& p)
{
	update_upper_left_corner(p);
	update_upper_right_corner(p);
	update_lower_left_corner(p);
	update_lower_right_corner(p);
}

void micromouse::maze::update_all_corners()
{
	maze::position_type p;

	for (p.first = 0; p.first < width(); ++p.first) {
		for (p.second = 0; p.second < width(); ++p.second) {
			update_corners(p);
		}
	}
}

std::istream& micromouse::operator>>(std::istream& is, maze& m)
{
	maze::size_type n;
	maze::position_type p;

	is >> n;
	m.width(std::sqrt(n));
	is >> n;
	p.first = n / m.width();
	p.second = n % m.width();
	m.start(p);
	is >> n;
	p.first = n / m.width();
	p.second = n % m.width();
	m.goal(p);
	for (p.first = 0; p.first < m.width(); ++p.first) {
		for (p.second = 0; p.second < m.width(); ++p.second) {
			is >> n;
			m[p].walls = std::bitset<4>{static_cast<unsigned long>(n)};
		}
	}
	m.update_all_corners();
	return is;
}

std::ostream& micromouse::operator<<(std::ostream& os, const maze& m)
{
	maze::position_type p;

	os <<
		m.width() * m.width() << ' ' <<
		m.start().first * m.width() + m.start().second << ' ' <<
		m.goal().first * m.width() + m.goal().second << ' ';
	for (p.first = 0; p.first < m.width(); ++p.first) {
		for (p.second = 0; p.second < m.width(); ++p.second) {
			os << m[p].walls.to_ulong() << ' ';
		}
	}
	os << std::flush;
	return os;
}

