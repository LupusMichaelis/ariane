#ifndef HPP_GRID_SDL_QUEST
#	define HPP_GRID_SDL_QUEST

#	include <memory>

#	include "surface.hpp"
#	include "videomode.hpp"

class Box ;

class Grid
	: public Surface
{
	public:
		Grid(Surface & surface, Size const & sprite_size) ;
		std::unique_ptr<Surface> extract(int const index) ;

		virtual
		~Grid() ;

	private:
		Size const compute_position(int index) const ;
		int const box_per_col() const ;
		int const box_per_row() const ;

		Size					m_box_size ;

		Surface & m_surface ;
} ;


#endif // define HPP_GRID_SDL_QUEST
