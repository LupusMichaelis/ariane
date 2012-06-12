
#include "image.hpp"

#include <SDL.h>


ImageSDL::ImageSDL(GuiLayout & gui_layout, std::unique_ptr<ImageMemory, SurfaceSDL::impl_ptr::deleter_type> p_surface, std::string const & filename)
	: Image(filename)
	, SurfaceSDL(gui_layout, move(p_surface))
{
}

ImageSDL::~ImageSDL()
{ }

