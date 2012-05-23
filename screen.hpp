
#ifndef HPP_SCREEN_SDL_QUEST
#	define HPP_SCREEN_SDL_QUEST

#	include "surface.hpp"

#	include <memory>

class VideoMode ;

class Screen
	: public Surface
{
		friend
		void create(std::shared_ptr<Screen> & p_surface, VideoMode const & videomode) ;
	public:
		virtual
		~Screen() ;

	protected:
		Screen(int width, int height, int depth) ;
		explicit Screen(VideoMode const & videomode) ;

		virtual
		void init() ;
		virtual
		void release() throw() ;
} /* class Screen */ ;

#endif // define HPP_SCREEN_SDL_QUEST
