
#include "gui.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

#include "widget.hpp"
#include "box.hpp"
#include "text_box.hpp"
#include "screen.hpp"

#include "tools.hpp"

#include <SDL/SDL.h>

////////////////////////////////////////////////////////////
Gui::Gui(VideoMode const & set_videomode)
	: mp_layout(std::make_unique<GuiLayout>(set_videomode))
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
	return *mp_layout ;
}
