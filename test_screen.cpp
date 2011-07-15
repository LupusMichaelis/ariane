#include <unistd.h>
#include <cassert>

#include <memory>
#include <iostream>
#include <boost/format.hpp>

#include "screen.hpp"
#include "canvas.hpp"

using std::cout ;
using std::endl ;
using boost::format ;

void test_base()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaff00)) ;

	assert(width(*p_screen) == 320) ;
	assert(height(*p_screen) == 280) ;
	assert(depth(*p_screen) == 16) ;

	std::shared_ptr<Canvas> p_s1 ;
	Canvas::create(p_s1, create_videomode(320, 280, 16)) ;
	p_s1->fill(create_color(0x00ff00)) ;
	p_screen->draw(*p_s1) ;

	std::shared_ptr<Canvas> p_s2;
	Canvas::create(p_s2, create_videomode(40, 40, 16)) ;
	p_s2->fill(create_color(0xaa)) ;
	p_screen->draw(*p_s2, Size(20, 20)) ;

	std::shared_ptr<Canvas> p_s3;
	Canvas::create(p_s3, create_videomode(30, 30, 16)) ;
	p_s3->fill(create_color(0xffff00)) ;
	p_screen->draw(*p_s3, Size(10, 10)) ;

	std::shared_ptr<Canvas> p_s4;
	Canvas::create(p_s4, create_videomode(60, 60, 16)) ;
	p_s4->draw(*p_screen) ;
	//p_screen->draw(*p_s4, Size(100, 100)) ;
	p_screen->fill(*p_s4, Size(60, 60), Size(150, 150)) ;

	p_screen->update() ;

	p_screen->dump(std::string(".test_screen.bmp")) ;

	sleep(2) ;
}

#include "image.hpp"

void test_load_image()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;

	std::shared_ptr<Image> p_images ;
	std::string filename("gfx/kraland_shapes.bmp") ;
	Image::create(p_images, filename) ;

	assert(width(*p_images) == 672) ;
	assert(height(*p_images) == 480) ;
	assert(depth(*p_images) == 24) ;

	p_screen->draw(*p_images) ;
	p_screen->update() ;

	sleep(2) ;
}

void test_resize()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaaaaa)) ;
	p_screen->update() ;

	sleep(2) ;
	p_screen->resize(create_size(480, 320)) ;

	std::shared_ptr<Canvas> p_s1 ;
	Canvas::create(p_s1, create_videomode(20, 20, 16)) ;
	p_s1->fill(create_color(0xaa)) ;

	p_screen->draw(*p_s1) ;
	p_screen->update() ;

	p_s1->resize(create_size(50, 50)) ;
	p_screen->draw(*p_s1, create_size(50, 50)) ;
	p_screen->update() ;

	sleep(2) ;
}

#include "grid.hpp"

void test_load_sprite()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 280, 16)) ;

	std::shared_ptr<Image> p_patchwork ;
	std::string filename("gfx/kraland_shapes.bmp") ;
	Image::create(p_patchwork, filename) ;

	Grid sprites(p_patchwork, Size(32,32)) ;

	for(int i=0 ; i < 300 ; ++i)
	{
		std::shared_ptr<Surface> p_sprite ;
		sprites.extract(p_sprite, i) ;

		p_screen->draw(*p_sprite) ;
		p_screen->update() ;

		usleep(500000) ;
	}

	sleep(2) ;
}

