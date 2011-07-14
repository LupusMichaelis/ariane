
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"
#	include "color.hpp"

#	include <memory>

#	include <boost/utility.hpp>


class Surface
	: boost::noncopyable
{
	public:
		virtual
		~Surface() ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		VideoMode const & videomode() const ;

		void draw(Surface const & motif) ;
		void draw(Surface const & motif, Size const & at) ;
		void update() const ;
		void fill(RGBColor const & color) ;
		void fill(Surface const & pattern) ;
		void fill(Surface const & pattern, Size const & from, Size const & to) ;
		void resize(Size const & new_size) ;

		void dump(std::string const & filename) ;

		void crop(Surface & target, Size const & origin, Size const & size) const ;

	protected:
		explicit Surface(VideoMode const & videomode) ;
		void set(void * raw) ;
		void * get() const ;

		void videomode(VideoMode const & new_videomode) ;

	private:
		virtual
		void init() = 0 ;
		virtual
		void release() throw() = 0 ;

	private:
		VideoMode m_videomode ;

		void * mp_raw ;
} ;


#endif // define HPP_SURFACE_SDL_QUEST
