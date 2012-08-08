#include <unistd.h>
#include <cassert>

#include <iostream>
#include <boost/format.hpp>

void test_widget() ;

struct test { char const * name ; void (*fn)() ; } ;
test const tests[] = {
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


#include "widget.hpp"
#include "parent.hpp"
#include "visitor.hpp"
#include "box.hpp"
#include "gui.hpp"
#include "videomode.hpp"

#include <memory>

void test_widget()
{
	Gui gui(create_videomode(640, 480, 24)) ;

	// Create a root node
	auto p_root = Box::make(gui) ;
	assert(!has_child(*p_root)) ;
	assert(!has_parent(*p_root)) ;

	// Give him a child
	{
		auto p_w1 = Box::make(gui) ;
		adopt(*p_root, *p_w1) ;
	}
	assert(!has_parent(*p_root)) ;
	assert(has_child(*p_root)) ;
	assert(!has_child(*children(*p_root)[0])) ;

	// Make a branch and add it on root
	{
		Box::SharedPtr p_w1 = Box::make(gui)
			, p_w2 = Box::make(gui)
			, p_w3 = Box::make(gui)
			;

		adopt(*p_w1, *p_w2) ;
		adopt(*p_w1, *p_w3) ;
		adopt(*p_root, *p_w1) ;
	}
	assert(has_child(*p_root)) ;
	assert(!has_parent(*p_root)) ;
	assert(!has_child(*children(*p_root)[0])) ;
	assert(has_parent(*children(*p_root)[0])) ;
	assert(has_child(*children(*p_root)[1])) ;
	assert(has_parent(*children(*p_root)[1])) ;
	assert(!has_child(*children(*children(*p_root)[1])[0])) ;
	assert(has_parent(*children(*children(*p_root)[1])[0])) ;

	// Abandon children
	{
		for(auto p_w: children(*p_root))
			abandon(*p_w) ;
	}
	assert(!has_child(*p_root)) ;
	assert(!has_parent(*p_root)) ;

	// Make sure detached childs will be deleted
	{
		Box::SharedPtr p_w1 = Box::make(gui)
			, p_w2 = Box::make(gui)
			, p_w3 = Box::make(gui)
			;

		adopt(*p_w1, *p_w2) ;
		adopt(*p_w1, *p_w3) ;
		adopt(*p_root, *p_w1) ;

		assert(p_w1.use_count() == 2) ;
		assert(p_w2.use_count() == 2) ;
		assert(p_w3.use_count() == 2) ;

		for(auto p_w: children(*p_root))
			p_w->parent()->abandon(p_w) ;

		assert(p_w1.unique()) ;
		assert(!has_parent(*p_w1)) ;
		assert(has_child(*p_w1)) ;

		// The *p_w1 is parent and must be released to let the leaf to be ready for deletion
		assert(has_parent(*p_w2)) ;
		assert(has_parent(*p_w3)) ;
		p_w1.reset() ;
		assert(!has_parent(*p_w2)) ;
		assert(!has_parent(*p_w3)) ;

		assert(p_w2.unique()) ;
		assert(p_w3.unique()) ;
	}
	assert(!has_child(*p_root)) ;
	assert(!has_parent(*p_root)) ;

	// Chain 3 node, attach to the root, detach the second and attach to the first child
	{
		std::vector<Box::SharedPtr> w_list ;
		w_list.push_back(Box::make(gui)) ;
		w_list.push_back(Box::make(gui)) ;
		w_list.push_back(Box::make(gui)) ;
		adopt(*w_list[1], *w_list[2]) ;
		adopt(*w_list[0], *w_list[1]) ;
		adopt(*p_root, *w_list[0]) ;
	}

	assert(!has_parent(*p_root)) ;
	assert(has_child(*p_root)) ;
	assert(children(*p_root).size() == 1) ;

	assert(children(*p_root)[0]->has_parent()) ;
	assert(has_child(*children(*p_root)[0])) ;
	assert(children(*children(*p_root)[0]).size() == 1) ;

	assert(children(*children(*p_root)[0])[0]->has_parent()) ;
	assert(has_child(*children(*children(*p_root)[0])[0])) ;
	assert(children(*children(*children(*p_root)[0])[0]).size() == 1) ;

	assert(has_parent(*children(*children(*children(*p_root)[0])[0])[0])) ;
	assert(!has_child(*children(*children(*children(*p_root)[0])[0])[0])) ;
	assert(children(*children(*children(*children(*p_root)[0])[0])[0]).size() == 0) ;

	{
		adopt(*p_root, *children(*children(*p_root)[0])[0]) ;
	}

	assert(!has_parent(*p_root)) ;
	assert(has_child(*p_root)) ;
	assert(children(*p_root).size() == 2) ;

	assert(children(*p_root)[0]->has_parent()) ;
	assert(!has_child(*children(*p_root)[0])) ;
	assert(children(*children(*p_root)[0]).size() == 0) ;

	assert(children(*p_root)[1]->has_parent()) ;
	assert(has_child(*children(*p_root)[1])) ;
	assert(children(*children(*p_root)[1]).size() == 1) ;

	assert(has_parent(*children(*children(*p_root)[1])[0])) ;
	assert(!has_child(*children(*children(*p_root)[1])[0])) ;
	assert(children(*children(*children(*p_root)[1])[0]).size() == 0) ;
}

