
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"
#	include "color.hpp"

#	include <memory>

class Size ;
typedef Size Position ;

template <typename SurfaceT>
void create(std::auto_ptr<SurfaceT> & p_surface, VideoMode const & videomode)
{
	std::auto_ptr<SurfaceT> p_new_surface(new SurfaceT(videomode)) ;
	p_new_surface->init() ;
	std::swap(p_surface, p_new_surface) ;
}


class Surface
{
	friend
	void create<Surface>(std::auto_ptr<Surface> & p_surface, VideoMode const & videomode) ;

	public:

		virtual
		~Surface() ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		VideoMode const & videomode() const ;

		void draw(Surface const & motif) ;
		void draw(Surface const & motif, Position const & at) ;
		void update() const ;
		void fill(RGBColor const & color) ;
		void fill(Surface const & pattern) ;
		void fill(Surface const & pattern, Position const & from, Position const & to) ;

	protected:
		explicit Surface(VideoMode const & videomode) ;
		void set(void * raw) ;
		void * get() const ;

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
