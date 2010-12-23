#ifndef HPP_CANVAS_SDL_QUEST
#	define HPP_CANVAS_SDL_QUEST

#	include "surface.hpp"

class Canvas
	: public Surface
{
	friend
	void create<Canvas>(std::auto_ptr<Canvas> & p_surface, VideoMode const & videomode) ;

	public:
		virtual
		~Canvas() ;

	private:
		Canvas() ;
		explicit Canvas(VideoMode const & videomode) ;

		virtual
		void init() ;
		virtual
		void release() throw() ;
} ;

#endif // define HPP_CANVAS_SDL_QUEST
