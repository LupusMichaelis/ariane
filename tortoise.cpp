#include "screen.hpp"
#include "canvas.hpp"
#include "event.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>


#include <boost/format.hpp>
#include <boost/bind.hpp>

class Engine
{
	public:
		Engine()
			: m_position(50, 50)
		{
		}

		void move(EventLoop &, KeyEvent const & ke) ;

		void run() ;

		void move_left() ;
		void move_right() ;
		void move_up() ;
		void move_down() ;

	private:
		EventLoop					m_ev_loop ;
		KeyBoard					m_kb ;

		Size						m_position ;
		std::shared_ptr<Screen>		mp_screen ;
		std::shared_ptr<Canvas>		mp_background ;
		std::shared_ptr<Canvas>		mp_bg_sprite ;
		std::shared_ptr<Canvas>		mp_sprite ;

} /* class Engine */ ;

void Engine::run()
{
	Screen::create(mp_screen, create_videomode(320, 240, 16)) ;
	mp_screen->fill(create_color(0xaaaa00)) ;

	auto p_screen_surface = std::dynamic_pointer_cast<Surface>(mp_screen) ;
	clone(mp_background, *p_screen_surface) ;

	Canvas::create(mp_sprite, create_videomode(20, 20, 16)) ;
	mp_sprite->fill(create_color(0x00aa)) ;

	Canvas::create(mp_bg_sprite, create_videomode(20, 20, 16)) ;
	mp_bg_sprite->fill(create_color(0x00)) ;

	mp_screen->draw(*mp_sprite, m_position) ;
	mp_screen->update() ;

	auto fn = boost::bind(&Engine::move, this, _1, _2) ;
	m_ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(fn)) ;
	m_ev_loop() ;
}

void Engine::move(EventLoop &, KeyEvent const & ke)
{
	mp_background->crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;
	mp_screen->draw(*mp_bg_sprite, m_position) ;

	if(ke.key() == m_kb.up())
		move_up() ;
	else if(ke.key() == m_kb.down())
		move_down() ;
	else if(ke.key() == m_kb.right())
		move_right() ;
	else if(ke.key() == m_kb.left())
		move_left() ;
}

void Engine::move_left()
{
	m_position = m_position - Size(10, 0) ;
	mp_screen->draw(*mp_sprite, m_position) ;
	mp_screen->update() ;
}

void Engine::move_right()
{
	m_position = m_position + Size(10, 0) ;
	mp_screen->draw(*mp_sprite, m_position) ;
	mp_screen->update() ;
}

void Engine::move_up()
{
	m_position = m_position - Size(0, 10) ;
	mp_screen->draw(*mp_sprite, m_position) ;
	mp_screen->update() ;
}

void Engine::move_down()
{
	m_position = m_position + Size(0, 10) ;
	mp_screen->draw(*mp_sprite, m_position) ;
	mp_screen->update() ;
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
