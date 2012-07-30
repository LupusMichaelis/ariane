
#include "grid.hpp"
#include "surface.hpp"
#include "gui_layout.hpp"
#include "box.hpp"

#include <cassert>

Grid::~Grid()
{
}

Grid::Grid(Widget & subject, Size const & sprite_size)
	: Decorator {std::move(subject.parent().abandon(subject))}
	, m_box_size(sprite_size)
{
}

Box * Grid::extract(int const index)
{
	Style box_style ;
	box_style.size(m_box_size) ;
	auto p_target = gui().box(box_style) ;
	//p_target->draw() ;

	Size position = compute_position(index) ;
	surface().crop(
			p_target->surface(), position, m_box_size) ;

	return static_cast<Box *>(p_target) ;
}

int const Grid::box_per_row() const
{
	return size().width() / m_box_size.width() ;
}

int const Grid::box_per_col() const
{
	return size().height() / m_box_size.height() ;
}

Size const Grid::compute_position(int index) const
{
	int current_row = index / box_per_row() ;
	int current_col = index - current_row * box_per_row() ;

	assert(current_col < box_per_row()) ;
	assert(current_row < box_per_col()) ;

	Size position(0, 0) ;
	position.width(m_box_size.width() * current_col) ;
	position.height(m_box_size.height() * current_row) ;

	return position ;
}


void Grid::draw()
{
}

void Grid::listen_events()
{
}

