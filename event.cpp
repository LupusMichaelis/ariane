
#include "event.hpp"

#include <SDL/SDL.h>

void EventLoop::operator() () const
{
	SDL_Event ev ;
	bool running = true ;
	do
	{
		SDL_WaitEvent(&ev) ;
		KeyEvent ke(ev.key.keysym.sym) ;
		switch(ev.type)
		{
			case SDL_KEYDOWN:
				m_onkeydown(ke) ;
				break ;

			case SDL_KEYUP:
				m_onkeyup(ke) ;
				/*
				switch(ev.key.keysym.sym)
				{
					case SDLK_LEFT: std::cout << "left\n" ; break ;
					case SDLK_RIGHT: std::cout << "right\n" ; break ;
					case SDLK_UP: std::cout << "up\n" ; break ;
					case SDLK_DOWN: std::cout << "down\n" ; break ;
					default: std::cout << "other\n" ;
				}
				*/
				break ;

			case SDL_QUIT:
				running = false ;
				break ;

			default:
				break ;
		}
	}
	while(running) ;
}

void EventLoop::attach_event(char const * event, keyboard_event_type::slot_function_type const & fn)
{
	m_onkeyup.connect(fn) ;
}
