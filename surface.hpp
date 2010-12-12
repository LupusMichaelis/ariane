
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include <memory>
#	include "videomode.hpp"

class Screen ;

class Surface
{
		class Impl ;
	public:
		Surface() ;
		Surface(Screen const & screen, VideoMode const & videomode) ;

		void * get() const ;

		void set_background(unsigned color) ;

		int width() const ;
		int height() const ;
		int depth() const ;

	private:
		std::auto_ptr<Impl> mp_impl ;
} ;

#endif // define HPP_SURFACE_SDL_QUEST
