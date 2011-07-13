#ifndef HPP_CANVAS_SDL_QUEST
#	define HPP_CANVAS_SDL_QUEST

#	include "surface.hpp"

class Canvas
	: public Surface
{
	public:
		virtual
		~Canvas() ;

		static
		void create(std::shared_ptr<Canvas> & p_surface, VideoMode const & videomode)
		{
			std::shared_ptr<Canvas> p_new_surface(new Canvas(videomode)) ;
			p_new_surface->init() ;
			std::swap(p_surface, p_new_surface) ;
		}

	private:
		Canvas() ;
		explicit Canvas(VideoMode const & videomode) ;

		void init() ;

		virtual
		void release() throw() ;
} ;

#endif // define HPP_CANVAS_SDL_QUEST
