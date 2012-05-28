#include "event.hpp"
#include "color.hpp"
#include "gui.hpp"
#include "api.hpp"

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
			, m_position(50, 50)
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

		std::unique_ptr<Surface>	mp_background ;
		std::unique_ptr<Surface>	mp_bg_sprite ;
		std::unique_ptr<Surface>	mp_sprite ;

		Size						m_position ;

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
			: m_gui(create_videomode(320, 240, 16))
		{
		}

		void run() ;

		EventLoop & event_loop()			{ return m_ev_loop ; }
		Surface & screen()					{ return m_gui.screen() ; }
		Gui & gui()							{ return m_gui ; }
		KeyBoard const & keyboard()	const	{ return m_kb ; }

	private:
		void init_title_screen() ;

		std::unique_ptr<Interface>	mp_interface ;

		EventLoop					m_ev_loop ;
		KeyBoard					m_kb ;

		Gui 						m_gui ;

} /* class Engine */ ;

void Interface::display()
{
	Surface & screen = m_engine.screen() ;
	screen.fill(create_color(0xaaaa00)) ;

	mp_background = m_engine.gui().surface(screen) ;

	mp_sprite = screen.gui().surface(Size {20, 20}) ;
	mp_sprite->fill(create_color(0x00aa)) ;

	mp_bg_sprite = screen.gui().surface(Size {20, 20}) ;
	mp_bg_sprite->fill(create_color(0x00)) ;

	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;

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
	if(me.pressing())
		return ;

	Surface & screen = m_engine.screen() ;

	mp_background->crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;
	screen.draw(*mp_bg_sprite, m_position) ;

	m_position = me.position() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Interface::move(EventLoop &, MouseEvent const & me)
{
	mp_background->crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;

	Surface & screen = m_engine.screen() ;
	screen.draw(*mp_bg_sprite, m_position) ;

	m_position = me.position() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Interface::move(EventLoop &, KeyEvent const & ke)
{
	mp_background->crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;

	Surface & screen = m_engine.screen() ;
	screen.draw(*mp_bg_sprite, m_position) ;

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
	m_position = m_position - Size(10, 0) ;

	Surface & screen = m_engine.screen() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Interface::move_right()
{
	m_position = m_position + Size(10, 0) ;

	Surface & screen = m_engine.screen() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Interface::move_up()
{
	m_position = m_position - Size(0, 10) ;

	Surface & screen = m_engine.screen() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Interface::move_down()
{
	m_position = m_position + Size(0, 10) ;

	Surface & screen = m_engine.screen() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Engine::run()
{
	mp_interface = std::make_unique<Interface>(*this) ;
	mp_interface->display() ;

	m_ev_loop() ;
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

