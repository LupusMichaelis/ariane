#include "color.hpp"
#include "style.hpp"
#include "event.hpp"
#include "gui.hpp"
#include "widget.hpp"
#include "api.hpp"
#include "memory.hpp"
#include "box.hpp"
#include "text_box.hpp"
#include "screen.hpp"

#include <cassert>
#include <cstring>
#include <unistd.h>

#include <memory>
#include <iostream>

#include <boost/format.hpp>
#include <boost/bind.hpp>

class Surface ;
class Interface ;

#include "interface.hpp"
#include "engine.hpp"
#include "visitor.hpp"

int main(int argc, char **argv)
{
	using std::cout ;
	using std::endl ;
	using boost::format ;

	cout << "Running quest\n" ;
	try
	{
		QuestEngine engine ;
		engine.run() ;
	}
	catch(std::exception const & e)
	{
		cout << format("Exception '%s'\n") % e.what() ;
	}
	catch(char const * raw)
	{
		cout << format("Exception '%s'\n") % raw ;
	}
	catch(...)
	{
		cout << format("Strange exception raised\n") ;
	}
}
