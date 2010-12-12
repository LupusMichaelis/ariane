
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

		SDL_Surface * mp_surface ;
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

void Screen::draw(Surface const & motif)
{
	SDL_Surface * raw = reinterpret_cast<SDL_Surface *>(motif.get()) ;
	int ret = SDL_BlitSurface(raw, 0, mp_impl->mp_surface, 0) ;
	if(ret == -1)
		throw SDL_GetError() ;
	else if(ret == -2)
		throw "Must reload resources" ;
}

void Screen::update() const
{
	SDL_UpdateRect(mp_impl->mp_surface, 0, 0, 0, 0) ;
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

	mp_surface = SDL_SetVideoMode(m_videomode.width(), m_videomode.height(), m_videomode.depth(), SDL_DOUBLEBUF) ;
	if(!mp_surface)
		throw SDL_GetError() ;
}

