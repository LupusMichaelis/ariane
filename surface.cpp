
#include "surface.hpp"
#include "screen.hpp"
#include "color.hpp"
#include "style.hpp"


#include <algorithm>

#include <boost/format.hpp>

#include <SDL/SDL.h>

Surface::Surface(VideoMode const & videomode)
	: m_videomode(videomode)
	, mp_raw(0)
{ }

Surface::~Surface()
{ }

VideoMode const & Surface::videomode() const
{ return m_videomode ; }

void Surface::videomode(VideoMode const & new_videomode)
{ m_videomode = new_videomode ; }

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

void Surface::fill(Surface const & pattern, Size const & from, Size const & to)
{
	Size size(to) ;
	std::shared_ptr<Canvas> p_buffer ;
	Canvas::create(p_buffer, create_videomode(to - from, depth(*this))) ;
	Size next(0, 0) ;
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
	draw(motif, Size(0, 0)) ;
}

void Surface::draw(Surface const & motif, Size const & at)
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

#include <cstring>
// XXX Big trouble here. We have to destroy the SDL_Surface before we can resize the
// screen. This means if we can't recover, the object is in inconsistent state.
// XXX I copy the surface structure before I destroy it. My guess is its undefined
// behaviour. Should look if they are a better (and efficient) way to copy the content of the surface.
void Surface::resize(Size const & new_size)
{
	release() ;
	m_videomode = create_videomode(new_size, m_videomode.depth()) ;
	init() ;
}


void Surface::update() const
{
	SDL_Surface * p_to = reinterpret_cast<SDL_Surface *>(mp_raw) ;
	SDL_UpdateRect(p_to, 0, 0, 0, 0) ;
}

void Surface::dump(std::string const & filename)
{
	SDL_Surface * p = reinterpret_cast<SDL_Surface *>(mp_raw) ;
	int r = SDL_SaveBMP(p, filename.c_str()) ;
	if(r < 0)
		throw SDL_GetError() ;
}

void Surface::crop(Surface & target, Size const & origin, Size const & size) const
{
	SDL_Rect orig ;

	orig.x = origin.width() ;
	orig.y = origin.height() ;
	orig.w = size.width() ;
	orig.h = size.height() ;

	SDL_Surface * p_to = reinterpret_cast<SDL_Surface *>(target.get()) ;
	SDL_Surface * p_from = reinterpret_cast<SDL_Surface *>(mp_raw) ;
	int ret = SDL_BlitSurface(p_from, &orig, p_to, 0) ;

	if(ret == -1)
		throw SDL_GetError() ;
	else if(ret == -2)
		throw "Must reload resources" ;
}

#include <SDL/SDL_ttf.h>

void Surface::write(std::string const & message, Size const & at, Style const & style)
{
	int ret = TTF_Init() ;
	if(ret == -1)
		throw SDL_GetError() ;

	std::string font_name = (boost::format("/usr/share/fonts/truetype/msttcorefonts/%s.ttf")
			% style.font()).str() ;

	TTF_Font * font = 0 ;
	font = TTF_OpenFont(font_name.c_str(), style.size()) ;
	if(!font)
		throw SDL_GetError() ;

	SDL_Surface * p_text = TTF_RenderText_Solid(font, message.c_str(), {0xff, 0xff, 0xff, 0, }) ;
	if(!p_text)
		throw SDL_GetError() ;

	SDL_Rect dst = { (Sint16) at.width(), (Sint16) at.height(), 0, 0, };
	//SDL_Rect box = { 0, 0, (Sint16) size.width(), (Sint16) size.height(), } ;

	SDL_Surface * p_to = reinterpret_cast<SDL_Surface *>(mp_raw) ;

	ret = SDL_BlitSurface(p_text, 0/*&box*/, p_to, &dst) ;
	if(ret == -1)
		throw SDL_GetError() ;

	SDL_FreeSurface(p_text) ;
}
