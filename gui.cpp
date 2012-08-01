
#include "gui.hpp"
#include "gui_layout.hpp"

#include "surface.hpp"

#include "visitor.hpp"

#include "widget.hpp"

#include "box.hpp"
#include "text_box.hpp"
#include "screen.hpp"

#include "videomode.hpp"
#include "event.hpp"
#include "style.hpp"

#include "tools.hpp"

struct Gui::Impl
{
	Impl(VideoMode const & set_videomode)
		: mp_layout(std::make_unique<GuiLayout>(set_videomode))
		, m_event_loop()
	{ }

	std::unique_ptr<GuiLayout>	mp_layout ;
	EventLoop					m_event_loop ;

	std::shared_ptr<Screen>		mp_screen ;
} /* struct Gui::Impl */ ;

////////////////////////////////////////////////////////////
Gui::Gui(VideoMode const & set_videomode)
	: mp_impl(std::make_unique<Impl>(set_videomode))
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
	if(!mp_impl->mp_screen)
		mp_impl->mp_screen = Screen::make(*this) ;

	return *mp_impl->mp_screen ;
}

std::shared_ptr<Box> Gui::box(Box & parent, Style const & set_style)
{
	auto p_box = Box::make(*this) ;
	adopt(parent, *p_box) ;
	p_box->style(set_style) ;
	return p_box ;
}

std::shared_ptr<TextBox> Gui::text_box(Box & parent, Style const & set_style)
{
	auto p_tb = TextBox::make(*this) ;
	adopt(parent, *p_tb) ;
	p_tb->style(set_style) ;

	return p_tb ;
}

EventLoop const & Gui::event_loop() const
{
	return const_cast<EventLoop &>(const_cast<Gui &>(*this).event_loop()) ;
}

EventLoop & Gui::event_loop()
{
	return mp_impl->m_event_loop ;
}

Style const Gui::style() const
{
	return Style() ;
}

GuiLayout const & Gui::layout() const
{
	return *mp_impl->mp_layout ;
}

void Gui::refresh()
{
	screen().draw() ;
	screen().surface().update() ;
}
