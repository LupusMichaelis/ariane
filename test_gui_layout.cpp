#include <unistd.h>
#include <cassert>

#include <iostream>
#include <boost/format.hpp>

void test_layout() ;
void test_resize() ;
void test_load_sprite() ;
void test_grid_surface() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
	{"test_layout", test_layout},
	{"test_resize", test_resize},
	{"test_grid_surface", test_grid_surface},
	{"test_load_sprite", test_load_sprite},
} ;
int const tests_size = sizeof tests / sizeof (test) ;

#include <cstring>

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	for(int n=0 ; n<tests_size ; ++n)
	{
		test current_test = tests[n] ;
		if(argc > 1 && 0 != std::strcmp(*(argv+1), current_test.name + std::strlen("test_")))
			continue ;

		cout << format("Running test '%s'\n")
			% current_test.name ;
		try
		{
			(current_test.fn)() ;
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
}


#include "surface.hpp"
#include "gui_layout.hpp"
#include "memory.hpp"
#include <memory>

void test_layout()
{
	GuiLayout gl(create_videomode(640, 480, 24)) ;
	auto s = gl.screen() ;

	sleep(1) ;
}

void test_resize()
{
	GuiLayout gl(create_videomode(480, 240, 24)) ;
	auto s = gl.screen() ;

	sleep(1) ;
	s->resize({640, 480}) ;
	sleep(1) ;
}

#include "grid_surface.hpp"

void test_load_sprite()
{
	GuiLayout gl(create_videomode(480, 240, 24)) ;
	auto s = gl.screen() ;

	std::string filename("gfx/kraland_shapes.bmp") ;
	//Style grid_surface_style = screen.style() ;

	auto p_image = gl.surface(filename) ;
	assert(p_image) ;
	auto p_patchwork = gl.grid_surface(*p_image, Size {32, 32}) ;
	assert(p_patchwork) ;

	for(int i=0 ; i < 300 ; ++i)
	{
		std::unique_ptr<Surface> p_sprite = p_patchwork->extract(i) ;
		assert(p_sprite) ;

		s->draw(*p_sprite) ;
		s->update() ;

		usleep(500000) ;
	}

	wait() ;
}

void test_grid_surface()
{
	GuiLayout gl(create_videomode(480, 240, 24)) ;
	auto s = gl.screen() ;

	std::string filename("gfx/kraland_shapes.bmp") ;
	auto p_patchwork = gl.surface(filename) ;

	auto p_sprites = gl.grid_surface(*p_patchwork, { 32, 32 }) ;

	int sprite_per_row = 672 / 32 ;
	//int sprite_per_col = 480 / 32 ;

	Size canvas_size { 5 * 32, 2 * (sprite_per_row + 10) * 32 } ;
	auto p_canvas = gl.surface(canvas_size) ;

	Size sprite_position ;
	for(int k=0 ; k < 2 ; ++k)
		for(int j=0 ; j < sprite_per_row ; ++j)
			for(int i=0 ; i < 5 ; ++i)
			{
				auto p_sprite = p_sprites->extract((i + 5*k) * sprite_per_row + j) ;
				sprite_position =  { i*32, (j+k*sprite_per_row)*32 } ;
				p_canvas->draw(*p_sprite, sprite_position) ;
			}

	// Grounds
	for(int j=0 ; j < 2 ; ++j)
		for(int i=0 ; i < 5 ; ++i)
		{
			auto p_sprite = p_sprites->extract(j + (i + 10) * sprite_per_row) ;
			sprite_position = { i*32, (j+2*sprite_per_row)*32 } ;
			p_canvas->draw(*p_sprite, sprite_position) ;
		}

	// Roads
	for(int j=0 ; j < 2 ; ++j)
		for(int i=0 ; i < 4 ; ++i)
		{
			auto p_sprite = p_sprites->extract(2 + j + (i + 10) * sprite_per_row) ;
			sprite_position = { i*32, (j+2+2*sprite_per_row)*32 } ;
			p_canvas->draw(*p_sprite, sprite_position) ;
		}

	// Trees
	for(int i=0 ; i < 4 ; ++i)
	{
		auto p_sprite = p_sprites->extract(2 + i + 14 * sprite_per_row) ;
		sprite_position = { i*32, (4+2*sprite_per_row)*32 } ;
		p_canvas->draw(*p_sprite, sprite_position) ;
	}

	// Flowers
	{
		auto p_sprite = p_sprites->extract(2 + 3 + 13 * sprite_per_row) ;
		sprite_position = { 4*32, (4+2*sprite_per_row)*32 } ;
		p_canvas->draw(*p_sprite, sprite_position) ;
	}

	s->resize(p_canvas->videomode().size()) ;
	s->draw(*p_canvas) ;
	s->update() ;
	s->dump(std::string("gfx/building.bmp")) ;
}

