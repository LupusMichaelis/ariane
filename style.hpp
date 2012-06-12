#ifndef HPP_STYLE_SDL_QUEST
#	define HPP_STYLE_SDL_QUEST

#	include <memory>
#	include "color.hpp"
#	include "videomode.hpp"

class Surface ;

class Font
{
	public:
		Font() ;
		Font(std::string const & font_name) ;

		std::string const & name() const ;
		void name(std::string const & new_name) ;

	private:
		std::string		m_name ;

} /* class Font */ ;

class Pen
{
	public:
		Pen() ;
		Pen(Font const & set_font, RGBColor const & set_color, unsigned const set_size) ;

		void color(RGBColor const & new_color) ;
		RGBColor const & color() const ;

		void font(Font const & new_font) ;
		Font const & font() const ;

		void size(unsigned const new_size) ;
		unsigned const size() const ;

	private:
		Font		m_font ;
		RGBColor	m_color ;
		unsigned	m_size ;

} /* class Pen */ ;

class Style
{
	public:
		Style() ;

		void pen(Pen const & new_pen) ;
		Pen const & pen() const ;

		void padding(Size const & new_padding) ;
		Size const & padding() const ;

		void position(Size const & new_size) ;
		Size const & position() const ;

		void size(Size const & new_size) ;
		Size const & size() const ;

		void color(RGBColor const & new_color) ;
		RGBColor const & color() const ;

	private:
		Pen			m_pen ;
		RGBColor	m_color ;
		Size		m_padding ;
		Size		m_position ;
		Size		m_size ;

} /* class Style */ ;

bool const operator ==(Font const & lhs, Font const & rhs) ;
bool const operator !=(Font const & lhs, Font const & rhs) ;

bool const operator ==(Pen const & lhs, Pen const & rhs) ;
bool const operator !=(Pen const & lhs, Pen const & rhs) ;

bool const operator ==(Style const & lhs, Style const & rhs) ;
bool const operator !=(Style const & lhs, Style const & rhs) ;

#	endif // HPP_STYLE_SDL_QUEST
