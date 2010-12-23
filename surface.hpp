
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"

#	include <memory>

class Size ;
typedef Size Position ;

class Surface
{
	public:
		//Surface() ;
		explicit Surface(VideoMode const & videomode) ;

		virtual
		~Surface() ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		VideoMode const & videomode() const ;

		virtual
		void init() = 0 ;
		virtual
		void release() throw() = 0 ;

		virtual
		void draw(Surface const & motif) ;
		virtual
		void draw(Surface const & motif, Position const & at) ;
		virtual
		void update() const ;
		virtual
		void fill(unsigned color) ;

	protected:
		void set(void * raw) ;
		void * get() const ;

	private:
		VideoMode m_videomode ;

		void * mp_raw ;
} ;


#endif // define HPP_SURFACE_SDL_QUEST
