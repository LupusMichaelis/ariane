#include "screen.hpp"
#include "canvas.hpp"
#include "event.hpp"

#include <unistd.h>
#include <cassert>

#include <memory>
#include <iostream>
#include <boost/format.hpp>

#include <functional>

using std::cout ;
using std::endl ;
using boost::format ;

void print_key_event(EventLoop &, KeyEvent const & ev)
{
	std::cout << boost::format("key event '%d'\n") % ev.key().value() ;
}

void print_mouse_event(EventLoop &, MouseEvent const & ev)
{
	std::cout << boost::format("mouse event (%d,%d) (%c%c%c%c%c)\n")
		% ev.position().width()
		% ev.position().height()
		% (ev.buttons()[0] ? '+' : '-')
		% (ev.buttons()[1] ? '+' : '-')
		% (ev.buttons()[2] ? '+' : '-')
		% (ev.buttons()[3] ? '+' : '-')
		% (ev.buttons()[4] ? '+' : '-')
		;
}

template <int K>
void on_key_stop(EventLoop & ev_loop, KeyEvent const & ev)
{
	print_key_event(ev_loop, ev) ;
	ev_loop.stop() ;
}

void wait()
{
	EventLoop ev_loop ;
	ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(on_key_stop<'q'>)) ;
	ev_loop() ;
}

#include "color.hpp"

void test_base()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaff00)) ;

	assert(width(*p_screen) == 320) ;
	assert(height(*p_screen) == 280) ;
	assert(depth(*p_screen) == 16) ;

	std::shared_ptr<Canvas> p_s1 ;
	create(p_s1, create_videomode(320, 280, 16)) ;
	p_s1->fill(create_color(0x00ff00)) ;
	p_screen->draw(*p_s1) ;

	std::shared_ptr<Canvas> p_s2;
	create(p_s2, create_videomode(40, 40, 16)) ;
	p_s2->fill(create_color(0xaa)) ;
	p_screen->draw(*p_s2, Size(20, 20)) ;

	std::shared_ptr<Canvas> p_s3;
	create(p_s3, create_videomode(30, 30, 16)) ;
	p_s3->fill(create_color(0xffff00)) ;
	p_screen->draw(*p_s3, Size(10, 10)) ;

	std::shared_ptr<Canvas> p_s4;
	create(p_s4, create_videomode(60, 60, 16)) ;
	p_s4->draw(*p_screen) ;
	//p_screen->draw(*p_s4, Size(100, 100)) ;
	p_screen->fill(*p_s4, Size(60, 60), Size(150, 150)) ;

	p_screen->update() ;

	p_screen->dump(std::string(".test_screen.bmp")) ;

	wait() ;
}

#include "image.hpp"

void test_load_image()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 280, 16)) ;

	std::shared_ptr<Image> p_images ;
	std::string filename("gfx/kraland_shapes.bmp") ;
	Image::create(p_images, filename) ;

	assert(width(*p_images) == 672) ;
	assert(height(*p_images) == 480) ;
	assert(depth(*p_images) == 24) ;

	p_screen->draw(*p_images) ;
	p_screen->update() ;

	wait() ;
}

void test_resize()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 280, 16)) ;
	p_screen->fill(create_color(0xaaaaaa)) ;
	p_screen->update() ;

	wait() ;
	p_screen->resize(create_size(480, 320)) ;

	std::shared_ptr<Canvas> p_s1 ;
	create(p_s1, create_videomode(20, 20, 16)) ;
	p_s1->fill(create_color(0xaa)) ;

	p_screen->draw(*p_s1) ;
	p_screen->update() ;

	p_s1->resize(create_size(50, 50)) ;
	p_screen->draw(*p_s1, create_size(50, 50)) ;
	p_screen->update() ;

	wait() ;
}

#include "grid.hpp"

void test_load_sprite()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 280, 16)) ;

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

	wait() ;
}

void test_grid()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(672, 480, 24)) ;

	std::shared_ptr<Image> p_patchwork ;
	std::string filename("gfx/kraland_shapes.bmp") ;
	Image::create(p_patchwork, filename) ;

	Grid sprites(p_patchwork, Size(32,32)) ;

	int sprite_per_row = 672 / 32 ;
	//int sprite_per_col = 480 / 32 ;

	std::shared_ptr<Canvas> p_target ;
	create(p_target, create_videomode(5 * 32, 2 * (sprite_per_row + 10) * 32, 24)) ;
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
	p_screen->resize(p_target->videomode().size()) ;
	p_screen->draw(*p_target) ;
	p_screen->update() ;
	p_target->dump(std::string("gfx/building.bmp")) ;

	wait() ;
}

void test_event()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 240, 24)) ;

	EventLoop ev_loop ;

	ev_loop.attach_event(EventLoop::keyboard_event_type::slot_function_type(print_key_event)) ;
	ev_loop.attach_event(EventLoop::mouse_button_event_type::slot_function_type(print_mouse_event)) ;
	ev_loop.attach_event(EventLoop::mouse_motion_event_type::slot_function_type(print_mouse_event)) ;
	ev_loop() ;
}

#include "style.hpp"

void test_write()
{
	std::shared_ptr<Screen> p_screen ;
	create(p_screen, create_videomode(320, 280, 16)) ;

	std::shared_ptr<Style> p_style ;
	Style::create(p_style, *p_screen) ;
	p_style->color(create_color(0xffffff)) ;
	p_style->font("Comic_Sans_MS") ;
	p_style->size(16) ;

	p_screen->write("Rock'n'roll!", Size(50, 50), *p_style) ;
	p_screen->write("Rock'n'roll!", Size(50, 70), *p_style) ;
	p_screen->write("Rock'n'roll!", Size(50, 90), *p_style) ;
	p_screen->update() ;

	EventLoop ev_loop ;
	ev_loop() ;
}
