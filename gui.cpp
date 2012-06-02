
#include "gui.hpp"
#include "videomode.hpp"
#include "surface.hpp"

#include "tools.hpp"

#include <SDL/SDL.h>

std::unique_ptr<Surface> Gui::surface(Size const & size) const
{
	VideoMode videomode { size, m_videomode.depth() } ;
	auto p_impl = std::make_unique<SurfaceMemory, SurfaceSDL::impl_ptr::deleter_type>(videomode) ;
	return std::make_unique<SurfaceSDL>(const_cast<Gui &>(*this), std::move(p_impl)) ;
}

std::unique_ptr<Surface> Gui::surface(std::string const & file_name) const
{
	auto p_impl = std::make_unique<ImageMemory, SurfaceSDL::impl_ptr::deleter_type>(file_name) ;
	return std::make_unique<SurfaceSDL>(const_cast<Gui &>(*this), std::move(p_impl)) ;
}

std::unique_ptr<Surface> Gui::surface(Surface const & source) const
{
	auto & sdl_source = dynamic_cast<SurfaceSDL const &>(source) ;
	return std::make_unique<SurfaceSDL>(sdl_source) ;
}

Gui::Gui(VideoMode const & set_videomode)
	: mp_screen()
	, m_videomode(set_videomode)
	, m_event_loop()
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError() ;
}

Gui::~Gui()
{
	SDL_Quit() ;
}

Surface & Gui::screen()
{
	if (!mp_screen)
		init_screen() ;

	return * mp_screen ;
}

Surface const & Gui::screen() const
{
	return const_cast<Surface &>(const_cast<Gui &>(*this).screen()) ;
}

void Gui::init_screen() const
{
	SurfaceSDL::impl_ptr p = std::make_unique<ScreenMemory, SurfaceSDL::impl_ptr::deleter_type>(m_videomode) ;
	mp_screen = std::make_unique<SurfaceSDL>(const_cast<Gui &>(*this), std::move(p)) ;
}


EventLoop const & Gui::event_loop() const
{
	return const_cast<EventLoop &>(const_cast<Gui &>(*this).event_loop()) ;
}

EventLoop & Gui::event_loop()
{
	return m_event_loop ;
}
