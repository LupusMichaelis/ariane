
#include "grid.hpp"
#include "gui_layout.hpp"

#include <cassert>

Grid::Grid(Surface const & matrix, Size const & sprite_size)
	: m_matrix(matrix)
	, m_box_size(sprite_size)
{
}

std::unique_ptr<Surface> Grid::extract(int index) const
{
	auto & gui_layout = m_matrix.gui_layout() ;
	auto new_surface = gui_layout.surface(m_box_size) ;

	Size position = compute_position(index) ;
	m_matrix.crop(*new_surface, position, m_box_size) ;

	return new_surface ;
}

int const Grid::box_per_row() const
{
	return width(m_matrix) / m_box_size.width() ;
}

int const Grid::box_per_col() const
{
	return height(m_matrix) / m_box_size.height() ;
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

