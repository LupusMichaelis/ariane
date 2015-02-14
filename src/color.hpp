#ifndef HPP_COLOR_SDL_QUEST
#	define HPP_COLOR_SDL_QUEST

#	include <string>

class Color
{
	public:
		virtual
		~Color();
};

class RGBColor;
RGBColor const create_color(unsigned hex);

class RGBColor
	: public Color
{
	public:
		RGBColor(unsigned char red, unsigned char green, unsigned char blue)
			: Color()
			, m_red(red)
			, m_green(green)
			, m_blue(blue)
		{ }

		unsigned char red() const	{ return m_red; }
		unsigned char green() const	{ return m_green; }
		unsigned char blue() const	{ return m_blue; }

	protected:
		unsigned char m_red;
		unsigned char m_green;
		unsigned char m_blue;
};

bool const operator ==(RGBColor const & lhs, RGBColor const & rhs);

#endif // define HPP_COLOR_SDL_QUEST
