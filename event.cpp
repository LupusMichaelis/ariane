
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
			if(ev.type == SDL_KEYUP)
				m_onkeyup(ke) ;
			else
				m_onkeydown(ke) ;
		}
		else if(ev.type == SDL_MOUSEBUTTONDOWN
				or ev.type == SDL_MOUSEBUTTONUP)
		{
			MouseEvent me(Size(ev.button.x, ev.button.y)) ;
			if(ev.type == SDL_MOUSEBUTTONUP)
				m_onmouseup(me) ;
			else
				m_onmousedown(me) ;
		}
		else if(ev.type == SDL_QUIT)
			running = false ;
	}
	while(running) ;
}

void EventLoop::attach_event(char const * event, keyboard_event_type::slot_function_type const & fn)
{
	m_onkeyup.connect(fn) ;
}

void EventLoop::attach_event(char const * event, mouse_event_type::slot_function_type const & fn)
{
	m_onmouseup.connect(fn) ;
}
