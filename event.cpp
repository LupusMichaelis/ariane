
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

static
std::map<unsigned, Key>	const & keys()
{
	static std::map<unsigned, Key> k ;

	if(k.size() == 0)
	{
#	define MAKE_KEY(name) k[name] = Key(name)
		MAKE_KEY(SDLK_TAB) ;
		MAKE_KEY(SDLK_CLEAR) ;
		MAKE_KEY(SDLK_RETURN) ;
		MAKE_KEY(SDLK_PAUSE) ;
		MAKE_KEY(SDLK_ESCAPE) ;
		MAKE_KEY(SDLK_SPACE) ;
		MAKE_KEY(SDLK_EXCLAIM) ;
		MAKE_KEY(SDLK_QUOTEDBL) ;
		MAKE_KEY(SDLK_HASH) ;
		MAKE_KEY(SDLK_DOLLAR) ;
		MAKE_KEY(SDLK_AMPERSAND) ;
		MAKE_KEY(SDLK_QUOTE) ;
		MAKE_KEY(SDLK_LEFTPAREN) ;
		MAKE_KEY(SDLK_RIGHTPAREN) ;
		MAKE_KEY(SDLK_ASTERISK) ;
		MAKE_KEY(SDLK_PLUS) ;
		MAKE_KEY(SDLK_COMMA) ;
		MAKE_KEY(SDLK_MINUS) ;
		MAKE_KEY(SDLK_PERIOD) ;
		MAKE_KEY(SDLK_SLASH) ;
		MAKE_KEY(SDLK_0) ;
		MAKE_KEY(SDLK_1) ;
		MAKE_KEY(SDLK_2) ;
		MAKE_KEY(SDLK_3) ;
		MAKE_KEY(SDLK_4) ;
		MAKE_KEY(SDLK_5) ;
		MAKE_KEY(SDLK_6) ;
		MAKE_KEY(SDLK_7) ;
		MAKE_KEY(SDLK_8) ;
		MAKE_KEY(SDLK_9) ;
		MAKE_KEY(SDLK_COLON) ;
		MAKE_KEY(SDLK_SEMICOLON) ;
		MAKE_KEY(SDLK_LESS) ;
		MAKE_KEY(SDLK_EQUALS) ;
		MAKE_KEY(SDLK_GREATER) ;
		MAKE_KEY(SDLK_QUESTION) ;
		MAKE_KEY(SDLK_AT) ;
		MAKE_KEY(SDLK_LEFTBRACKET) ;
		MAKE_KEY(SDLK_BACKSLASH) ;
		MAKE_KEY(SDLK_RIGHTBRACKET) ;
		MAKE_KEY(SDLK_CARET) ;
		MAKE_KEY(SDLK_UNDERSCORE) ;
		MAKE_KEY(SDLK_BACKQUOTE) ;
		MAKE_KEY(SDLK_a) ;
		MAKE_KEY(SDLK_b) ;
		MAKE_KEY(SDLK_c) ;
		MAKE_KEY(SDLK_d) ;
		MAKE_KEY(SDLK_e) ;
		MAKE_KEY(SDLK_f) ;
		MAKE_KEY(SDLK_g) ;
		MAKE_KEY(SDLK_h) ;
		MAKE_KEY(SDLK_i) ;
		MAKE_KEY(SDLK_j) ;
		MAKE_KEY(SDLK_k) ;
		MAKE_KEY(SDLK_l) ;
		MAKE_KEY(SDLK_m) ;
		MAKE_KEY(SDLK_n) ;
		MAKE_KEY(SDLK_o) ;
		MAKE_KEY(SDLK_p) ;
		MAKE_KEY(SDLK_q) ;
		MAKE_KEY(SDLK_r) ;
		MAKE_KEY(SDLK_s) ;
		MAKE_KEY(SDLK_t) ;
		MAKE_KEY(SDLK_u) ;
		MAKE_KEY(SDLK_v) ;
		MAKE_KEY(SDLK_w) ;
		MAKE_KEY(SDLK_x) ;
		MAKE_KEY(SDLK_y) ;
		MAKE_KEY(SDLK_z) ;
		MAKE_KEY(SDLK_DELETE) ;
		MAKE_KEY(SDLK_KP0) ;
		MAKE_KEY(SDLK_KP1) ;
		MAKE_KEY(SDLK_KP2) ;
		MAKE_KEY(SDLK_KP3) ;
		MAKE_KEY(SDLK_KP4) ;
		MAKE_KEY(SDLK_KP5) ;
		MAKE_KEY(SDLK_KP6) ;
		MAKE_KEY(SDLK_KP7) ;
		MAKE_KEY(SDLK_KP8) ;
		MAKE_KEY(SDLK_KP9) ;
		MAKE_KEY(SDLK_KP_PERIOD) ;
		MAKE_KEY(SDLK_KP_DIVIDE) ;
		MAKE_KEY(SDLK_KP_MULTIPLY) ;
		MAKE_KEY(SDLK_KP_MINUS) ;
		MAKE_KEY(SDLK_KP_PLUS) ;
		MAKE_KEY(SDLK_KP_ENTER) ;
		MAKE_KEY(SDLK_KP_EQUALS) ;
		MAKE_KEY(SDLK_UP) ;
		MAKE_KEY(SDLK_DOWN) ;
		MAKE_KEY(SDLK_RIGHT) ;
		MAKE_KEY(SDLK_LEFT) ;
		MAKE_KEY(SDLK_INSERT) ;
		MAKE_KEY(SDLK_HOME) ;
		MAKE_KEY(SDLK_END) ;
		MAKE_KEY(SDLK_PAGEUP) ;
		MAKE_KEY(SDLK_PAGEDOWN) ;
		MAKE_KEY(SDLK_F1) ;
		MAKE_KEY(SDLK_F2) ;
		MAKE_KEY(SDLK_F3) ;
		MAKE_KEY(SDLK_F4) ;
		MAKE_KEY(SDLK_F5) ;
		MAKE_KEY(SDLK_F6) ;
		MAKE_KEY(SDLK_F7) ;
		MAKE_KEY(SDLK_F8) ;
		MAKE_KEY(SDLK_F9) ;
		MAKE_KEY(SDLK_F10) ;
		MAKE_KEY(SDLK_F11) ;
		MAKE_KEY(SDLK_F12) ;
		MAKE_KEY(SDLK_F13) ;
		MAKE_KEY(SDLK_F14) ;
		MAKE_KEY(SDLK_F15) ;
		MAKE_KEY(SDLK_NUMLOCK) ;
		MAKE_KEY(SDLK_CAPSLOCK) ;
		MAKE_KEY(SDLK_SCROLLOCK) ;
		MAKE_KEY(SDLK_RSHIFT) ;
		MAKE_KEY(SDLK_LSHIFT) ;
		MAKE_KEY(SDLK_RCTRL) ;
		MAKE_KEY(SDLK_LCTRL) ;
		MAKE_KEY(SDLK_RALT) ;
		MAKE_KEY(SDLK_LALT) ;
		MAKE_KEY(SDLK_RMETA) ;
		MAKE_KEY(SDLK_LMETA) ;
		MAKE_KEY(SDLK_LSUPER) ;
		MAKE_KEY(SDLK_RSUPER) ;
		MAKE_KEY(SDLK_MODE) ;
		MAKE_KEY(SDLK_HELP) ;
		MAKE_KEY(SDLK_PRINT) ;
		MAKE_KEY(SDLK_SYSREQ) ;
		MAKE_KEY(SDLK_BREAK) ;
		MAKE_KEY(SDLK_MENU) ;
		MAKE_KEY(SDLK_POWER) ;
		MAKE_KEY(SDLK_EURO) ;
#	undef MAKE_KEY
	}

	return k ;
}

