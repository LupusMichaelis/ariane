
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"

#	include <memory>

class Size ;
typedef Size Position ;

class Surface
{
		class Impl ;

	public:
		Surface() ;
		explicit Surface(VideoMode const & videomode) ;

		virtual
		~Surface() ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		void * get() const ;
		void set(void *) ;

		VideoMode const & videomode() const ;

		virtual
		void init() ;
		virtual
		void release() throw() ;

		virtual
		void create(Surface & target, VideoMode const & videomode) ;
		virtual
		void draw(Surface const & motif) ;
		virtual
		void draw(Surface const & motif, Position const & at) ;

		virtual
		void update() const ;

		virtual
		void fill(unsigned color) ;

	protected:
		std::auto_ptr<Impl> mp_impl ;
		VideoMode m_videomode ;
} ;

inline
VideoMode const & Surface::videomode() const
{ return m_videomode ; }


#endif // define HPP_SURFACE_SDL_QUEST
