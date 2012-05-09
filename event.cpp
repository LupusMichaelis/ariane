
#include "event.hpp"

#include <SDL/SDL.h>
#include <cstring>
#include <map>

class KeyBoard::KBImpl
{
		std::map<unsigned, Key> m_keys ;

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
		}

		Key const & up() const			{ return m_keys.find(SDLK_UP)->second ; }
		Key const & down() const		{ return m_keys.find(SDLK_DOWN)->second ; }
		Key const & right() const		{ return m_keys.find(SDLK_RIGHT)->second ; }
		Key const & left() const		{ return m_keys.find(SDLK_LEFT)->second ; }

		Key const & enter() const		{ return m_keys.find(SDLK_RETURN)->second ; }
		Key const & right_ctrl() const	{ return m_keys.find(SDLK_RCTRL)->second ; }
		Key const & left_ctrl() const	{ return m_keys.find(SDLK_LCTRL)->second ; }
		Key const & right_alt() const	{ return m_keys.find(SDLK_RALT)->second ; }
		Key const & left_alt() const	{ return m_keys.find(SDLK_LALT)->second ; }
		Key const & right_shift() const	{ return m_keys.find(SDLK_RSHIFT)->second ; }
		Key const & left_shift() const	{ return m_keys.find(SDLK_LSHIFT)->second ; }
		Key const & right_meta() const	{ return m_keys.find(SDLK_RMETA)->second ; }
		Key const & left_meta() const	{ return m_keys.find(SDLK_LMETA)->second ; }

} /* class KeyBoard::KBImpl */ ;

KeyBoard::KeyBoard()
	: mp_impl(new KeyBoard::KBImpl)
{
}

Key const & KeyBoard::up()			{ return mp_impl->up() ; }
Key const & KeyBoard::down()		{ return mp_impl->down() ; }
Key const & KeyBoard::right()		{ return mp_impl->right() ; }
Key const & KeyBoard::left()		{ return mp_impl->left() ; }

Key const & KeyBoard::enter()		{ return mp_impl->enter() ; }
Key const & KeyBoard::right_ctrl()	{ return mp_impl->right_ctrl() ; }
Key const & KeyBoard::left_ctrl()	{ return mp_impl->left_ctrl() ; }
Key const & KeyBoard::right_alt()	{ return mp_impl->right_alt() ; }
Key const & KeyBoard::left_alt()	{ return mp_impl->left_alt() ; }
Key const & KeyBoard::right_shift()	{ return mp_impl->right_shift() ; }
Key const & KeyBoard::left_shift()	{ return mp_impl->left_shift() ; }

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
	else if(kev.keysym.sym == SDLK_RETURN)
		pk = &kb.enter() ;
	else
		throw std::logic_error("Unknown key stroke") ;

	return KeyEvent (*pk) ;
}

void EventLoop::operator() () const
{
	SDL_Event ev ;
	std::memset(&ev, 0, sizeof ev) ;

	bool running = true ;
	do
	{
		SDL_WaitEvent(&ev) ;
		if(ev.type == SDL_KEYDOWN or ev.type == SDL_KEYUP)
		{
			KeyEvent ke(KeyEvent_from_sdl(ev.key)) ;
			m_onkeypress(ke) ;
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

			bool down = ev.type == SDL_MOUSEBUTTONDOWN ? true : false ;

			MouseButtonEvent me(Size(ev.button.x, ev.button.y), bs, down) ;

			m_onmousebutton(me) ;
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

			m_onmousemotion(me) ;
		}
		else if(ev.type == SDL_ACTIVEEVENT)
		{
			// get the focus on window
			WindowEvent we ;
		}
		else if(ev.type == SDL_VIDEORESIZE)
		{
			// Resize surface
		}
		else if(ev.type == SDL_VIDEOEXPOSE)
		{
			// redraw surface
		}
		else if(ev.type == SDL_QUIT)
			running = false ;
	}
	while(running) ;
}

void EventLoop::attach_event(keyboard_event_type::slot_function_type const & fn)
{
	m_onkeypress.connect(fn) ;
}

void EventLoop::attach_event(mouse_button_event_type::slot_function_type const & fn)
{
	m_onmousebutton.connect(fn) ;
}

void EventLoop::attach_event(mouse_motion_event_type::slot_function_type const & fn)
{
	m_onmousemotion.connect(fn) ;
}
