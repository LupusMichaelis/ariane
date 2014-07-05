#include <unistd.h>
#include <cassert>

#include <iostream>
#include <boost/format.hpp>

void test_map() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
	{"test_map", test_map},
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
		catch(std::string s)
		{
			cout << format("Exception std::string '%s'\n") % s ;
		}
		catch(char const * raw)
		{
			cout << format("Exception const char * '%s'\n") % raw ;
		}
		catch(...)
		{
			cout << format("Strange exception raised\n") ;
		}
	}
}


#include "map.hpp"

void test_map()
{
	Map map;
	std::ifstream in { "game/town0" };
	map.load(in);

	std::cout << boost::format("Size {%d,%d,%d}\n")
		% map.dimensions().width()
		% map.dimensions().height()
		% map.dimensions().depth();

	for(auto element: map.elements())
		std::cout << boost::format("%s {%d,%d}\n")
			% element.m_name
			% element.m_position.width()
			% element.m_position.height()
			;

}


