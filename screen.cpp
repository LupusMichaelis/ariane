
#include "screen.hpp"
#include "surface.hpp"
#include "videomode.hpp"

#include <SDL/SDL.h>

struct Screen::Impl
{
		explicit Impl(VideoMode const & videomode) ;
		Impl(int width, int height, int depth) ;

		~Impl() ;

		void init() ;
		void release() throw() ;

		VideoMode m_videomode ;

		SDL_Surface * p_surface ;
} ;

Screen::Screen(VideoMode const & videomode)
	: mp_impl(new Screen::Impl(videomode))
{ }

Screen::Screen(int width, int height, int depth)
	: mp_impl(new Screen::Impl(width, height, depth))
{ }

int Screen::height() const { return mp_impl->m_videomode.height() ; }
int Screen::width() const { return mp_impl->m_videomode.width() ; }
int Screen::depth() const { return mp_impl->m_videomode.depth() ; }

void Screen::create(Surface & target, VideoMode const & videomode)
{
	Surface new_surface(*this, videomode) ;
	std::swap(new_surface, target) ;
}

Screen::Impl::Impl(VideoMode const & videomode)
	: m_videomode(videomode)
{
	init() ;
}

Screen::Impl::~Impl()
{
	release() ;
}

Screen::Impl::Impl(int width, int height, int depth)
	: m_videomode(create_videomode(width, height, depth))
{
	init() ;
}

void Screen::Impl::release()
	throw()
{
	SDL_Quit() ;
}

void Screen::Impl::init()
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError() ;

	p_surface = SDL_SetVideoMode(m_videomode.width(), m_videomode.height(), m_videomode.depth(), SDL_DOUBLEBUF) ;
	if(!p_surface)
		throw SDL_GetError() ;
}

