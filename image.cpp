
#include "image.hpp"

#include <SDL.h>


ImageSDL::ImageSDL(Gui & gui, std::unique_ptr<ImageMemory> p_surface, std::string const & filename)
	: Image(filename)
	, SurfaceSDL(gui, move(p_surface))
{
}

ImageSDL::~ImageSDL()
{ }

