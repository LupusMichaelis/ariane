
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
	Canvas::create(p_new_canvas, create_videomode(32, 32, 16)) ;
	Size position(m_sprite_size.width() * index, 0) ;
	mp_matrix->crop(*p_new_canvas, position, m_sprite_size) ;

	std::shared_ptr<Surface> p_new_surface(std::static_pointer_cast<Surface>(p_new_canvas)) ;

	std::swap(p_new_surface, p_target) ;
}
