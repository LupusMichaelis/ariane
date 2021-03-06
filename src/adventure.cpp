
#include "adventure.hpp"

#include "engine.hpp"
#include "style.hpp"
#include "screen.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "image_box.hpp"
#include "motif_library.hpp"

#include "quest_engine.hpp"

#ifndef NDEBUG
#	include <iostream>
#	include <boost/format.hpp>
#endif // NDEBUG

#include "SDL/SDL.h"

AdventureInterface::AdventureInterface(QuestEngine & engine)
	: QuestInterface {engine}
	, mp_adventure_model {std::make_unique<AdventureModel>()}
	, mp_motif_library {engine.gui().layout().motif_library("gfx/town0")}
{
}

AdventureInterface::~AdventureInterface()
{
}

void AdventureInterface::display()
{
	Screen & screen = engine().gui().screen();

	Style bg_style = screen.style();
	Size padding {10, 10};
	Size cell_size {32, 32};

	Size scene_size = adventure_model().map().dimensions().size();
	scene_size.width(scene_size.width() * cell_size.width());
	scene_size.height(scene_size.height() * cell_size.height());

	bg_style.size(scene_size);

	bg_style.position(padding);
	bg_style.color(create_color(0xaaaaff));
	auto p_container = screen.gui().box(screen, bg_style);

	container(p_container);

	for(auto & element: adventure_model().map().elements())
	{
		auto p_image = motif_library().image_box(screen.gui(), element.m_name);
		auto image_style = screen.gui().style();
		image_style.color(create_color(0x89ff3a));
		image_style.padding({0,0});
		image_style.size(cell_size);
		image_style.position((element.m_position - Size{1, 1}) * 32);
		p_image->style(image_style);
		p_container->adopt(p_image);

#ifndef NDEBUG
		std::cout << boost::format("Element '%s' at {w:%d,h:%d}\n")
			% element.m_name % element.m_position.width() % element.m_position.height();

		std::cout << boost::format("\tImage at {w:%d,h%d}\n")
			% image_style.position().width() % image_style.position().height();
#endif // NDEBUG
	}

	// Let's have the player start holding the door
	auto image_style = screen.gui().style();
	image_style.color(create_color(0x89ff30));
	image_style.padding({0,0});
	image_style.size(cell_size);

	{
		auto const doors = adventure_model().map().find_element_by_name("door");
		image_style.position((doors.front().m_position - Size{1,0})* 32);
	}

	mp_hodor = motif_library().image_box(screen.gui(), "player");
	mp_hodor->style(image_style);
	p_container->adopt(mp_hodor);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void AdventureInterface::move(EventLoop &, MouseButtonEvent const & me)
{
}

void AdventureInterface::move(EventLoop &, MouseEvent const & me)
{
}

void AdventureInterface::move(EventLoop &, KeyEvent const & ke)
{
	if(!ke.pressing())
		return;

	KeyBoard const & kb = engine().keyboard();

	if(ke.key() == kb.up())
		move_up();
	if(ke.key() == kb.down())
		move_down();
	if(ke.key() == kb.right())
		move_right();
	if(ke.key() == kb.left())
		move_left();
}

void AdventureInterface::move_left()
{
	Style hodor_style = mp_hodor->style();
	Size new_position = hodor_style.position() - Size(32, 0);
	hodor_style.position(new_position);
	mp_hodor->style(hodor_style);
}

void AdventureInterface::move_right()
{
	Style hodor_style = mp_hodor->style();
	Size new_position = hodor_style.position() + Size(32, 0);
	hodor_style.position(new_position);
	mp_hodor->style(hodor_style);
}

void AdventureInterface::move_up()
{
	Style hodor_style = mp_hodor->style();
	Size new_position = hodor_style.position() - Size(0, 32);
	hodor_style.position(new_position);
	mp_hodor->style(hodor_style);
}

void AdventureInterface::move_down()
{
	Style hodor_style = mp_hodor->style();
	Size new_position = hodor_style.position() + Size(0, 32);
	hodor_style.position(new_position);
	mp_hodor->style(hodor_style);
}

AdventureModel & AdventureInterface::adventure_model()
{
	return const_cast<AdventureModel &>(const_cast<AdventureInterface const&>(*this).adventure_model());
}

AdventureModel const & AdventureInterface::adventure_model() const
{
	return *mp_adventure_model;
}

MotifLibrary & AdventureInterface::motif_library()
{
	return *mp_motif_library;
}

////////////////////////////////////////////////////////////////////////////////

struct AdventureModel::Impl
{
	Size m_position;
	Map m_map;
} /* struct AdventureModel */;

AdventureModel::AdventureModel()
	: mp_impl(std::make_unique<Impl>())
{
	std::ifstream in { "game/town0" };
	mp_impl->m_map.load(in);
}

AdventureModel::~AdventureModel()
{
}

Map const & AdventureModel::map() const
{
	return mp_impl->m_map;
}

Size const & AdventureModel::position() const
{
	return mp_impl->m_position;
}
