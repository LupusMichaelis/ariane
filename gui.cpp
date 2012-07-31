
#include "gui.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"

#include "widget.hpp"
#include "box.hpp"
#include "text_box.hpp"
#include "screen.hpp"
#include "grid.hpp"

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

Box * const Gui::box(Style const & set_style)
{
	auto p_tb = std::make_unique<Box>(*this, nullptr) ;
	auto * p = p_tb.get() ;

	p_tb->style(set_style) ;
	m_orphans.push_back(std::move(p_tb)) ;
	return p ;
}

Box * const Gui::box(Widget & parent, Style const & set_style)
{
	auto p_tb = std::make_unique<Box>(*this, &parent) ;
	auto * p = p_tb.get() ;

	p_tb->style(set_style) ;
	parent.adopt(std::move(p_tb)) ;
	return p ;
}

TextBox * const Gui::text_box(Widget & parent, Style const & set_style)
{
	auto p_tb = std::make_unique<TextBox>(*this, &parent) ;
	auto * p = p_tb.get() ;

	p_tb->style(set_style) ;
	parent.adopt(std::move(p_tb)) ;
	return p ;
}

Image * const Gui::image(Widget & parent, Style const & set_style, std::string const & filename)
{
	auto p_i = std::make_unique<Image>(*this, &parent) ;
	p_i->style(set_style) ;
	p_i->filename(filename) ;
	auto * p = p_i.get() ;
	parent.adopt(std::move(p_i)) ;
	p->load() ;
	return p ;
}

	/*
Grid * const Gui::grid(Widget & decorated, Size const & box_size)
{
	//auto p_b = std::make_unique<Grid>(decorated, box_size) ;
	//auto * p = p_b.get() ;

	auto p = new Grid {decorated, box_size} ;
	return p ;
}
	*/

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
