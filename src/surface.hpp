#ifndef HPP_SURFACE_SDL_QUEST
#	define HPP_SURFACE_SDL_QUEST

#	include "videomode.hpp"
#	include "api.hpp"

#	include <memory>

#	include <boost/utility.hpp>

#	include <SDL/SDL.h>


class RGBColor ;
class Style ;
class VideoMode ;
class Size ;
class GuiLayout ;

class SurfaceSDL
	: virtual public Surface
{
	public:
		explicit SurfaceSDL(GuiLayout & gui_layout, VideoMode videomode, bool is_screen = false) ;
		explicit SurfaceSDL(GuiLayout & gui_layout, std::string filename) ;
		SurfaceSDL(SurfaceSDL const & copied) ;

		~SurfaceSDL() ;

		/** Provide the raw surface resource, depends of graphical backend
		 */
		VideoMode const videomode() const ;

		void border(Border const & border) ;
		void draw(Surface const & motif) ;
		void draw(Surface const & motif, Size const & at) ;
		void draw(Surface const & motif, Size const & at, Size const & by) ;
		void fill(RGBColor const & color) ;
		void fill(Surface const & pattern) ;
		void fill(Surface const & pattern, Size const & from, Size const & by) ;
		void crop(Surface & target, Size const & origin, Size const & size) const ;
		void resize(Size const & new_size) ;
		void write(std::string const & message, Style const & style) ;

		void dump(std::string const & filename) const ;

		void update() const ;

		const GuiLayout & gui_layout() const ;
		GuiLayout & gui_layout() ;

	protected:
		void set_raw(SDL_Surface *) ;
		SDL_Surface * get_raw() const ;

	private:
		void draw_static(Surface const & motif, Size const * at, Size const * by) ;

	private:
		struct Impl ;
		std::unique_ptr<Impl> mp_impl ;
} ;


#endif // define HPP_SURFACE_SDL_QUEST
