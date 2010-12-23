
#include "canvas.hpp"

#include <SDL.h>

void Canvas::release()
	throw()
{
	SDL_Surface * p_raw = reinterpret_cast<SDL_Surface *>(get()) ;
	SDL_FreeSurface(p_raw) ;
}

void Canvas::init()
{
	SDL_Surface * p_raw = SDL_CreateRGBSurface(SDL_SWSURFACE
			, width(*this), height(*this), depth(*this)
			//, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000) ;
			, 0, 0, 0, 0) ;
	if(!p_raw)
		throw SDL_GetError() ;

	set(p_raw) ;
}

Canvas::Canvas()
	: Surface(create_videomode(0, 0, 0))
{
}

Canvas::Canvas(VideoMode const & videomode)
	: Surface(videomode)
{
}

Canvas::~Canvas()
{ }

