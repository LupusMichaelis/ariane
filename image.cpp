
#include "image.hpp"

#include <SDL.h>

void Image::release()
	throw()
{
	SDL_Surface * p_raw = reinterpret_cast<SDL_Surface *>(get()) ;
	SDL_FreeSurface(p_raw) ;
}

void Image::init()
{
	SDL_Surface * p_raw = SDL_LoadBMP(m_filename.c_str()) ;
	if(!p_raw)
		throw SDL_GetError() ;

	videomode(create_videomode(p_raw->w, p_raw->h, 0)) ;

	set(p_raw) ;
}

Image::Image()
	: Surface(create_videomode(0, 0, 0))
	, m_filename()
{
}

Image::Image(std::string const & filename)
	: Surface(create_videomode(0, 0, 0))
	, m_filename(filename)
{
}

Image::~Image()
{ }

