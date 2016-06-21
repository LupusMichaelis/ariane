#include "image_box.hpp"
#include "gui.hpp"
#include "gui_layout.hpp"
#include "surface.hpp"
#include "style.hpp"
#include "memory.hpp"

struct ImageBox::Impl
{
	Impl(std::unique_ptr<Surface> p_image)
		: mp_image {std::move(p_image)}
	{ }

	std::unique_ptr<Surface> mp_image;
};

ImageBox::ImageBox(Gui & gui, std::unique_ptr<Surface> mp_image)
	: Box {gui}
	, mp_impl {std::make_unique<Impl>(std::move(mp_image))}
{
}

ImageBox::~ImageBox()
{
}

ImageBox::SharedPtr ImageBox::make(Gui & gui, std::string file_name)
{
	auto p_surface = gui.layout().surface(file_name);
	return std::shared_ptr<ImageBox>(new ImageBox {gui, std::move(p_surface)});
}

ImageBox::SharedPtr ImageBox::make(Gui & gui, Surface const & surface)
{
	auto p_surface = gui.layout().surface(surface);
	return std::shared_ptr<ImageBox>(new ImageBox {gui, std::move(p_surface)});
}

/*
ImageBox::SharedPtr ImageBox::make(Gui & gui, Surface const & surface, Size from, Size to)
{
	auto p_surface = gui.layout().surface(surface, from, to);
	return std::shared_ptr<ImageBox>(new ImageBox {gui, p_surface});
}
*/

void ImageBox::draw()
{
	Box::draw();
	surface().draw(*mp_impl->mp_image);
}

