
#include "screen.hpp"
#include "surface.hpp"
#include "videomode.hpp"


#include <SDL/SDL.h>

Screen::Screen(VideoMode const & videomode)
	: Surface(videomode)
{ }

Screen::Screen(int width, int height, int depth)
	: Surface(create_videomode(width, height, depth))
{ }


void Screen::release()
	throw()
{
	SDL_Quit() ;
}

void Screen::init()
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError() ;

	SDL_Surface * raw = SDL_SetVideoMode(m_videomode.width(), m_videomode.height(), m_videomode.depth(), SDL_DOUBLEBUF) ;
	if(!raw)
		throw SDL_GetError() ;

	set(raw) ;
}

Screen::~Screen()
{ }

