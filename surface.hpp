
#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"

#	include <memory>

class Surface
{
		class Impl ;
	public:
		Surface() ;
		explicit Surface(VideoMode const & videomode) ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		void * get() const ;

		void set_background(unsigned color) ;

		int width() const ;
		int height() const ;
		int depth() const ;

	private:
		std::auto_ptr<Impl> mp_impl ;
} ;

#endif // define HPP_SURFACE_SDL_QUEST
