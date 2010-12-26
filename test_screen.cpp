
#include "screen.hpp"
#include "canvas.hpp"

#include <unistd.h>
#include <iostream>
#include <boost/format.hpp>

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	std::auto_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaff00)) ;

	cout << format("height (%d) width(%d) depth(%d)")
		% height(*p_screen)
		% width(*p_screen)
		% depth(*p_screen)
		<< endl ;

	std::auto_ptr<Canvas> p_s1 ;
	Canvas::create(p_s1, create_videomode(320, 280, 16)) ;
	p_s1->fill(create_color(0x00ff00)) ;
	p_screen->draw(*p_s1) ;

	std::auto_ptr<Canvas> p_s2;
	Canvas::create(p_s2, create_videomode(40, 40, 16)) ;
	p_s2->fill(create_color(0xaa)) ;
	p_screen->draw(*p_s2, Position(20, 20)) ;

	std::auto_ptr<Canvas> p_s3;
	Canvas::create(p_s3, create_videomode(30, 30, 16)) ;
	p_s3->fill(create_color(0xffff00)) ;
	p_screen->draw(*p_s3, Position(10, 10)) ;

	std::auto_ptr<Canvas> p_s4;
	Canvas::create(p_s4, create_videomode(60, 60, 16)) ;
	p_s4->draw(*p_screen) ;
	//p_screen->draw(*p_s4, Position(100, 100)) ;
	p_screen->fill(*p_s4, Position(60, 60), Position(150, 150)) ;

	p_screen->update() ;

	sleep(5) ;
}
