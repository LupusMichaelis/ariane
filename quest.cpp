#include "color.hpp"
#include "style.hpp"
#include "event.hpp"
#include "gui.hpp"
#include "widget.hpp"
#include "api.hpp"
#include "memory.hpp"
#include "box.hpp"
#include "text_box.hpp"
#include "screen.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>

#include <boost/format.hpp>
#include <boost/bind.hpp>

class Surface ;
class Interface ;

#include "interface.hpp"
#include "engine.hpp"
#include "visitor.hpp"

class QuestEngine
	: public Engine
{
	public:
		void menu() ;
		void map_editor() ;
		void tortoise() ;

		virtual
		void first_interface() { menu() ; }
} /* class QuestEngine */ ;

class QuestInterface
	: public Interface
{
	public:
		explicit QuestInterface(QuestEngine & engine)
			: Interface {engine}
		{ }

		QuestEngine & engine() { return static_cast<QuestEngine &>(Interface::engine()) ; }

		virtual ~QuestInterface()
		{
			if(mp_container && has_parent(*mp_container))
				abandon(*mp_container) ;
			mp_container.reset() ;
		}

	protected:
		void set_container(Box::SharedPtr p_container)
		{
			if(mp_container)
				abandon(*mp_container) ;

			std::swap(mp_container, p_container) ;
		}

	private:
		Box::SharedPtr	mp_container ;

} /* QuestInterface */ ;

class TortoiseInterface
	: public QuestInterface
{
	public:
		explicit TortoiseInterface(QuestEngine & engine)
			: QuestInterface {engine}
		{ }

		virtual ~TortoiseInterface() { }

	private:
		void move_left() ;
		void move_right() ;
		void move_up() ;
		void move_down() ;

		virtual
		void move(EventLoop &, KeyEvent const & ke) ;
		virtual
		void move(EventLoop &, MouseEvent const & me) ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & me) ;

		virtual
		void display() ;

	private:
		Box::SharedPtr				mp_turtle ;

} /* class TortoiseInterface */ ;

class MenuInterface
	: public QuestInterface
{
	public:
		explicit MenuInterface(QuestEngine & engine)
			: QuestInterface {engine}
			, m_current(0)
			, m_widgets(3)
		{
		}

		void entry_next() ;
		void entry_previous() ;
		void select() ;

	private:
		virtual
		void move(EventLoop &, KeyEvent const & ke) ;
		virtual
		void move(EventLoop &, MouseEvent const & ) { } ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & ) { } ;

		virtual
		void display() ;

		unsigned					m_current ;
		std::vector<TextBox::SharedPtr>
									m_widgets ;

} /* class MenuInterface */ ;

#include "grid.hpp"
#include "gui_layout.hpp"

class MapEditorInterface
	: public QuestInterface
{
	public: 
		explicit MapEditorInterface(QuestEngine & set_engine)
			: QuestInterface { set_engine }
			, mp_canvas_model { std::make_shared<GridModel>(210, 10, 10) }
			, mp_palette_model { std::make_shared<GridModel>(210, 10, 5) }
			, mp_image { std::move(set_engine.gui().layout().surface("gfx/building.bmp")) }
			, mp_reference { set_engine.gui().layout().grid_surface(*mp_image , Size {32, 32}) }
			, mp_canvas_view { std::make_shared<Grid>(set_engine.gui(), mp_canvas_model, mp_reference) }
			, mp_palette_view { std::make_shared<Grid>(set_engine.gui(), mp_palette_model, mp_reference) }
		{

			mp_canvas_model->set(225, 0, 1) ;

			mp_canvas_model->set(227, 1, 0) ;
			mp_canvas_model->set(226, 1, 1) ;
			mp_canvas_model->set(227, 1, 2) ;

			mp_canvas_model->set(225, 2, 1) ;
		}

		virtual
		void move(EventLoop &, KeyEvent const & ke) ;
		virtual
		void move(EventLoop &, MouseEvent const & me) ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & mbe) ;

		virtual
		void display() ;

	protected:
		GridModel & palette_model()	{ return *mp_palette_model ; }
		GridModel & canvas_model()	{ return *mp_canvas_model ; }
		Grid & palette_view()		{ return static_cast<Grid&>(*mp_palette_view) ; }
		Grid & canvas_view()		{ return static_cast<Grid&>(*mp_canvas_view) ; }

	private:
		GridModel::SharedPtr			mp_canvas_model ;
		GridModel::SharedPtr			mp_palette_model ;
		std::unique_ptr<Surface>		mp_image ;
		GridSurface::SharedPtr			mp_reference ;
		Grid::SharedPtr					mp_canvas_view ;
		Grid::SharedPtr					mp_palette_view ;

} /* class MapEditorInterface */ ;

