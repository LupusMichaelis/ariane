#ifndef HPP_API_QUEST
#	define HPP_API_QUEST

#	include <memory>
#	include <boost/utility.hpp>

class RGBColor ;
class Style ;
class VideoMode ;
class Size ;
class GuiLayout ;

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

class ImageSurface
	: public Surface
{
	public:
		virtual ~ImageSurface() = 0 ;

	protected:
		explicit ImageSurface(std::string const & filename) ;

	private:

		std::string m_filename ;
} /* class Image */ ;


#endif // define HPP_API_QUEST
