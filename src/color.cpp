
#include "color.hpp"

Color::~Color()
{
}

RGBColor const create_color(unsigned hex)
{
	return RGBColor(hex >> 16, hex >> 8, hex);
}

bool const operator ==(RGBColor const & lhs, RGBColor const & rhs)
{
	return lhs.red() == rhs.red()
		&& lhs.green() == rhs.green()
		&& lhs.blue() == rhs.blue();
}
