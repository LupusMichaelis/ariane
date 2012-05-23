#ifndef HPP_CANVAS_SDL_QUEST
#	define HPP_CANVAS_SDL_QUEST

#	include "surface.hpp"

class Canvas ;

void clone(std::shared_ptr<Canvas> & p_target, Surface const & source) ;

class Canvas
	: public Surface
{
		friend
		void create(std::shared_ptr<Canvas> & p_surface, VideoMode const & videomode) ;
	public:
		virtual
		~Canvas() ;

		Canvas(Canvas const & copied) ;

		virtual
		void resize(Size const & new_size) ;

	private:
		Canvas() ;
		explicit Canvas(VideoMode const & videomode) ;

		void init() ;

		void clone(std::shared_ptr<Canvas> & p_target) ;
		virtual
		void release() throw() ;
} ;

#endif // define HPP_CANVAS_SDL_QUEST
