#include <unistd.h>
#include <cassert>

#include <iostream>
#include <boost/format.hpp>

void test_surface() ;
void test_layout() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
	{"test_layout", test_layout},
	{"test_surface", test_surface},
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

void test_surface()
{
	SurfaceSDL::impl_ptr p = std::make_unique<ScreenMemory, SurfaceSDL::impl_ptr::deleter_type>(create_videomode(640, 480, 24)) ;
	//auto s = std::make_unique<SurfaceSDL>(std::move(p)) ;
	sleep(1) ;
}

