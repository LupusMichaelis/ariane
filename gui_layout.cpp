
#include "gui_layout.hpp"
#include "surface.hpp"
#include "grid_surface.hpp"
#include "font_manager.hpp"
#include "text_surface.hpp"
#include "gui.hpp"

#include "memory.hpp"

struct GuiLayout::Impl
{
	Impl(GuiLayout & gui_layout, VideoMode const & videomode)
		: m_videomode(videomode)
		, m_font_manager(gui_layout, {})
	{ }

	VideoMode					m_videomode ;
	FontManager					m_font_manager ;
} /* struct GuiLayout::Impl */ ;

GuiLayout::GuiLayout(VideoMode const & set_videomode)
	: mp_impl(std::make_unique<Impl>(*this, set_videomode))
{
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		throw SDL_GetError() ;
	SDL_ShowCursor(SDL_DISABLE) ;
}

std::unique_ptr<Surface> GuiLayout::screen() const
{
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), videomode(), true) ;
}

GuiLayout::~GuiLayout()
{
	SDL_Quit() ;
}

VideoMode const GuiLayout::videomode() const
{
	return mp_impl->m_videomode ;
}

std::unique_ptr<Surface> GuiLayout::surface(Size const & size) const
{
	VideoMode v { size, videomode().depth() } ;
	return std::make_unique<SurfaceSDL>(const_cast<GuiLayout &>(*this), v) ;
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

std::unique_ptr<GridSurface> GuiLayout::grid_surface(Surface & reference, Size const & sprite_size) const
{
	return std::make_unique<GridSurface>(reference, sprite_size) ;
}

FontManager const & GuiLayout::fonts() const
{
	return const_cast<FontManager &>(const_cast<GuiLayout &>(*this).fonts()) ;
}

FontManager & GuiLayout::fonts()
{
	return mp_impl->m_font_manager ;
}

std::unique_ptr<TextSurface> GuiLayout::text(std::string const & content, Pen const & pen, Size const & size)
{
	return std::make_unique<TextSurfaceSDL>(*this
			, create_videomode(size, videomode().depth())
			, content
			, pen
		) ;
}
