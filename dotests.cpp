
#include <unistd.h>
#include <cassert>

#include <memory>
#include <iostream>
#include <boost/format.hpp>

void test_base() ;
void test_resize() ;
void test_compose() ;
void test_image_editor() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
	{"test_base", test_base},
	{"test_resize", test_resize},
	{"test_compose", test_compose},
	{"test_image_editor", test_image_editor},
} ;
int const tests_size = sizeof tests / sizeof (test) ;

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	test current_test ;
	for(int n=0 ; n<tests_size ; ++n)
	{
		current_test = tests[n] ;
		cout << format("Running test '%s'\n")
			% current_test.name ;
		try { (current_test.fn)() ; }
		catch(char * raw)
		{
			cout << format("Exception '%s'\n") % raw ;
		}
	}
}


