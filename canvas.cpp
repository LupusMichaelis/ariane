
#include "canvas.hpp"

#include <SDL.h>

void create(std::shared_ptr<Canvas> & p_surface, VideoMode const & videomode)
{
	std::shared_ptr<Canvas> p_new_surface(new Canvas(videomode)) ;
	p_new_surface->init() ;
	std::swap(p_surface, p_new_surface) ;
}

void clone(std::shared_ptr<Canvas> & p_target, Surface const & source)
{
	std::shared_ptr<Canvas> p_new_canvas ;
	create(p_new_canvas, source.videomode()) ;
	p_new_canvas->draw(source) ;
	std::swap(p_target, p_new_canvas) ;
}


void Canvas::release()
	throw()
{
	SDL_Surface * p_raw = reinterpret_cast<SDL_Surface *>(get()) ;
	SDL_FreeSurface(p_raw) ;
}

void Canvas::init()
{
	SDL_Surface * p_raw = SDL_CreateRGBSurface(SDL_SWSURFACE
			, width(*this), height(*this), depth(*this)
			//, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000) ;
			, 0, 0, 0, 0) ;
	if(!p_raw)
		throw SDL_GetError() ;

	set(p_raw) ;
}

Canvas::Canvas()
	: Surface(create_videomode(0, 0, 0))
{
}

Canvas::Canvas(VideoMode const & videomode)
	: Surface(videomode)
{
}

Canvas::~Canvas()
{ }

void Canvas::resize(Size const & new_size)
{
	std::shared_ptr<Canvas> p_original ;
	clone(p_original) ;
	Surface::resize(new_size) ;
	draw(*p_original) ;
	p_original->release() ;
}

void Canvas::clone(std::shared_ptr<Canvas> & p_target)
{
	std::shared_ptr<Canvas> p_new_canvas(new Canvas(videomode())) ;
	p_new_canvas->init() ;
	p_new_canvas->draw(*this) ;
	std::swap(p_target, p_new_canvas) ;
}

