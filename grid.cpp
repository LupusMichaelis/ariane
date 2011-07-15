
#include "grid.hpp"
#include "canvas.hpp"


Grid::Grid(std::shared_ptr<Surface> const & matrix, Size const & sprite_size)
	: mp_matrix(matrix)
	, m_box_size(sprite_size)
{
}
#include <cassert>

void Grid::extract(std::shared_ptr<Surface> & p_target, int index) const
{
	std::shared_ptr<Canvas> p_new_canvas ;
	Canvas::create(p_new_canvas, create_videomode(m_box_size.width(), m_box_size.height(), 16)) ;

	Size position = compute_position(index) ;

	mp_matrix->crop(*p_new_canvas, position, m_box_size) ;

	std::shared_ptr<Surface> p_new_surface(std::static_pointer_cast<Surface>(p_new_canvas)) ;

	std::swap(p_new_surface, p_target) ;
}

int const Grid::box_per_row() const
{
	return width(*mp_matrix) / m_box_size.width() ;
}

int const Grid::box_per_col() const
{
	return height(*mp_matrix) / m_box_size.height() ;
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

