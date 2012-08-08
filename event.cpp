
#include "event.hpp"
#include "memory.hpp"

#include <SDL/SDL.h>
#include <cstring>
#include <map>
#include <boost/format.hpp>

int const EventLoop::c_time_tick_event = 100 ; // in ms

static Uint32 _heart_beat (Uint32 time, void * )
{
	SDL_UserEvent ue ;
	std::memset(&ue, 0, sizeof ue) ;
	ue.type = SDL_USEREVENT ;
	ue.code = EventLoop::c_time_tick_event ;

	//TimeEvent * te = new TimeEvent ;
	//ue.data1 = te ;

	SDL_Event e ;
	std::memset(&e, 0, sizeof e) ;
	e.user = ue ;

	return SDL_PushEvent(&e) ;
}


class KeyBoard::KBImpl
{
		typedef std::map<unsigned, Key>		key_type ;
		key_type m_keys ;

		Key const & get(unsigned id) const
		{
			key_type::const_iterator found(m_keys.find(id)) ;

			if(found == m_keys.end())
				throw std::logic_error((boost::format("Key id '%d' not found.") % id).str()) ;

			return found->second ;
		}

	public:
		KBImpl()
		{
			m_keys[SDLK_UP] = Key(SDLK_UP) ;
			m_keys[SDLK_DOWN] = Key(SDLK_DOWN) ;
			m_keys[SDLK_RIGHT] = Key(SDLK_RIGHT) ;
			m_keys[SDLK_LEFT] = Key(SDLK_LEFT) ;

			m_keys[SDLK_RETURN] = Key(SDLK_RETURN) ;
			m_keys[SDLK_RCTRL] = Key(SDLK_RCTRL) ;
			m_keys[SDLK_LCTRL] = Key(SDLK_LCTRL) ;
			m_keys[SDLK_RALT] = Key(SDLK_RALT) ;
			m_keys[SDLK_LALT] = Key(SDLK_LALT) ;
			m_keys[SDLK_RSHIFT] = Key(SDLK_RSHIFT) ;
			m_keys[SDLK_LSHIFT] = Key(SDLK_LSHIFT) ;
			m_keys[SDLK_RMETA] = Key(SDLK_RMETA) ;
			m_keys[SDLK_LMETA] = Key(SDLK_LMETA) ;

			m_keys[SDLK_k] = Key(SDLK_k) ;
			m_keys[SDLK_q] = Key(SDLK_q) ;
		}

		Key const & up() const			{ return get(SDLK_UP) ; }
		Key const & down() const		{ return get(SDLK_DOWN) ; }
		Key const & right() const		{ return get(SDLK_RIGHT) ; }
		Key const & left() const		{ return get(SDLK_LEFT) ; }

		Key const & enter() const		{ return get(SDLK_RETURN) ; }
		Key const & right_ctrl() const	{ return get(SDLK_RCTRL) ; }
		Key const & left_ctrl() const	{ return get(SDLK_LCTRL) ; }
		Key const & right_alt() const	{ return get(SDLK_RALT) ; }
		Key const & left_alt() const	{ return get(SDLK_LALT) ; }
		Key const & right_shift() const	{ return get(SDLK_RSHIFT) ; }
		Key const & left_shift() const	{ return get(SDLK_LSHIFT) ; }
		Key const & right_meta() const	{ return get(SDLK_RMETA) ; }
		Key const & left_meta() const	{ return get(SDLK_LMETA) ; }

		Key const & q() const			{ return get(SDLK_q) ; }

} /* class KeyBoard::KBImpl */ ;

KeyBoard::KeyBoard()
	: mp_impl(new KeyBoard::KBImpl)
{
}

Key const & KeyBoard::up() const			{ return mp_impl->up() ; }
Key const & KeyBoard::down() const			{ return mp_impl->down() ; }
Key const & KeyBoard::right() const			{ return mp_impl->right() ; }
Key const & KeyBoard::left() const			{ return mp_impl->left() ; }

Key const & KeyBoard::enter() const			{ return mp_impl->enter() ; }
Key const & KeyBoard::right_ctrl() const	{ return mp_impl->right_ctrl() ; }
Key const & KeyBoard::left_ctrl() const		{ return mp_impl->left_ctrl() ; }
Key const & KeyBoard::right_alt() const		{ return mp_impl->right_alt() ; }
Key const & KeyBoard::left_alt() const		{ return mp_impl->left_alt() ; }
Key const & KeyBoard::right_shift() const	{ return mp_impl->right_shift() ; }
Key const & KeyBoard::left_shift() const	{ return mp_impl->left_shift() ; }

Key const & KeyBoard::q() const				{ return mp_impl->q() ; }

static
KeyEvent const KeyEvent_from_sdl(SDL_KeyboardEvent const & kev)
{
	KeyBoard kb ;
	Key const * pk = 0 ;

	if(kev.keysym.sym == SDLK_UP)
		pk = &kb.up() ;
	else if(kev.keysym.sym == SDLK_DOWN)
		pk = &kb.down() ;
	else if(kev.keysym.sym == SDLK_LEFT)
		pk = &kb.left() ;
	else if(kev.keysym.sym == SDLK_RIGHT)
		pk = &kb.right() ;
	else if(kev.keysym.sym == SDLK_LCTRL)
		pk = &kb.left_ctrl() ;
	else if(kev.keysym.sym == SDLK_RCTRL)
		pk = &kb.right_ctrl() ;
	else if(kev.keysym.sym == SDLK_LALT)
		pk = &kb.left_alt() ;
	else if(kev.keysym.sym == SDLK_RALT)
		pk = &kb.right_alt() ;
	else if(kev.keysym.sym == SDLK_q)
		pk = &kb.q() ;
	else if(kev.keysym.sym == SDLK_RETURN)
		pk = &kb.enter() ;
	else
		throw std::logic_error("Unknown key stroke") ;

	return KeyEvent (*pk, kev.type == SDL_KEYDOWN ? true : false) ;
}

