
#include "screen.hpp"
#include "surface.hpp"

#include <unistd.h>
#include <iostream>
#include <boost/format.hpp>

int main(int argc, char **argv)
{
	Screen screen(320, 280, 16) ;
	//Surface canvas(screen.canvas()) ;

	std::cout << boost::format("height (%d) width(%d) depth(%d)")
		% screen.height()
		% screen.width()
		% screen.depth()
		<< std::endl ;

	Surface s ;
	screen.create(s, create_videomode(20, 20, 16)) ;
	s.set_background(0xffff) ;
	screen.draw(s) ;

	screen.update() ;

	sleep(1) ;
}
