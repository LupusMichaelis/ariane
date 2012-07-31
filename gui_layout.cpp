
#include "gui_layout.hpp"
#include "surface.hpp"

#include "tools.hpp"

GuiLayout::GuiLayout(VideoMode const & set_videomode)
	: m_videomode(set_videomode)
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError() ;
}

std::unique_ptr<Surface> GuiLayout::screen()
{
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), m_videomode, true) ;
}

GuiLayout::~GuiLayout()
{
	SDL_Quit() ;
}

std::unique_ptr<Surface> GuiLayout::surface(Size const & size) const
{
	VideoMode videomode { size, m_videomode.depth() } ;
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), videomode) ;
}

std::unique_ptr<Surface> GuiLayout::surface(std::string const & file_name) const
{
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), file_name) ;
}

std::unique_ptr<Surface> GuiLayout::surface(Surface const & source) const
{
	auto & sdl_source = dynamic_cast<SurfaceSDL const &>(source) ;
	return std::make_unique<SurfaceSDL>(sdl_source) ;
}

