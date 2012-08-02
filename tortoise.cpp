#include "event.hpp"
#include "color.hpp"
#include "gui.hpp"
#include "api.hpp"
#include "widget.hpp"
#include "box.hpp"

#include "tools.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>


#include <boost/format.hpp>
#include <boost/bind.hpp>

class Engine ;
class Surface ;

class Interface
{
	public:
		explicit Interface(Engine & engine)
			: m_engine(engine)
		{ }

		virtual ~Interface() { unlisten_events() ; }

		void move(EventLoop &, KeyEvent const & ke) ;
		void move(EventLoop &, MouseEvent const & me) ;
		void move(EventLoop &, MouseButtonEvent const & me) ;

		void move_left() ;
		void move_right() ;
		void move_up() ;
		void move_down() ;

		void display() ;

	private:
		Engine &					m_engine ;
		Box::SharedPtr				mp_turtle ;

		void listen_events() ;
		void unlisten_events() ;

		// Managed connection, to avoid dandling events if this is deleted
		std::vector<boost::signals2::connection>
									m_cons ;

} /* class Interface */ ;

class Engine
{
	public:
		Engine()
			: m_gui(create_videomode(1024, 768, 16))
		{
		}

		void run() ;

		EventLoop & event_loop()			{ return gui().event_loop() ; }
		Screen & screen()					{ return m_gui.screen() ; }
		Gui & gui()							{ return m_gui ; }
		KeyBoard const & keyboard()	const	{ return m_kb ; }

		void refresh()
		{
			gui().refresh() ;
		}

		void heart_beat(EventLoop &)
		{
			refresh() ;
		}

	private:
		void init_title_screen() ;

		std::unique_ptr<Interface>	mp_interface ;

		KeyBoard					m_kb ;

		Gui 						m_gui ;

} /* class Engine */ ;

#include "screen.hpp"
#include "style.hpp"

void Interface::display()
{
	Screen & screen = m_engine.screen() ;

	Style bg_style = screen.style() ;
	bg_style.color(create_color(0xaaaa00)) ;
	screen.style(bg_style) ;

	Style sprite_style ;
	sprite_style.size(Size {20, 20}) ;
	sprite_style.color(create_color(0x00aa)) ;

	sprite_style.color(create_color(0x00)) ;
	mp_turtle = screen.gui().box(screen, sprite_style) ;

	listen_events() ;
}

void Interface::listen_events()
{
	EventLoop & ev_loop = m_engine.event_loop() ;
	boost::signals2::connection con ;

	void (Interface::*oks)(EventLoop &, KeyEvent const &) = &Interface::move ;
	auto wrapped_oks = boost::bind(oks, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(wrapped_oks)) ;
	m_cons.push_back(con) ;

	void (Interface::*omb)(EventLoop &, MouseEvent const &) = &Interface::move ;
	auto wrapped_omb = boost::bind(omb, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::mouse_motion_event_type::slot_function_type(wrapped_omb)) ;
	m_cons.push_back(con) ;

	void (Interface::*opb)(EventLoop &, MouseButtonEvent const &) = &Interface::move ;
	auto wrapped_opb = boost::bind(opb, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::mouse_button_event_type::slot_function_type(wrapped_opb)) ;
	m_cons.push_back(con) ;
}

void Interface::unlisten_events()
{
	std::for_each(m_cons.begin(), m_cons.end(), std::mem_fun_ref(&boost::signals2::connection::disconnect)) ;
}

void Interface::move(EventLoop &, MouseButtonEvent const & me)
{
	Style turtle_style = mp_turtle->style() ;
	Style bg_style = m_engine.gui().screen().style() ;

	RGBColor turtle_color = turtle_style.color() ;
	RGBColor bg_color = bg_style.color() ;

	turtle_style.color(bg_color) ;
	mp_turtle->style(turtle_style) ;

	bg_style.color(turtle_color) ;
	m_engine.gui().screen().style(bg_style) ;
}

void Interface::move(EventLoop &, MouseEvent const & me)
{
	Style turtle_style = mp_turtle->style() ;
	turtle_style.position(me.position()) ;
	mp_turtle->style(turtle_style) ;
}

void Interface::move(EventLoop &, KeyEvent const & ke)
{
	if(ke.pressing())
		return ;

	KeyBoard const & kb = m_engine.keyboard() ;

	if(ke.key() == kb.up())
		move_up() ;
	else if(ke.key() == kb.down())
		move_down() ;
	else if(ke.key() == kb.right())
		move_right() ;
	else if(ke.key() == kb.left())
		move_left() ;
}

void Interface::move_left()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() - Size(10, 0) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void Interface::move_right()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() + Size(10, 0) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void Interface::move_up()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() - Size(0, 10) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void Interface::move_down()
{
	Style turtle_style = mp_turtle->style() ;
	Size new_position = turtle_style.position() + Size(0, 10) ;
	turtle_style.position(new_position) ;
	mp_turtle->style(turtle_style) ;
}

void Engine::run()
{
	mp_interface = std::make_unique<Interface>(*this) ;
	mp_interface->display() ;

	void (Engine::*oks)(EventLoop &) = &Engine::heart_beat ;
	auto wrapped_oks = boost::bind(oks, this, _1) ;
	auto con = event_loop().attach_event(EventLoop::time_event_type::slot_function_type(wrapped_oks)) ;

	(gui().event_loop())() ;
	con.disconnect() ;
}

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	cout << "Running tortoise\n" ;
	try
	{
		Engine engine ;
		engine.run() ;
	}
	catch(std::exception const & e)
	{
		cout << format("Exception '%s'\n") % e.what() ;
	}
	catch(char * raw)
	{
		cout << format("Exception '%s'\n") % raw ;
	}
	catch(...)
	{
		cout << format("Strange exception raised\n") ;
	}
}

