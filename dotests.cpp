
#include <unistd.h>
#include <cassert>

#include <memory>
#include <iostream>
#include <boost/format.hpp>

void test_base() ;
void test_resize() ;
void test_load_image() ;
void test_load_sprite() ;
void test_grid() ;
void test_event() ;
void test_write() ;
void test_widget() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
	{"test_base", test_base},
	{"test_resize", test_resize},
	{"test_load_image", test_load_image},
	{"test_load_sprite", test_load_sprite},
	{"test_grid", test_grid},
	{"test_event", test_event},
	{"test_write", test_write},
	{"test_widget", test_widget},
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


