#include "menu.hpp"
#include "engine.hpp"
#include "style.hpp"
#include "screen.hpp"
#include "gui.hpp"

void MenuInterface::display()
{
	Screen & screen = engine().gui().screen() ;

	Style containter_style { screen.style() } ;
	containter_style.position({10, 0}) ;
	containter_style.size(containter_style.size() - 2 * containter_style.position() - 2 * containter_style.padding()) ;
	containter_style.color(create_color(0x660000)) ;
	auto p_container = screen.gui().box(screen, containter_style) ;

	{
		auto p_title = screen.gui().text_box(*p_container, title_style()) ;
		p_title->text("Hill quest") ;
	}

	Style entry_style = screen.style() ;
	entry_style.color(create_color(0xaaaaaa)) ;
	{
		Pen pen { Font {"Verdana"}, create_color(0x00aa), 11 };
		entry_style.pen(pen) ;
	}

	entry_style.size(Size {4 * 50, 1 * 50} ) ;

	m_widgets[0] = screen.gui().text_box(*p_container, entry_style) ;
	m_widgets[0]->text("Map builder") ;

	entry_style.color(create_color(0x111111)) ;
	m_widgets[1] = screen.gui().text_box(*p_container, entry_style) ;
	m_widgets[1]->text("Tortoise") ;

	m_widgets[2] = screen.gui().text_box(*p_container, entry_style) ;
	m_widgets[2]->text("Get me") ;

	int pos = 0 ;
	for(auto w: m_widgets)
	{
		Style s { w->style() } ;
		s.position(Size {6 * 50, (3 + pos) * 50 } ) ;
		w->style(s) ;

		++pos ;
	}

	set_container(p_container) ;
}

void MenuInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(ke.pressing())
		return ;

	KeyBoard const & kb = engine().keyboard() ;

	if(ke.key() == kb.up())
		entry_previous() ;
	else if(ke.key() == kb.down())
		entry_next() ;
	else if(ke.key() == kb.enter())
		select() ;
}

void MenuInterface::entry_next()
{
	{
		auto & w = *m_widgets[m_current] ; 
		Style ws { w.style() } ;
		ws.color(create_color(0x111111)) ;
		w.style(ws) ;
	}


	++m_current;
	if(m_current == m_widgets.size())
		m_current = 0 ;

	{
		auto & w = *m_widgets[m_current] ; 
		Style ws { w.style() } ;
		ws.color(create_color(0xaaaaaa)) ;
		w.style(ws) ;
	}
}

void MenuInterface::entry_previous()
{
	{
		auto & w = *m_widgets[m_current] ; 
		Style ws { w.style() } ;
		ws.color(create_color(0x111111)) ;
		w.style(ws) ;
	}

	if(m_current == 0)
		m_current = m_widgets.size() - 1 ;
	else
		--m_current ;

	{
		auto & w = *m_widgets[m_current] ; 
		Style ws { w.style() } ;
		ws.color(create_color(0xaaaaaa)) ;
		w.style(ws) ;
	}
}

void MenuInterface::select()
{
	if(1 == m_current)
		engine().tortoise() ;
	else if(2 == m_current)
		engine().menu() ;
	else if(0 == m_current)
		engine().map_editor() ;
	else
		engine().game_over() ;
}

