
#include "interface.hpp"
#include "engine.hpp"
#include "tools.hpp"

struct Interface::Impl
{
	Impl(Engine & engine)
		: m_engine(engine)
		, m_cons()
	{ }

	Engine &							m_engine ;

	// Managed connection, to avoid dandling events if this is deleted
	std::vector<EventLoop::con_type>	m_cons ;

} /* struct Interface::Impl */ ;

Interface::Interface(Engine & engine)
	: mp_impl {std::make_unique<Impl>(engine)}
{
	listen_events() ;
}

Interface::~Interface()
{
	unlisten_events() ;
}

Engine & Interface::engine()
{
	return mp_impl->m_engine ;
}

void Interface::unlisten_events()
{
	std::for_each(cons().begin(), cons().end(), std::mem_fun_ref(&EventLoop::con_type::disconnect)) ;
}

std::vector<EventLoop::con_type> & Interface::cons()
{
	return mp_impl->m_cons ;
}

void Interface::listen_events()
{
	EventLoop & ev_loop = engine().gui().event_loop() ;
	boost::signals2::connection con ;

	void (Interface::*oks)(EventLoop &, KeyEvent const &) = &Interface::move ;
	auto wrapped_oks = boost::bind(oks, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(wrapped_oks)) ;
	cons().push_back(con) ;

	void (Interface::*omb)(EventLoop &, MouseEvent const &) = &Interface::move ;
	auto wrapped_omb = boost::bind(omb, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::mouse_motion_event_type::slot_function_type(wrapped_omb)) ;
	cons().push_back(con) ;

	void (Interface::*opb)(EventLoop &, MouseButtonEvent const &) = &Interface::move ;
	auto wrapped_opb = boost::bind(opb, this, _1, _2) ;
	con = ev_loop.attach_event(EventLoop::mouse_button_event_type::slot_function_type(wrapped_opb)) ;
	cons().push_back(con) ;
}

