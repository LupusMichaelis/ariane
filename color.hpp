#ifndef HPP_COLOR_SDL_QUEST
#	define HPP_COLOR_SDL_QUEST

#	include <string>

class Color
{
	public:
		virtual
		~Color() ;
} ;

class RGBColor ;
RGBColor const create_color(unsigned hex) ;

class RGBColor
	: public Color
{
	public:
		RGBColor(char red, char green, char blue)
			: Color()
			, m_red(red)
			, m_green(green)
			, m_blue(blue)
		{ }

		char red() const	{ return m_red ; }
		char green() const	{ return m_green ; }
		char blue() const	{ return m_blue ; }

	protected:
		char m_red ;
		char m_green ;
		char m_blue ;
} ;

#endif // define HPP_COLOR_SDL_QUEST
