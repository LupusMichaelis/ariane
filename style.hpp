#ifndef HPP_STYLE_SDL_QUEST
#	define HPP_STYLE_SDL_QUEST

#	include <memory>
#	include "color.hpp"

class Surface ;

class Style
{
	public:
		static
		void create(std::shared_ptr<Style> & p_style, Surface & screen) ;

	private:
		Style(Surface & parent_screen) ;

	public:
		void color(RGBColor const & new_color) ;
		RGBColor const & color() const ;

		void font(std::string const & new_font) ;
		std::string const & font() const ;

		void size(unsigned const & new_size) ;
		unsigned const size() const ;

	private:
		Surface & m_screen ;

		RGBColor	m_color ;
		std::string	m_font ;
		unsigned	m_size ;
} /* class Style */ ;

#	endif // HPP_STYLE_SDL_QUEST
