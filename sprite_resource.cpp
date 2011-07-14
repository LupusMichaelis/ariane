
#include "sprite_resource.hpp"
#include "canvas.hpp"


SpriteResource::SpriteResource(std::shared_ptr<Surface> const & matrix, Size const & sprite_size)
	: mp_matrix(matrix)
	, m_sprite_size(sprite_size)
{
}
#include <cassert>

void SpriteResource::extract(std::shared_ptr<Surface> & p_target, int index) const
{
	std::shared_ptr<Canvas> p_new_canvas ;
	Canvas::create(p_new_canvas, create_videomode(m_sprite_size.width(), m_sprite_size.height(), 16)) ;

	Size position = compute_position(index) ;

	mp_matrix->crop(*p_new_canvas, position, m_sprite_size) ;

	std::shared_ptr<Surface> p_new_surface(std::static_pointer_cast<Surface>(p_new_canvas)) ;

	std::swap(p_new_surface, p_target) ;
}

Size const SpriteResource::compute_position(int index) const
{
	int element_per_row = width(*mp_matrix) / m_sprite_size.width() ;
	int element_per_col = height(*mp_matrix) / m_sprite_size.height() ;

	int current_row = index / element_per_row ;
	int current_col = index - current_row * element_per_row ;

	assert(current_col < element_per_row) ;
	assert(current_row < element_per_col) ;

	Size position(0, 0) ;
	position.width(m_sprite_size.width() * current_col) ;
	position.height(m_sprite_size.height() * current_row) ;

	return position ;
}

