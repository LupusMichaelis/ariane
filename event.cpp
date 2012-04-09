
#include "event.hpp"

#include <SDL/SDL.h>
#include <cstring>

#include <boost/format.hpp>

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
			KeyEvent ke(ev.key.keysym.sym) ;
			//if(ev.type == SDL_KEYUP)
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
