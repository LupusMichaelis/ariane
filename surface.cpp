
#include "surface.hpp"
#include "screen.hpp"

#include <algorithm>

#include <SDL/SDL.h>

Surface::Surface(VideoMode const & videomode)
	: m_videomode(videomode)
	, mp_raw(0)
{ }

Surface::~Surface()
{ }

VideoMode const & Surface::videomode() const
{ return m_videomode ; }

void * Surface::get() const
{
	return mp_raw ;
}

void Surface::set(void * raw)
{
	mp_raw = raw ;
}

void Surface::fill(RGBColor const & color)
{
	SDL_Surface * p_raw = reinterpret_cast<SDL_Surface *>(get()) ;
	Uint32 mapped_color = SDL_MapRGB(p_raw->format, color.red(), color.green(), color.blue()) ;
	int ret = SDL_FillRect(p_raw, 0, mapped_color) ;
	if(ret == -1)
		throw SDL_GetError() ;
}

#include "canvas.hpp"

void Surface::fill(Surface const & pattern, Position const & from, Position const & to)
{
	Position size(to) ;
	std::auto_ptr<Canvas> p_buffer ;
	Canvas::create(p_buffer, create_videomode(to - from, depth(*this))) ;
	Position next(0, 0) ;
	do
	{
		p_buffer->draw(pattern, next) ;

		next.width(next.width() + width(pattern)) ;
		if(next.width() > to.width())
		{
			next.height(next.height() + height(pattern)) ;
			next.width(0) ;
		}
	} while(next.height() < to.height()) ;

	draw(*p_buffer, from) ;
}

void Surface::draw(Surface const & motif)
{
	draw(motif, Position(0, 0)) ;
}

void Surface::draw(Surface const & motif, Position const & at)
{
	SDL_Rect dst ;
	dst.x = at.width() ;
	dst.y = at.height() ;
	
	SDL_Surface * p_from = reinterpret_cast<SDL_Surface *>(motif.get()) ;
	SDL_Surface * p_to = reinterpret_cast<SDL_Surface *>(mp_raw) ;
	int ret = SDL_BlitSurface(p_from, 0, p_to, &dst) ;
	
	if(ret == -1)
		throw SDL_GetError() ;
	else if(ret == -2)
		throw "Must reload resources" ;
}

void Surface::update() const
{
	SDL_Surface * p_to = reinterpret_cast<SDL_Surface *>(mp_raw) ;
	SDL_UpdateRect(p_to, 0, 0, 0, 0) ;
}

