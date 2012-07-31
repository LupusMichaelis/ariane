#include <unistd.h>
#include <cassert>

#include <iostream>
#include <boost/format.hpp>

void test_layout() ;
void test_resize() ;
void test_load_sprite() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
	{"test_layout", test_layout},
	{"test_resize", test_resize},
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
#include "tools.hpp"
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

#include "grid.hpp"

void test_load_sprite()
{
	GuiLayout gl(create_videomode(480, 240, 24)) ;
	auto s = gl.screen() ;

	std::string filename("gfx/kraland_shapes.bmp") ;
	//Style grid_style = screen.style() ;

	auto p_image = gl.surface(filename) ;
	assert(p_image) ;
	auto p_patchwork = gl.grid(*p_image, Size {32, 32}) ;
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

