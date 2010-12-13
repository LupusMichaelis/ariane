
#ifndef HPP_SCREEN_SDL_QUEST
#	define HPP_SCREEN_SDL_QUEST

#	include "surface.hpp"

#	include <memory>

class VideoMode ;

class Screen
	: public Surface
{
	public:
		Screen(int width, int height, int depth) ;
		explicit Screen(VideoMode const & videomode) ;

		virtual
		~Screen() ;

		virtual
		void init() ;
		virtual
		void release() throw() ;
} ;

#endif // define HPP_SCREEN_SDL_QUEST
