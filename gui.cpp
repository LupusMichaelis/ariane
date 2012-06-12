
#include "gui.hpp"
#include "videomode.hpp"
#include "surface.hpp"
#include "widget.hpp"

#include "tools.hpp"

#include <SDL/SDL.h>

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

////////////////////////////////////////////////////////////
Gui::Gui(VideoMode const & set_videomode)
	: m_layout(set_videomode)
	, m_event_loop()
{
}

Gui::~Gui()
{
}

Screen const & Gui::screen() const
{
	return const_cast<Screen const &>(const_cast<Gui &>(*this).screen()) ;
}

Screen & Gui::screen()
{
	if(!mp_screen)
		mp_screen = std::make_unique<Screen>(*this) ;

	return *mp_screen ;
}

TextBox * const Gui::text_box(Widget & parent, Style const & set_style)
{
	auto p_tb = std::make_unique<TextBox>(*this, &parent) ;
	auto * p = p_tb.get() ;

	p_tb->style(set_style) ;
	parent.adopt(std::move(p_tb)) ;
	return p ;
}

EventLoop const & Gui::event_loop() const
{
	return const_cast<EventLoop &>(const_cast<Gui &>(*this).event_loop()) ;
}

EventLoop & Gui::event_loop()
{
	return m_event_loop ;
}

Style const Gui::style() const
{
	return Style() ;
}

GuiLayout & Gui::layout()
{
	return m_layout ;
}
