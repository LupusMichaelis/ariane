
#include "api.hpp"


Surface::~Surface()
{
}

ImageSurface::ImageSurface(std::string const & filename)
	: m_filename(filename)
{
}

ImageSurface::~ImageSurface()
{
}
