
#ifndef HPP_SCREEN_SDL_QUEST
#	define HPP_SCREEN_SDL_QUEST

#	include "surface.hpp"

#	include <memory>

class VideoMode ;

class Screen
	: public Surface
{
	public:
		virtual
		~Screen() ;

		static
		void create(std::shared_ptr<Screen> & p_surface, VideoMode const & videomode)
		{
			std::shared_ptr<Screen> p_new_surface(new Screen(videomode)) ;
			p_new_surface->init() ;
			std::swap(p_surface, p_new_surface) ;
		}

	protected:
		Screen(int width, int height, int depth) ;
		explicit Screen(VideoMode const & videomode) ;

		virtual
		void init() ;
		virtual
		void release() throw() ;
} ;

#endif // define HPP_SCREEN_SDL_QUEST
