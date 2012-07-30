#ifndef HPP_IMAGE_SDL_QUEST
#	define HPP_IMAGE_SDL_QUEST

#	include "surface.hpp"

#	include <string>

class GuiLayout ;

class ImageSDL
	: public ImageSurface, public SurfaceSDL
{
	public:
		virtual
		~ImageSDL() ;

		ImageSDL(GuiLayout & gui_layout, std::unique_ptr<ImageMemory, SurfaceSDL::impl_ptr::deleter_type> p_surface, std::string const & filename) ;
} ;

#endif // define HPP_IMAGE_SDL_QUEST
