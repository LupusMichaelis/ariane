#ifndef HPP_SPRITE_RESOURCE_SDL_QUEST
#	define HPP_SPRITE_RESOURCE_SDL_QUEST

#	include <memory>

#	include "surface.hpp"

class SpriteResource
{
	public:
		SpriteResource(std::shared_ptr<Surface> const & matrix, Size const & sprite_size) ;
		void extract(std::shared_ptr<Surface> & p_target, int index) const ;

	private:
		std::shared_ptr<Surface>	mp_matrix ;
		Size						m_sprite_size ;
} ;


#endif // define HPP_SPRITE_RESOURCE_SDL_QUEST
