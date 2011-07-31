
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
			if(ev.type == SDL_KEYDOWN)
				m_onkeydown(ke) ;
			else
				m_onkeyup(ke) ;
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
