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

const micromouse::maze::cell_type& micromouse::maze::operator[](const position_type& p) const
{
	return cells_[p.first][p.second];
}

micromouse::maze::cell_type& micromouse::maze::operator[](const position_type& p)
{
	return cells_[p.first][p.second];
}

void micromouse::maze::clear()
{
	for (auto& row : cells_)
		std::fill(row.begin(), row.end(), std::bitset<4>{});
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
			m[p] = std::bitset<4>{static_cast<unsigned long>(n)};
		}
	}
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
			os << m[p].to_ulong() << ' ';
		}
	}
	os << std::flush;
	return os;
}
