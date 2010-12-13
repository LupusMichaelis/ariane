
#include "surface.hpp"
#include "screen.hpp"

#include <SDL.h>
#include <algorithm>

struct Surface::Impl
{
	explicit Impl(Surface & surface)
	: m_surface(surface)
	, mp_surface(0)
	{ }

	virtual
	~Impl()
	{
		m_surface.release() ;
	}
	
	Surface & m_surface ;
	SDL_Surface * mp_surface ;
} ;

void Surface::release()
	throw()
{
	SDL_FreeSurface(mp_impl->mp_surface) ;
}

void Surface::init()
{
	mp_impl->mp_surface = SDL_CreateRGBSurface(SDL_SWSURFACE
			, m_videomode.width(), m_videomode.height(), m_videomode.depth()
			//, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000) ;
			, 0, 0, 0, 0) ;
	if(!mp_impl->mp_surface)
		throw SDL_GetError() ;
}

Surface::Surface()
	: mp_impl(new Impl(*this))
	, m_videomode(create_videomode(0, 0, 0))
{
}

Surface::Surface(VideoMode const & videomode)
	: mp_impl(new Impl(*this))
	, m_videomode(videomode)
{
}

Surface::~Surface()
{
}

void * Surface::get() const
{
	return mp_impl->mp_surface ;
}

void Surface::set(void * raw)
{
	mp_impl->mp_surface = reinterpret_cast<SDL_Surface *>(raw) ;
}

int Surface::width() const { return m_videomode.width() ; }
int Surface::height() const { return m_videomode.height() ; }
int Surface::depth() const { return m_videomode.depth() ; }

void Surface::set_background(unsigned color)
{
	SDL_Surface * raw = reinterpret_cast<SDL_Surface *>(get()) ;
	Uint32 mapped_color = SDL_MapRGB(raw->format, color >> 16, color >> 8, color) ;
	//Uint32 mapped_color = SDL_MapRGB(mp_impl->mp_surface->format, color & 0x000000ff, color & 0x0000ff00, color & 0x00ff0000) ;
	int ret = SDL_FillRect(raw, 0, mapped_color) ;
	if(ret == -1)
		throw SDL_GetError() ;
}

void Surface::create(Surface & target, VideoMode const & videomode)
{
	Surface new_surface(videomode) ;
	//new_surface.init() ;
	// XXX Surface seems to do not be swap-proof
	std::swap(new_surface, target) ;
}

void Surface::draw(Surface const & motif)
{
	draw(motif, Position(0, 0)) ;
}

void Surface::draw(Surface const & motif, Position const & at)
{
	SDL_Surface * raw = reinterpret_cast<SDL_Surface *>(motif.get()) ;
	SDL_Rect dst ;
	dst.x = at.width() ;
	dst.y = at.height() ;
	
	int ret = SDL_BlitSurface(raw, 0, mp_impl->mp_surface, &dst) ;
	
	if(ret == -1)
		throw SDL_GetError() ;
	else if(ret == -2)
		throw "Must reload resources" ;
}

void Surface::update() const
{
	SDL_UpdateRect(mp_impl->mp_surface, 0, 0, 0, 0) ;
}

