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

void test_grid()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(672, 480, 24)) ;

	std::shared_ptr<Image> p_patchwork ;
	std::string filename("gfx/kraland_shapes.bmp") ;
	Image::create(p_patchwork, filename) ;

	Grid sprites(p_patchwork, Size(32,32)) ;

	int sprite_per_row = 672 / 32 ;
	//int sprite_per_col = 480 / 32 ;

	std::shared_ptr<Canvas> p_target ;
	Canvas::create(p_target, create_videomode(5 * 32, 2 * (sprite_per_row + 10) * 32, 24)) ;
	for(int k=0 ; k < 2 ; ++k)
		for(int j=0 ; j < sprite_per_row ; ++j)
			for(int i=0 ; i < 5 ; ++i)
			{
				std::shared_ptr<Surface> p_sprite ;
				sprites.extract(p_sprite, (i + 5*k) * sprite_per_row + j) ;
				p_target->draw(*p_sprite, Size(i*32, (j+k*sprite_per_row)*32)) ;
			}

	// Grounds
	for(int j=0 ; j < 2 ; ++j)
		for(int i=0 ; i < 5 ; ++i)
		{
			std::shared_ptr<Surface> p_sprite ;
			sprites.extract(p_sprite, j + (i + 10) * sprite_per_row) ;
			p_target->draw(*p_sprite, Size(i*32, (j+2*sprite_per_row)*32)) ;
		}

	// Roads
	for(int j=0 ; j < 2 ; ++j)
		for(int i=0 ; i < 4 ; ++i)
		{
			std::shared_ptr<Surface> p_sprite ;
			sprites.extract(p_sprite, 2 + j + (i + 10) * sprite_per_row) ;
			p_target->draw(*p_sprite, Size(i*32, (j+2+2*sprite_per_row)*32)) ;
		}

	// Trees
	for(int i=0 ; i < 4 ; ++i)
	{
		std::shared_ptr<Surface> p_sprite ;
		sprites.extract(p_sprite, 2 + i + 14 * sprite_per_row) ;
		p_target->draw(*p_sprite, Size(i*32, (4+2*sprite_per_row)*32)) ;
	}

	// Flowers
	{
		std::shared_ptr<Surface> p_sprite ;
		sprites.extract(p_sprite, 2 + 3 + 13 * sprite_per_row) ;
		p_target->draw(*p_sprite, Size(4*32, (4+2*sprite_per_row)*32)) ;
	}

	p_target->update() ;
	p_screen->draw(*p_target) ;
	p_screen->update() ;
	p_target->dump(std::string("gfx/building.bmp")) ;

//	sleep(2) ;
}

#include <functional>
#include "event.hpp"

void print_key_event(KeyEvent const & ev)
{
	std::cout << boost::format("hop '%c'\n") % ev.key() ;
}

void test_event()
{
	std::shared_ptr<Screen> p_screen ;
	Screen::create(p_screen, create_videomode(320, 240, 24)) ;

	EventLoop ev_loop ;

	ev_loop.attach_event("onkeypress", print_key_event) ;
	ev_loop() ;
}

