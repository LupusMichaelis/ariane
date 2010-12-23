
#include "screen.hpp"
#include "canvas.hpp"

#include <unistd.h>
#include <iostream>
#include <boost/format.hpp>

int main(int argc, char **argv)
{
	std::auto_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(0xaaff00) ;

	std::cout << boost::format("height (%d) width(%d) depth(%d)")
		% height(*p_screen)
		% width(*p_screen)
		% depth(*p_screen)
		<< std::endl ;

	std::auto_ptr<Canvas> p_s1 ;
	create(p_s1, create_videomode(320, 280, 16)) ;
	p_s1->fill(0x00ff00) ;
	p_screen->draw(*p_s1) ;

	std::auto_ptr<Canvas> p_s2;
	create(p_s2, create_videomode(30, 30, 16)) ;
	p_s2->fill(0xffff00) ;
	p_screen->draw(*p_s2, Position(10, 10)) ;

	p_screen->update() ;

	sleep(1) ;
}
