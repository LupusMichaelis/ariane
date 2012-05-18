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

template<typename T>
T & get_or_throw(std::shared_ptr<T> const & sptr)
{
	if(!sptr) throw std::bad_alloc() ;
	return *sptr ;
}

class Engine ;

class Interface
{
	public:
		explicit Interface(Engine & engine)
			: m_engine(engine)
		{ }

		virtual ~Interface() { detach_events() ; }

		void background(std::shared_ptr<Canvas> const & new_background)
								{ mp_background = new_background ; }
		Canvas & background()	{ return get_or_throw(mp_background) ; }
		Engine & engine()		{ return m_engine ; }

		void display() ;

	protected:
		template <typename FunPtrT, typename ClassT, typename SlotFunT>
		void attach_event(FunPtrT const fun_ptr) ;
		void detach_events() ;

		virtual
		void init_display() = 0 ;
		virtual
		void listen_events() = 0 ;

	private:
		Engine &					m_engine ;

		std::shared_ptr<Canvas>		mp_background ;

		// Managed connection, to avoid dandling events if this is deleted
		std::vector<boost::signals::connection>
								m_cons ;

} /* class Interface */ ;

class MenuInterface
	: public Interface
{
	public:
		explicit MenuInterface(Engine & engine)
			: Interface(engine)
			, m_position(50, 50)
		{ }

		void move(EventLoop &, KeyEvent const & ke) ;
		void move(EventLoop &, MouseEvent const & me) ;
		void move(EventLoop &, MouseButtonEvent const & me) ;

		void entry_next() ;
		void entry_previous() ;

	private:
		virtual
		void init_display() ;
		virtual
		void listen_events() ;

		std::shared_ptr<Canvas>		mp_bg_sprite ;
		std::shared_ptr<Canvas>		mp_sprite ;

		Size						m_position ;

} /* class MenuInterface */ ;

class Engine
{
	public:
		Engine()
		{
		}

		void run() ;

		EventLoop & event_loop()			{ return m_ev_loop ; }
		Screen & screen()					{ return *mp_screen ; }
		KeyBoard const & keyboard()	const	{ return m_kb ; }

	private:
		void init_title_screen() ;

		std::shared_ptr<Interface>	mp_interface ;

		EventLoop					m_ev_loop ;
		KeyBoard					m_kb ;

		std::shared_ptr<Screen>		mp_screen ;

} /* class Engine */ ;

template <typename FunPtrT, typename ClassT, typename SlotFunT>
void Interface::attach_event(FunPtrT const fun_ptr)
{
	auto wrapped_fun_ptr = boost::bind(fun_ptr, static_cast<ClassT * const>(this), _1, _2) ;
	EventLoop & ev_loop = engine().event_loop() ;
	boost::signals::connection con ;
	con = ev_loop.attach_event(SlotFunT(wrapped_fun_ptr)) ;
	m_cons.push_back(con) ;
}

void Interface::display()
{
	init_display() ;
	listen_events() ;
}

void Interface::detach_events()
{
	std::for_each(m_cons.begin(), m_cons.end(), std::mem_fun_ref(&boost::signals::connection::disconnect)) ;
}

void MenuInterface::init_display()
{
	Screen & screen = engine().screen() ;
	screen.fill(create_color(0xaaaa00)) ;

	Surface & screen_surface = dynamic_cast<Surface &>(screen) ;
	std::shared_ptr<Canvas> new_background ;
	clone(new_background, screen_surface) ;
	background(new_background) ;

	Canvas::create(mp_sprite, create_videomode(20, 20, 16)) ;
	mp_sprite->fill(create_color(0x00aa)) ;

	Canvas::create(mp_bg_sprite, create_videomode(20, 20, 16)) ;
	mp_bg_sprite->fill(create_color(0x00)) ;

	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void MenuInterface::listen_events()
{
	// This is complex, I should be able to make something simpler (automatic FunPtr
	// signature, for example)
	attach_event<void (MenuInterface::*)(EventLoop &, KeyEvent const &)
		, MenuInterface
		, EventLoop::keyboard_event_type::slot_function_type
		>(&MenuInterface::move) ;
	attach_event<void (MenuInterface::*)(EventLoop &, MouseEvent const &)
		, MenuInterface
		, EventLoop::mouse_motion_event_type::slot_function_type
		>(&MenuInterface::move) ;
	attach_event<void (MenuInterface::*)(EventLoop &, MouseButtonEvent const &)
		, MenuInterface
		, EventLoop::mouse_button_event_type::slot_function_type
		>(&MenuInterface::move) ;
}

void MenuInterface::move(EventLoop &, MouseButtonEvent const & me)
{
	if(me.pressing())
		return ;

	Screen & screen = engine().screen() ;

	background().crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;
	screen.draw(*mp_bg_sprite, m_position) ;

	m_position = me.position() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void MenuInterface::move(EventLoop &, MouseEvent const & me)
{
	background().crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;

	Screen & screen = engine().screen() ;
	screen.draw(*mp_bg_sprite, m_position) ;

	m_position = me.position() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void MenuInterface::move(EventLoop &, KeyEvent const & ke)
{
	background().crop(*mp_bg_sprite, m_position, mp_sprite->videomode().size()) ;

	Screen & screen = engine().screen() ;
	screen.draw(*mp_bg_sprite, m_position) ;

	if(ke.pressing())
		return ;

	KeyBoard const & kb = engine().keyboard() ;

	if(ke.key() == kb.up())
		entry_next() ;
	else if(ke.key() == kb.down())
		entry_previous() ;
	/*
	else if(ke.key() == kb.right())
		move_right() ;
	else if(ke.key() == kb.left())
		move_left() ;
	*/
}

void MenuInterface::entry_next()
{
	m_position = m_position - Size(0, 10) ;

	Screen & screen = engine().screen() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void MenuInterface::entry_previous()
{
	m_position = m_position + Size(0, 10) ;

	Screen & screen = engine().screen() ;
	screen.draw(*mp_sprite, m_position) ;
	screen.update() ;
}

void Engine::run()
{
	Screen::create(mp_screen, create_videomode(320, 240, 16)) ;
	{
		std::shared_ptr<MenuInterface>	p_interface(new MenuInterface(*this)) ;
		mp_interface = p_interface ;
	}
	mp_interface->display() ;

	m_ev_loop() ;
}

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	cout << "Running quest\n" ;
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
