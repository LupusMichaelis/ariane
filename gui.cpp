
#include "gui.hpp"
#include "videomode.hpp"
#include "surface.hpp"

#include "tools.hpp"

#include <SDL/SDL.h>

std::unique_ptr<Surface> Gui::surface(Size const & size) const
{
	VideoMode videomode { size, m_videomode.depth() } ;
	return std::make_unique<SurfaceSDL>(const_cast<Gui &>(*this), std::make_unique<SurfaceMemory>(videomode)) ;
}

std::unique_ptr<Surface> Gui::surface(std::string const & file_name) const
{
	return std::make_unique<SurfaceSDL>(const_cast<Gui &>(*this), std::make_unique<ImageMemory>(file_name)) ;
}

std::unique_ptr<Surface> Gui::surface(Surface const & source) const
{
	auto & sdl_source = dynamic_cast<SurfaceSDL const &>(source) ;
	return std::make_unique<SurfaceSDL>(sdl_source) ;
}

Gui::Gui(VideoMode const & set_videomode)
	: m_videomode(set_videomode)
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
	std::unique_ptr<RawSurfaceMemory> p = std::make_unique<ScreenMemory>(m_videomode) ;
	mp_screen = std::make_unique<SurfaceSDL>(const_cast<Gui &>(*this), std::move(p)) ;
}


