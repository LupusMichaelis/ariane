
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
	SurfaceSDL::impl_ptr p = std::make_unique<ScreenMemory, SurfaceSDL::impl_ptr::deleter_type>(m_videomode) ;
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), std::move(p)) ;
}

GuiLayout::~GuiLayout()
{
	SDL_Quit() ;
}

std::unique_ptr<Surface> GuiLayout::surface(Size const & size) const
{
	VideoMode videomode { size, m_videomode.depth() } ;
	auto p_impl = std::make_unique<SurfaceMemory, SurfaceSDL::impl_ptr::deleter_type>(videomode) ;
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), std::move(p_impl)) ;
}

std::unique_ptr<Surface> GuiLayout::surface(std::string const & file_name) const
{
	auto p_impl = std::make_unique<ImageMemory, SurfaceSDL::impl_ptr::deleter_type>(file_name) ;
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), std::move(p_impl)) ;
}

std::unique_ptr<Surface> GuiLayout::surface(Surface const & source) const
{
	auto & sdl_source = dynamic_cast<SurfaceSDL const &>(source) ;
	return std::make_unique<SurfaceSDL>(sdl_source) ;
}

