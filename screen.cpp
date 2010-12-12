
#include "screen.hpp"
#include <SDL/SDL.h>

struct Screen::Impl
{
		Impl(int width, int height, int depth) ;
		int m_width, m_height, m_depth ;

		SDL_Surface * p_surface ;
} ;

Screen::Screen(int width, int height, int depth)
	: mp_impl(new Screen::Impl(width, height, depth))
{ }

int Screen::height() const { return mp_impl->m_height ; }
int Screen::width() const { return mp_impl->m_width ; }
int Screen::depth() const { return mp_impl->m_depth ; }

void Screen::create(Surface & target, int height, int width)
{
	Surface new_surface(*this, height, width) ;
	std::swap(new_surface, target) ;
}

Screen::Impl::Impl(int width, int height, int depth)
	: m_width(width), m_height(height), m_depth(depth)
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError() ;

	p_surface = SDL_SetVideoMode(m_width, m_height, m_depth, SDL_DOUBLEBUF) ;
	if(!p_surface)
		throw SDL_GetError() ;
}

