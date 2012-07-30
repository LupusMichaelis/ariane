#ifndef HPP_GRID_SDL_QUEST
#	define HPP_GRID_SDL_QUEST

#	include <memory>

#	include "videomode.hpp"
#	include "decorator.hpp"

class Box ;

class Grid
	: public Decorator
{
	public:
		Grid(Widget & subject, Size const & sprite_size) ;
		Box * extract(int const index) ;

		virtual
		~Grid() ;

	private:
		Size const compute_position(int index) const ;
		int const box_per_col() const ;
		int const box_per_row() const ;

		Size					m_box_size ;

		virtual
		void draw() ;
		virtual
		void listen_events() ;

} ;


#endif // define HPP_GRID_SDL_QUEST
