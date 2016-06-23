
#include "motif_library.hpp"
#include "surface.hpp"
#include "image_box.hpp"

#include <map>
#include <string>
#include <functional>

struct MotifLibrary::Impl
{
	std::map<std::string, std::unique_ptr<Surface>> m_motifs;
};

MotifLibrary::MotifLibrary()
	: mp_impl {std::make_unique<Impl>()}
{
}

MotifLibrary::~MotifLibrary()
{
}

void MotifLibrary::add(std::string name, std::unique_ptr<Surface> & p_new)
{
	mp_impl->m_motifs.insert(std::make_pair(name, std::move(p_new)));
}

std::shared_ptr<ImageBox> MotifLibrary::image_box(Gui & gui, std::string reference) const
{
	auto & motif = *mp_impl->m_motifs.at(reference);
	auto p_image = ImageBox::make(gui, motif);

	return p_image;
}
