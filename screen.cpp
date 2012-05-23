
#include "screen.hpp"
#include "surface.hpp"
#include "videomode.hpp"


#include <SDL/SDL.h>

void create(std::shared_ptr<Screen> & p_surface, VideoMode const & videomode)
{
	std::shared_ptr<Screen> p_new_surface(new Screen(videomode)) ;
	p_new_surface->init() ;
	std::swap(p_surface, p_new_surface) ;
}

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

	SDL_Surface * raw = SDL_SetVideoMode(width(*this), height(*this), depth(*this), SDL_DOUBLEBUF) ;
	if(!raw)
		throw SDL_GetError() ;

	set(raw) ;
}

Screen::~Screen()
{ }

