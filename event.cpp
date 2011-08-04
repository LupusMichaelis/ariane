
#include "event.hpp"

#include <SDL/SDL.h>
#include <cstring>

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
			m_onkey(ke) ;
		}
		else if(ev.type == SDL_MOUSEBUTTONDOWN
				or ev.type == SDL_MOUSEBUTTONUP)
		{
			MouseClickEvent me
				( Size(ev.button.x, ev.button.y)
				//, ev.type == SDL_MOUSEBUTTONUP ? true : false
				, (ev.button.button == SDL_BUTTON_LEFT) ? true : false << 0 |
				  (ev.button.button == SDL_BUTTON_MIDDLE) ? true : false << 1 |
				  (ev.button.button == SDL_BUTTON_RIGHT) ? true : false << 2
				) ;

			m_onmouseclick(me) ;
		}
		else if(ev.type == SDL_MOUSEMOTION)
		{
			MouseMoveEvent me
				( Size(ev.motion.x, ev.motion.y)
				, (ev.motion.state & SDL_BUTTON(1) ? true : false << 0) |
				  (ev.motion.state & SDL_BUTTON(2) ? true : false << 1) |
				  (ev.motion.state & SDL_BUTTON(3) ? true : false << 2)
				) ;

			m_onmousemove(me) ;
		}
		else if(ev.type == SDL_MOUSEMOTION)
		{
		}
		else if(ev.type == SDL_QUIT)
			running = false ;
	}
	while(running) ;
}

void EventLoop::attach_event(keyboard_event_type::slot_function_type const & fn)
{
	m_onkey.connect(fn) ;
}

void EventLoop::attach_event(mouseclick_event_type::slot_function_type const & fn)
{
	m_onmouseclick.connect(fn) ;
}

void EventLoop::attach_event(mousemove_event_type::slot_function_type const & fn)
{
	m_onmousemove.connect(fn) ;
}