static
Key const & key_from_SDL(unsigned sdl_id)
{
	auto found = keys().find(sdl_id) ;

	if(found == keys().end())
		throw std::logic_error((boost::format("Key id '%d' (%s) not found.") % sdl_id % SDL_GetKeyName((SDLKey)sdl_id)).str()) ;

	return found->second ;
}

struct KeyBoard::Impl
{
	std::map<unsigned, Key> const & m_keys ;
	Impl() : m_keys(keys()) { }
	~Impl() { }

	Key const & get(unsigned sdl_id) const
	{
		return key_from_SDL(sdl_id) ;
	}

} /* struct KeyBoard::Impl */ ;

KeyBoard::KeyBoard()
	: mp_impl(std::make_unique<Impl>())
{
}

KeyBoard::~KeyBoard()
{
}

Key const & KeyBoard::enter() const			{ return mp_impl->get(SDLK_RETURN) ; }
Key const & KeyBoard::right_ctrl() const	{ return mp_impl->get(SDLK_RCTRL) ; }
Key const & KeyBoard::left_ctrl() const		{ return mp_impl->get(SDLK_LCTRL) ; }
Key const & KeyBoard::right_alt() const		{ return mp_impl->get(SDLK_RALT) ; }
Key const & KeyBoard::left_alt() const		{ return mp_impl->get(SDLK_LALT) ; }
Key const & KeyBoard::right_shift() const	{ return mp_impl->get(SDLK_RSHIFT) ; }
Key const & KeyBoard::left_shift() const	{ return mp_impl->get(SDLK_LSHIFT) ; }
Key const & KeyBoard::right_meta() const	{ return mp_impl->get(SDLK_RMETA) ; }
Key const & KeyBoard::left_meta() const		{ return mp_impl->get(SDLK_LMETA) ; }
Key const & KeyBoard::tab() const			{ return mp_impl->get(SDLK_TAB) ; }
Key const & KeyBoard::clear() const			{ return mp_impl->get(SDLK_CLEAR) ; }
Key const & KeyBoard::_return() const		{ return mp_impl->get(SDLK_RETURN) ; }
Key const & KeyBoard::pause() const			{ return mp_impl->get(SDLK_PAUSE) ; }
Key const & KeyBoard::escape() const		{ return mp_impl->get(SDLK_ESCAPE) ; }
Key const & KeyBoard::space() const			{ return mp_impl->get(SDLK_SPACE) ; }
Key const & KeyBoard::exclaim() const		{ return mp_impl->get(SDLK_EXCLAIM) ; }
Key const & KeyBoard::quotedbl() const		{ return mp_impl->get(SDLK_QUOTEDBL) ; }
Key const & KeyBoard::hash() const			{ return mp_impl->get(SDLK_HASH) ; }
Key const & KeyBoard::dollar() const		{ return mp_impl->get(SDLK_DOLLAR) ; }
Key const & KeyBoard::ampersand() const		{ return mp_impl->get(SDLK_AMPERSAND) ; }
Key const & KeyBoard::quote() const			{ return mp_impl->get(SDLK_QUOTE) ; }
Key const & KeyBoard::leftparen() const		{ return mp_impl->get(SDLK_LEFTPAREN) ; }
Key const & KeyBoard::rightparen() const	{ return mp_impl->get(SDLK_RIGHTPAREN) ; }
Key const & KeyBoard::asterisk() const		{ return mp_impl->get(SDLK_ASTERISK) ; }
Key const & KeyBoard::plus() const			{ return mp_impl->get(SDLK_PLUS) ; }
Key const & KeyBoard::comma() const			{ return mp_impl->get(SDLK_COMMA) ; }
Key const & KeyBoard::minus() const			{ return mp_impl->get(SDLK_MINUS) ; }
Key const & KeyBoard::period() const		{ return mp_impl->get(SDLK_PERIOD) ; }
Key const & KeyBoard::slash() const			{ return mp_impl->get(SDLK_SLASH) ; }
Key const & KeyBoard::_0() const			{ return mp_impl->get(SDLK_0) ; }
Key const & KeyBoard::_1() const			{ return mp_impl->get(SDLK_1) ; }
Key const & KeyBoard::_2() const			{ return mp_impl->get(SDLK_2) ; }
Key const & KeyBoard::_3() const			{ return mp_impl->get(SDLK_3) ; }
Key const & KeyBoard::_4() const			{ return mp_impl->get(SDLK_4) ; }
Key const & KeyBoard::_5() const			{ return mp_impl->get(SDLK_5) ; }
Key const & KeyBoard::_6() const			{ return mp_impl->get(SDLK_6) ; }
Key const & KeyBoard::_7() const			{ return mp_impl->get(SDLK_7) ; }
Key const & KeyBoard::_8() const			{ return mp_impl->get(SDLK_8) ; }
Key const & KeyBoard::_9() const			{ return mp_impl->get(SDLK_9) ; }
Key const & KeyBoard::colon() const			{ return mp_impl->get(SDLK_COLON) ; }
Key const & KeyBoard::semicolon() const		{ return mp_impl->get(SDLK_SEMICOLON) ; }
Key const & KeyBoard::less() const			{ return mp_impl->get(SDLK_LESS) ; }
Key const & KeyBoard::equals() const		{ return mp_impl->get(SDLK_EQUALS) ; }
Key const & KeyBoard::greater() const		{ return mp_impl->get(SDLK_GREATER) ; }
Key const & KeyBoard::question() const		{ return mp_impl->get(SDLK_QUESTION) ; }
Key const & KeyBoard::at() const			{ return mp_impl->get(SDLK_AT) ; }
Key const & KeyBoard::leftbracket() const	{ return mp_impl->get(SDLK_LEFTBRACKET) ; }
Key const & KeyBoard::backslash() const		{ return mp_impl->get(SDLK_BACKSLASH) ; }
Key const & KeyBoard::rightbracket() const	{ return mp_impl->get(SDLK_RIGHTBRACKET) ; }
Key const & KeyBoard::caret() const			{ return mp_impl->get(SDLK_CARET) ; }
Key const & KeyBoard::underscore() const	{ return mp_impl->get(SDLK_UNDERSCORE) ; }
Key const & KeyBoard::backquote() const		{ return mp_impl->get(SDLK_BACKQUOTE) ; }
Key const & KeyBoard::a() const				{ return mp_impl->get(SDLK_a) ; }
Key const & KeyBoard::b() const				{ return mp_impl->get(SDLK_b) ; }
Key const & KeyBoard::c() const				{ return mp_impl->get(SDLK_c) ; }
Key const & KeyBoard::d() const				{ return mp_impl->get(SDLK_d) ; }
Key const & KeyBoard::e() const				{ return mp_impl->get(SDLK_e) ; }
Key const & KeyBoard::f() const				{ return mp_impl->get(SDLK_f) ; }
Key const & KeyBoard::g() const				{ return mp_impl->get(SDLK_g) ; }
Key const & KeyBoard::h() const				{ return mp_impl->get(SDLK_h) ; }
Key const & KeyBoard::i() const				{ return mp_impl->get(SDLK_i) ; }
Key const & KeyBoard::j() const				{ return mp_impl->get(SDLK_j) ; }
Key const & KeyBoard::k() const				{ return mp_impl->get(SDLK_k) ; }
Key const & KeyBoard::l() const				{ return mp_impl->get(SDLK_l) ; }
Key const & KeyBoard::m() const				{ return mp_impl->get(SDLK_m) ; }
Key const & KeyBoard::n() const				{ return mp_impl->get(SDLK_n) ; }
Key const & KeyBoard::o() const				{ return mp_impl->get(SDLK_o) ; }
Key const & KeyBoard::p() const				{ return mp_impl->get(SDLK_p) ; }
Key const & KeyBoard::q() const				{ return mp_impl->get(SDLK_q) ; }
Key const & KeyBoard::r() const				{ return mp_impl->get(SDLK_r) ; }
Key const & KeyBoard::s() const				{ return mp_impl->get(SDLK_s) ; }
Key const & KeyBoard::t() const				{ return mp_impl->get(SDLK_t) ; }
Key const & KeyBoard::u() const				{ return mp_impl->get(SDLK_u) ; }
Key const & KeyBoard::v() const				{ return mp_impl->get(SDLK_v) ; }
Key const & KeyBoard::w() const				{ return mp_impl->get(SDLK_w) ; }
Key const & KeyBoard::x() const				{ return mp_impl->get(SDLK_x) ; }
Key const & KeyBoard::y() const				{ return mp_impl->get(SDLK_y) ; }
Key const & KeyBoard::z() const				{ return mp_impl->get(SDLK_z) ; }
Key const & KeyBoard::_delete() const		{ return mp_impl->get(SDLK_DELETE) ; }
Key const & KeyBoard::kp0() const			{ return mp_impl->get(SDLK_KP0) ; }
Key const & KeyBoard::kp1() const			{ return mp_impl->get(SDLK_KP1) ; }
Key const & KeyBoard::kp2() const			{ return mp_impl->get(SDLK_KP2) ; }
Key const & KeyBoard::kp3() const			{ return mp_impl->get(SDLK_KP3) ; }
Key const & KeyBoard::kp4() const			{ return mp_impl->get(SDLK_KP4) ; }
Key const & KeyBoard::kp5() const			{ return mp_impl->get(SDLK_KP5) ; }
Key const & KeyBoard::kp6() const			{ return mp_impl->get(SDLK_KP6) ; }
Key const & KeyBoard::kp7() const			{ return mp_impl->get(SDLK_KP7) ; }
Key const & KeyBoard::kp8() const			{ return mp_impl->get(SDLK_KP8) ; }
Key const & KeyBoard::kp9() const			{ return mp_impl->get(SDLK_KP9) ; }
Key const & KeyBoard::kp_period() const		{ return mp_impl->get(SDLK_KP_PERIOD) ; }
Key const & KeyBoard::kp_divide() const		{ return mp_impl->get(SDLK_KP_DIVIDE) ; }
Key const & KeyBoard::kp_multiply() const	{ return mp_impl->get(SDLK_KP_MULTIPLY) ; }
Key const & KeyBoard::kp_minus() const		{ return mp_impl->get(SDLK_KP_MINUS) ; }
Key const & KeyBoard::kp_plus() const		{ return mp_impl->get(SDLK_KP_PLUS) ; }
Key const & KeyBoard::kp_enter() const		{ return mp_impl->get(SDLK_KP_ENTER) ; }
Key const & KeyBoard::kp_equals() const		{ return mp_impl->get(SDLK_KP_EQUALS) ; }
Key const & KeyBoard::up() const			{ return mp_impl->get(SDLK_UP) ; }
Key const & KeyBoard::down() const			{ return mp_impl->get(SDLK_DOWN) ; }
Key const & KeyBoard::right() const			{ return mp_impl->get(SDLK_RIGHT) ; }
Key const & KeyBoard::left() const			{ return mp_impl->get(SDLK_LEFT) ; }
Key const & KeyBoard::insert() const		{ return mp_impl->get(SDLK_INSERT) ; }
Key const & KeyBoard::home() const			{ return mp_impl->get(SDLK_HOME) ; }
Key const & KeyBoard::end() const			{ return mp_impl->get(SDLK_END) ; }
Key const & KeyBoard::pageup() const		{ return mp_impl->get(SDLK_PAGEUP) ; }
Key const & KeyBoard::pagedown() const		{ return mp_impl->get(SDLK_PAGEDOWN) ; }
Key const & KeyBoard::f1() const			{ return mp_impl->get(SDLK_F1) ; }
Key const & KeyBoard::f2() const			{ return mp_impl->get(SDLK_F2) ; }
Key const & KeyBoard::f3() const			{ return mp_impl->get(SDLK_F3) ; }
Key const & KeyBoard::f4() const			{ return mp_impl->get(SDLK_F4) ; }
Key const & KeyBoard::f5() const			{ return mp_impl->get(SDLK_F5) ; }
Key const & KeyBoard::f6() const			{ return mp_impl->get(SDLK_F6) ; }
Key const & KeyBoard::f7() const			{ return mp_impl->get(SDLK_F7) ; }
Key const & KeyBoard::f8() const			{ return mp_impl->get(SDLK_F8) ; }
Key const & KeyBoard::f9() const			{ return mp_impl->get(SDLK_F9) ; }
Key const & KeyBoard::f10() const			{ return mp_impl->get(SDLK_F10) ; }
Key const & KeyBoard::f11() const			{ return mp_impl->get(SDLK_F11) ; }
Key const & KeyBoard::f12() const			{ return mp_impl->get(SDLK_F12) ; }
Key const & KeyBoard::f13() const			{ return mp_impl->get(SDLK_F13) ; }
Key const & KeyBoard::f14() const			{ return mp_impl->get(SDLK_F14) ; }
Key const & KeyBoard::f15() const			{ return mp_impl->get(SDLK_F15) ; }
Key const & KeyBoard::numlock() const		{ return mp_impl->get(SDLK_NUMLOCK) ; }
Key const & KeyBoard::capslock() const		{ return mp_impl->get(SDLK_CAPSLOCK) ; }
Key const & KeyBoard::scrollock() const		{ return mp_impl->get(SDLK_SCROLLOCK) ; }
Key const & KeyBoard::rshift() const		{ return mp_impl->get(SDLK_RSHIFT) ; }
Key const & KeyBoard::lshift() const		{ return mp_impl->get(SDLK_LSHIFT) ; }
Key const & KeyBoard::rctrl() const			{ return mp_impl->get(SDLK_RCTRL) ; }
Key const & KeyBoard::lctrl() const			{ return mp_impl->get(SDLK_LCTRL) ; }
Key const & KeyBoard::ralt() const			{ return mp_impl->get(SDLK_RALT) ; }
Key const & KeyBoard::lalt() const			{ return mp_impl->get(SDLK_LALT) ; }
Key const & KeyBoard::rmeta() const			{ return mp_impl->get(SDLK_RMETA) ; }
Key const & KeyBoard::lmeta() const			{ return mp_impl->get(SDLK_LMETA) ; }
Key const & KeyBoard::lsuper() const		{ return mp_impl->get(SDLK_LSUPER) ; }
Key const & KeyBoard::rsuper() const		{ return mp_impl->get(SDLK_RSUPER) ; }
Key const & KeyBoard::mode() const			{ return mp_impl->get(SDLK_MODE) ; }
Key const & KeyBoard::help() const			{ return mp_impl->get(SDLK_HELP) ; }
Key const & KeyBoard::print() const			{ return mp_impl->get(SDLK_PRINT) ; }
Key const & KeyBoard::sysreq() const		{ return mp_impl->get(SDLK_SYSREQ) ; }
Key const & KeyBoard::_break() const		{ return mp_impl->get(SDLK_BREAK) ; }
Key const & KeyBoard::menu() const			{ return mp_impl->get(SDLK_MENU) ; }
Key const & KeyBoard::power() const			{ return mp_impl->get(SDLK_POWER) ; }
Key const & KeyBoard::euro() const			{ return mp_impl->get(SDLK_EURO) ; }


static
KeyEvent const KeyEvent_from_sdl(SDL_KeyboardEvent const & kev)
{
	Key const & k = key_from_SDL(kev.keysym.sym) ;
	return KeyEvent (k, kev.type == SDL_KEYDOWN ? true : false) ;
}

struct EventLoop::Impl
{
	bool					m_running ;

	keyboard_event_type		m_onkeypress ;
	mouse_button_event_type m_onmousebutton ;
	mouse_motion_event_type m_onmousemotion ;

	time_event_type			m_onheartbeat ;

	unsigned				m_heart_pace ;

	~Impl() { }
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
