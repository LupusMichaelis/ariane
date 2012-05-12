#include "screen.hpp"
#include "canvas.hpp"
#include "event.hpp"

#include <unistd.h>
#include <cassert>

#include <memory>
#include <iostream>
#include <boost/format.hpp>

#include <cstring>

void tortoise() ;

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	cout << "Running tortoise\n" ;
	try
	{
		tortoise() ;
	}
	catch(std::exception const & e)
	{
		cout << format("Exception '%s'\n") % e.what() ;
	}
	catch(char * raw)
	{
		cout << format("Exception '%s'\n") % raw ;
	}
	catch(...)
	{
		cout << format("Strange exception raised\n") ;
	}
}

void tortoise()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 240, 24)) ;

	EventLoop ev_loop ;

	//ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(print_key_event)) ;
	ev_loop() ;
}