struct EventLoop::Impl
{
	bool					m_running ;

	keyboard_event_type		m_onkeypress ;
	mouse_button_event_type m_onmousebutton ;
	mouse_motion_event_type m_onmousemotion ;

	time_event_type			m_onheartbeat ;

	unsigned				m_heart_pace ;
} /* struct EventLoop::Impl */ ;

EventLoop::EventLoop()
	: mp_impl(std::make_unique<Impl>())
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::operator() ()
{
	SDL_Event ev ;
	std::memset(&ev, 0, sizeof ev) ;

	init_heart(100) ;

	mp_impl->m_running = true ;
	do
	{
		SDL_WaitEvent(&ev) ;
		if(ev.type == SDL_KEYDOWN or ev.type == SDL_KEYUP)
		{
			KeyEvent ke(KeyEvent_from_sdl(ev.key)) ;
			mp_impl->m_onkeypress(*this, ke) ;
		}
		else if(ev.type == SDL_MOUSEBUTTONDOWN
				or ev.type == SDL_MOUSEBUTTONUP)
		{
			MouseEvent::buttons_mask_type bs ;
			bs[0] = ev.button.button == SDL_BUTTON_LEFT ? true : false ;
			bs[1] = ev.button.button == SDL_BUTTON_MIDDLE ? true : false ;
			bs[2] = ev.button.button == SDL_BUTTON_RIGHT ? true : false ;
			bs[3] = ev.button.button == SDL_BUTTON_WHEELUP ? true : false ;
			bs[4] = ev.button.button == SDL_BUTTON_WHEELDOWN ? true : false ;

			bool press = ev.type == SDL_MOUSEBUTTONDOWN ? true : false ;

			MouseButtonEvent me(Size(ev.button.x, ev.button.y), bs, press) ;

			mp_impl->m_onmousebutton(*this, me) ;
		}
		else if(ev.type == SDL_MOUSEMOTION)
		{
			MouseEvent::buttons_mask_type bs ;

			bs[0] = ev.button.button & SDL_BUTTON(SDL_BUTTON_LEFT) ? true : false ;
			bs[1] = ev.button.button & SDL_BUTTON(SDL_BUTTON_MIDDLE) ? true : false ;
			bs[2] = ev.button.button & SDL_BUTTON(SDL_BUTTON_RIGHT) ? true : false ;
			bs[3] = ev.button.button & SDL_BUTTON(SDL_BUTTON_WHEELUP) ? true : false ;
			bs[4] = ev.button.button & SDL_BUTTON(SDL_BUTTON_WHEELDOWN) ? true : false ;

			MouseMotionEvent me(Size(ev.motion.x, ev.motion.y), bs) ;

			mp_impl->m_onmousemotion(*this, me) ;
		}
		else if(ev.type == SDL_ACTIVEEVENT)
		{
			// get the focus on window
			WindowEvent we ;
		}
		else if(ev.type == SDL_VIDEORESIZE)
		{
			// Resized surface
		}
		else if(ev.type == SDL_VIDEOEXPOSE)
		{
			// redraw surface
		}
		else if(ev.type == SDL_USEREVENT)
		{
			SDL_UserEvent * uev = &ev.user ;
			if(uev->code == EventLoop::c_time_tick_event)
			{
				mp_impl->m_onheartbeat(*this) ;
				heart_pace() ;
			}
		}
		else if(ev.type == SDL_QUIT)
			stop() ;
	}
	while(mp_impl->m_running) ;
}


void EventLoop::init_heart(unsigned pace)
{
	mp_impl->m_heart_pace = pace ;
	heart_pace() ;
}

void EventLoop::heart_pace()
{
	unsigned interval = 1000 /* ms */ / mp_impl->m_heart_pace ;

	if(-1 ==SDL_InitSubSystem(SDL_INIT_TIMER))
		throw SDL_GetError() ;
	// XXX 10 : system time granularity
	/* int timer_id = */ SDL_AddTimer((interval / 10 )* 10, _heart_beat, NULL) ;
}

void EventLoop::stop()
{
	mp_impl->m_running = false ;
}

EventLoop::con_type const EventLoop::attach_event(keyboard_event_type::slot_function_type const & fn)
{
	return mp_impl->m_onkeypress.connect(fn) ;
}

EventLoop::con_type const EventLoop::attach_event(mouse_button_event_type::slot_function_type const & fn)
{
	return mp_impl->m_onmousebutton.connect(fn) ;
}

EventLoop::con_type const EventLoop::attach_event(mouse_motion_event_type::slot_function_type const & fn)
{
	return mp_impl->m_onmousemotion.connect(fn) ;
}

EventLoop::con_type const EventLoop::attach_event(time_event_type::slot_function_type const & fn)
{
	return mp_impl->m_onheartbeat.connect(fn) ;
}
