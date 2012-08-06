
#include "engine.hpp"
#include "event.hpp"
#include "interface.hpp"
#include "style.hpp"

// XXX
#include "text_box.hpp"
#include "screen.hpp"

#include "tools.hpp"

struct Engine::Impl
{
	Impl() : m_gui(create_videomode(640, 480, 16)) { }

	std::unique_ptr<Interface>	mp_interface ;
	KeyBoard					m_kb ;
	Gui 						m_gui ;

} /* struct Engine::Impl */ ;

Engine::Engine()
	: mp_impl { std::make_unique<Impl>() }
{
}

Engine::~Engine()
{
}

Gui & Engine::gui()
{
	return const_cast<Gui &>(const_cast<Engine const &>(*this).gui()) ;
}

Gui const & Engine::gui() const
{
	return mp_impl->m_gui ;
}

KeyBoard const & Engine::keyboard() const
{
	return mp_impl->m_kb ;
}

void Engine::heart_beat(EventLoop &) 
{
	gui().refresh() ;
}

void Engine::run()
{
	first_interface() ;

	EventLoop & ev_loop = gui().event_loop() ;
	void (Engine::*oks)(EventLoop &) = &Engine::heart_beat ;
	auto wrapped_oks = boost::bind(oks, this, _1) ;
	auto con = ev_loop.attach_event(EventLoop::time_event_type::slot_function_type(wrapped_oks)) ;

	ev_loop() ;
	con.disconnect() ;
}

void Engine::game_over()
{
	Pen pen(Font {"Verdana", 30}, create_color(0x00bbbb), 30) ;
	Style style {pen, create_color(0), {10, 10}, {1, 1}, {100, 50} } ;

	TextBox::SharedPtr p_b = gui().text_box(gui().screen(), style) ;
	p_b->text("Looser!") ;

	gui().refresh() ;
	sleep(2) ;
	gui().event_loop().stop() ;
}

void Engine::set_interface(std::unique_ptr<Interface> p_interface)
{
	std::swap(mp_impl->mp_interface, p_interface) ;
	mp_impl->mp_interface->display() ;
}

