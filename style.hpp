#ifndef HPP_STYLE_SDL_QUEST
#	define HPP_STYLE_SDL_QUEST

#	include <memory>
#	include "color.hpp"
#	include "videomode.hpp"

class Pen ;

class Font
{
	public:
		typedef std::shared_ptr<Font>		SharedPtr ;

	public:
		Font(Font const & copied) ;
		Font & operator =(Font const & copied) ;
		Font(std::string const & name, unsigned const size) ;
		virtual
		~Font() ;

		virtual
		unsigned const size() const ;
		virtual
		std::string const & name() const ;

	private:
		class Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class Font */ ;

bool const operator ==(Font const & lhs, Font const & rhs) ;
bool const operator !=(Font const & lhs, Font const & rhs) ;

class Pen
{
	public:
		Pen(Pen const & copied) ;
		Pen & operator =(Pen const & copied) ;
		Pen(Font const & set_font, RGBColor const & set_color, unsigned const set_size) ;

		void color(RGBColor const & new_color) ;
		RGBColor const & color() const ;

		void font(Font const & new_font) ;
		Font const & font() const ;

		void size(unsigned const new_size) ;
		unsigned const size() const ;

		~Pen() ;

	private:
		class Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class Pen */ ;

class Style
{
	public:
		Style(Pen const & set_pen
				, RGBColor const & set_color
				, Size const & set_position
				, Size const & set_padding
				, Size const & set_size
			) ;

		Style(Style const & copied) ;
		Style & operator =(Style const & copied) ;

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

		virtual ~Style() ;

	private:
		class Impl ;
		std::unique_ptr<Impl>	mp_impl ;

} /* class Style */ ;

bool const operator ==(Pen const & lhs, Pen const & rhs) ;
bool const operator !=(Pen const & lhs, Pen const & rhs) ;

bool const operator ==(Style const & lhs, Style const & rhs) ;
bool const operator !=(Style const & lhs, Style const & rhs) ;

#	endif // HPP_STYLE_SDL_QUEST
