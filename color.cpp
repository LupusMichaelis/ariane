
#include "color.hpp"

Color::~Color()
{
}

RGBColor const create_color(unsigned hex)
{
	return RGBColor(hex >> 16, hex >> 8, hex) ;
}
