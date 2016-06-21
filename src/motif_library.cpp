
#include "motif_library.hpp"
#include "surface.hpp"

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

Surface const & MotifLibrary::fetch(std::string reference) const
{
	return *mp_impl->m_motifs.at(reference);
}
