
#include "surface.hpp"

#include <SDL.h>

struct Surface::Impl
{
		Impl(Screen const & screen, VideoMode const & videomode)
		: m_screen(screen), m_videomode(videomode)
		{
			init() ;
		}
		void init() ;

		Screen const & m_screen ;
		VideoMode m_videomode ;

		SDL_Surface * mp_surface ;
} ;

void Surface::Impl::init()
{
	mp_surface = SDL_CreateRGBSurface(SDL_SWSURFACE
			, m_videomode.width(), m_videomode.height(), m_videomode.depth()
			, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000) ;
	if(!mp_surface)
		throw SDL_GetError() ;
}

Surface::Surface(Screen const & screen, VideoMode const & videomode)
	: mp_impl(new Surface::Impl(screen, videomode))
{ }

Surface::Surface()
	: mp_impl()
{ }

void * Surface::get() const
{
	return mp_impl->mp_surface ;
}

int Surface::width() const { return mp_impl->m_videomode.width() ; }
int Surface::height() const { return mp_impl->m_videomode.height() ; }
int Surface::depth() const { return mp_impl->m_videomode.depth() ; }

void Surface::set_background(unsigned color)
{
	Uint32 mapped_color = SDL_MapRGB(mp_impl->mp_surface->format, color & 0x000000ff, color & 0x0000ff00, color & 0x00ff0000) ;
	int ret = SDL_FillRect(mp_impl->mp_surface, 0, mapped_color) ;
	if(ret == -1)
		throw SDL_GetError() ;
	SDL_UpdateRect(mp_impl->mp_surface, 0, 0, width(), height()) ;
}

