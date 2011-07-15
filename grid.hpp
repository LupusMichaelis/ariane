#ifndef HPP_GRID_SDL_QUEST
#	define HPP_GRID_SDL_QUEST

#	include <memory>

#	include "surface.hpp"

class Grid
{
	public:
		Grid(std::shared_ptr<Surface> const & matrix, Size const & sprite_size) ;
		void extract(std::shared_ptr<Surface> & p_target, int index) const ;

	private:
		Size const compute_position(int index) const ;
		int const box_per_col() const ;
		int const box_per_row() const ;

		std::shared_ptr<Surface>	mp_matrix ;
		Size						m_box_size ;
} ;


#endif // define HPP_GRID_SDL_QUEST
