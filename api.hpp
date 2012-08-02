#ifndef HPP_API_QUEST
#	define HPP_API_QUEST

#	include <memory>
#	include <boost/utility.hpp>

class GuiLayout ;
class RGBColor ;
class VideoMode ;
class Size ;
class Style ;

class Surface
	//: boost::noncopyable
{
	public:
		virtual VideoMode const videomode() const = 0 ;

		virtual void draw(Surface const & motif) = 0 ;
		virtual void draw(Surface const & motif, Size const & at) = 0 ;
		virtual void update() const = 0 ;
		virtual void fill(RGBColor const & color) = 0 ;
		virtual void fill(Surface const & pattern, Size const & from, Size const & to) = 0 ;
		virtual void resize(Size const & new_size) = 0 ;
		virtual void dump(std::string const & filename) = 0 ;
		virtual void crop(Surface & target, Size const & origin, Size const & size) const = 0;

		virtual void write(std::string const & message, Style const & style) = 0 ;

		virtual ~Surface() = 0 ;

		virtual const GuiLayout & gui_layout() const = 0 ;
		virtual GuiLayout & gui_layout() = 0 ;

} /* class Surface */ ;

class Gui ;
class Style ;

class Drawable
{
	public:
		virtual Surface /* const */ & surface() = 0 ;
		virtual void draw() = 0 ;

		virtual Gui const & gui() = 0 ;

		virtual void style(Style const & new_style) = 0 ;
		virtual Style const & style() const = 0 ;
		virtual Style style() = 0 ;

		virtual ~Drawable() = 0 ;

} /* class Drawable */ ;

#include <boost/signals2/trackable.hpp>

class EventTarget
	: public boost::signals2::trackable
{
	public:
		virtual ~EventTarget() = 0 ;

} /* class EventTarget */ ;

#endif // define HPP_API_QUEST