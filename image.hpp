#ifndef HPP_IMAGE_SDL_QUEST
#	define HPP_IMAGE_SDL_QUEST

#	include "surface.hpp"

class ImageSDL
	: public Image, public SurfaceSDL
{
	public:
		virtual
		~ImageSDL() ;

		ImageSDL(Gui & gui, std::unique_ptr<ImageMemory, SurfaceSDL::impl_ptr::deleter_type> p_surface, std::string const & filename) ;
} ;

#endif // define HPP_IMAGE_SDL_QUEST
