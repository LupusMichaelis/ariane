
#include "screen.hpp"
#include "surface.hpp"

#include <unistd.h>
#include <iostream>
#include <boost/format.hpp>

int main(int argc, char **argv)
{
	Screen screen(320, 280, 16) ;
	screen.init() ;
	//Surface canvas(screen.canvas()) ;
	screen.fill(0xaaff00) ;

	std::cout << boost::format("height (%d) width(%d) depth(%d)")
		% height(screen)
		% width(screen)
		% depth(screen)
		<< std::endl ;

	Surface s1 ;
	screen.create(s1, create_videomode(50, 50, 16)) ;
	s1.init() ;
	s1.fill(0x00ff00) ;
	screen.draw(s1) ;

	Surface s2 ;
	screen.create(s2, create_videomode(30, 30, 16)) ;
	s2.init() ;
	s2.fill(0xffff00) ;
	screen.draw(s2, Position(10, 10)) ;

	screen.update() ;

	sleep(1) ;
}
