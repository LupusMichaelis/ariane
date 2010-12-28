
#include "screen.hpp"
#include "canvas.hpp"

#include <unistd.h>
#include <cassert>

#include <iostream>
#include <boost/format.hpp>
using std::cout ;
using std::endl ;
using boost::format ;

void test_base() ;
void test_resize() ;
void test_compose() ;

int main(int argc, char **argv)
{
	//test_base() ;
	test_resize() ;
	//test_compose() ;
}

void test_base()
{
	std::auto_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaff00)) ;

	assert(width(*p_screen) == 320) ;
	assert(height(*p_screen) == 280) ;
	assert(depth(*p_screen) == 16) ;

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

	p_screen->dump(std::string(".test_screen.bmp")) ;

	sleep(5) ;
}

#include "image.hpp"

void test_compose()
{
	std::auto_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;

	std::auto_ptr<Image> p_images ;
	std::string filename("gfx/kraland_shapes.bmp") ;
	Image::create(p_images, filename) ;

	/*
	cout << format("width(%d) height(%d) depth(%d)")
		% width(*p_images)
		% height(*p_images)
		% depth(*p_images)
		<< endl ;
	*/

	assert(width(*p_images) == 672) ;
	assert(height(*p_images) == 480) ;
	assert(depth(*p_images) == 24) ;

	/*
	p_screen->resize(p_images) ;

	assert(width(*p_images) == width(*p_screen)) ;
	assert(height(*p_images) == height(*p_screen)) ;
	*/

	p_screen->draw(*p_images) ;
	p_screen->update() ;

	sleep(5) ;
}

void test_resize()
{
	std::auto_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaaaaa)) ;

	sleep(5) ;
	p_screen->resize(create_size(480, 320)) ;

	std::auto_ptr<Canvas> p_s1 ;
	Canvas::create(p_s1, create_videomode(20, 20, 16)) ;
	p_s1->fill(create_color(0xaa)) ;

	p_screen->draw(*p_s1) ;
	p_screen->update() ;

	p_s1->resize(create_size(50, 50)) ;
	p_screen->draw(*p_s1, create_size(50, 50)) ;
	p_screen->update() ;

	sleep(5) ;
}
