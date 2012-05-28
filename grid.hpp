#ifndef HPP_GRID_SDL_QUEST
#	define HPP_GRID_SDL_QUEST

#	include <memory>

#	include "surface.hpp"

class Grid
{
	public:
		Grid(Surface const & matrix, Size const & sprite_size) ;
		std::unique_ptr<Surface> extract(int index) const ;

		std::shared_ptr<Surface> const surface() const ;

	private:
		Size const compute_position(int index) const ;
		int const box_per_col() const ;
		int const box_per_row() const ;

		Surface const &			m_matrix ;
		Size					m_box_size ;
} ;


#endif // define HPP_GRID_SDL_QUEST
