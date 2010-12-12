
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

	/*
	Surface s ;
	screen.create(s, 20, 20) ;
	*/

	sleep(10) ;
}
