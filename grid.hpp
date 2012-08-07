#ifndef HPP_GRID_SDL_QUEST
#	define HPP_GRID_SDL_QUEST

#	include <memory>

#	include "surface.hpp"
#	include "videomode.hpp"

class Grid
	: public Surface
{
	public:
		Grid(Surface const & surface, Size const & sprite_size) ;
		std::unique_ptr<Surface> extract(int const index) ;

		virtual
		~Grid() ;

		virtual VideoMode const videomode() const ;

		virtual void border(Border const & border) ;
		virtual void draw(Surface const & motif) ;
		virtual void draw(Surface const & motif, Size const & at) ;
		virtual void update() const ;
		virtual void fill(RGBColor const & color) ;
		virtual void fill(Surface const & pattern, Size const & from, Size const & to) ;
		virtual void resize(Size const & new_size) ;
		virtual void dump(std::string const & filename) ;
		virtual void crop(Surface & target, Size const & origin, Size const & size) const;

		virtual void write(std::string const & message, Style const & style) ;

		virtual const GuiLayout & gui_layout() const ;
		virtual GuiLayout & gui_layout() ;

	private:
		Size const compute_position(int index) const ;
		int const box_per_col() const ;
		int const box_per_row() const ;

		Size const & sprite_size() const ;

		class Impl ;
		std::unique_ptr<Impl> mp_impl ;
} ;


#endif // define HPP_GRID_SDL_QUEST
