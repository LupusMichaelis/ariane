#ifndef HPP_CANVAS_SDL_QUEST
#	define HPP_CANVAS_SDL_QUEST

#	include "surface.hpp"

class Canvas
	: public Surface
{
	public:
		Canvas() ;
		explicit Canvas(VideoMode const & videomode) ;

		virtual
		~Canvas() ;

		virtual
		void init() ;
		virtual
		void release() throw() ;
} ;

#endif // define HPP_CANVAS_SDL_QUEST