void QuestEngine::tortoise()
{
	Engine::set_interface<TortoiseInterface, QuestEngine>() ;
}


void QuestEngine::menu()
{
	Engine::set_interface<MenuInterface, QuestEngine>() ;
}

void QuestEngine::map_editor()
{
	Engine::set_interface<MapEditorInterface, QuestEngine>() ;
}

void TortoiseInterface::display()
{
	Screen & screen = engine().gui().screen() ;

	Style bg_style = screen.style() ;
	bg_style.position({10, 10}) ;
	bg_style.size(bg_style.size() - Size {20, 20}) ;
	bg_style.color(create_color(0xaaaa00)) ;
	auto p_container = screen.gui().box(screen, bg_style) ;

	Style sprite_style = screen.style() ;
	sprite_style.size(Size {20, 20}) ;
	sprite_style.color(create_color(0x00aa)) ;

	sprite_style.color(create_color(0x00)) ;
	mp_turtle = screen.gui().box(*p_container, sprite_style) ;

	set_container(p_container) ;
}

void TortoiseInterface::move(EventLoop &, MouseButtonEvent const & me)
{
	Style turtle_style = mp_turtle->style() ;
	Style bg_style = engine().gui().screen().style() ;

	RGBColor turtle_color = turtle_style.color() ;
	RGBColor bg_color = bg_style.color() ;

	turtle_style.color(bg_color) ;
	mp_turtle->style(turtle_style) ;

	bg_style.color(turtle_color) ;
	engine().gui().screen().style(bg_style) ;
}

void TortoiseInterface::move(EventLoop &, MouseEvent const & me)
{
	Style turtle_style = mp_turtle->style() ;
	turtle_style.position(me.position()) ;
	mp_turtle->style(turtle_style) ;
}

void TortoiseInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(ke.pressing())
		return ;

	KeyBoard const & kb = engine().keyboard() ;

	if(ke.key() == kb.up())
		move_up() ;
	else if(ke.key() == kb.down())
		move_down() ;
	else if(ke.key() == kb.right())
		move_right() ;
	else if(ke.key() == kb.left())
		move_left() ;
}

void TortoiseInterface::move_left()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() - Size(10, 0) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void TortoiseInterface::move_right()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() + Size(10, 0) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void TortoiseInterface::move_up()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() - Size(0, 10) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void TortoiseInterface::move_down()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() + Size(0, 10) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

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

void MapEditorInterface::display()
{
	auto s = canvas_view().style() ;
	s.border({create_color(0), 0u}) ;
	s.padding({5, 5}) ;

	/////
	s.size({int(32u * canvas_model().columns()), int(32u * canvas_model().rows())}) ;
	canvas_view().style(s) ;

	/////
	s.size({int(32u * palette_model().columns()), int(32u * palette_model().rows())}) ;
	s.position( {32 + canvas_view().style().size().width(), canvas_view().style().position().height() } ) ;
	palette_view().style(s) ;

	Style bg_style = engine().gui().style() ;
	bg_style.position({0, 0}) ;
	bg_style.size(engine().gui().screen().style().size() - Size {20, 20}) ;
	bg_style.color(create_color(0xaaaaaa)) ;

	auto p_container = engine().gui().box(engine().gui().screen(), bg_style) ;
	adopt(*p_container, canvas_view()) ;
	adopt(*p_container, palette_view()) ;

	set_container(p_container) ;
}

void MapEditorInterface::move(EventLoop &, MouseButtonEvent const & )
{
}

void MapEditorInterface::move(EventLoop &, MouseEvent const & )
{
}

void MapEditorInterface::move(EventLoop &, KeyEvent const & )
{
	mp_canvas_model->set(234, 5, 5) ;
}


int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	cout << "Running quest\n" ;
	try
	{
		QuestEngine engine ;
		engine.run() ;
	}
	catch(std::exception const & e)
	{
		cout << format("Exception '%s'\n") % e.what() ;
	}
	catch(char const * raw)
	{
		cout << format("Exception '%s'\n") % raw ;
	}
	catch(...)
	{
		cout << format("Strange exception raised\n") ;
	}
}
