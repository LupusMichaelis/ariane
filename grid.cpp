
#include "grid.hpp"
#include "surface.hpp"
#include "gui_layout.hpp"
#include "box.hpp"

#include <cassert>

Grid::~Grid()
{
}

Grid::Grid(Surface & surface, Size const & sprite_size)
	: m_box_size(sprite_size)
	, m_surface(surface)
{
}

/*
std::unique_ptr<Surface> Grid::extract(int const index)
{
	auto p_target = gui().layout().surface(box_style.size()) ;

	Size position = compute_position(index) ;
	m_surface.crop(p_target, position, m_box_size) ;

	return std::move(p_target) ;
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

*/

