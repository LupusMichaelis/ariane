
#include "grid_surface.hpp"
#include "surface.hpp"
#include "gui_layout.hpp"

#include "tools.hpp"

#include <cassert>

struct GridSurface::Impl
{
	Impl(Surface const & surface, Size const & sprite_size)
		: m_sprite_size(sprite_size)
		, m_ref(surface.gui_layout().surface(surface))
	{ }

	Size						m_sprite_size ;
	std::unique_ptr<Surface>	m_ref ;

} /* class GridSurface::Impl */ ;

GridSurface::~GridSurface()
{
}

GridSurface::GridSurface(Surface const & surface, Size const & sprite_size)
	: Surface {surface}
	, mp_impl { std::make_unique<Impl>(surface, sprite_size) }
{
}

Size const & GridSurface::sprite_size() const
{
	return mp_impl->m_sprite_size ;
}

std::unique_ptr<Surface> GridSurface::extract(unsigned const index)
{
	auto p_target = gui_layout().surface(sprite_size()) ;

	Size position = compute_position(index) ;
	crop(*p_target, position, sprite_size()) ;

	return std::move(p_target) ;
}

unsigned const GridSurface::box_per_row() const
{
	return videomode().size().width() / sprite_size().width() ;
}

unsigned const GridSurface::box_per_col() const
{
	return videomode().size().height() / sprite_size().height() ;
}

Size const GridSurface::compute_position(unsigned const index) const
{
	unsigned const current_row = index / box_per_row() ;
	unsigned const current_col = index - current_row * box_per_row() ;

	assert(current_col < box_per_row()) ;
	assert(current_row < box_per_col()) ;

	Size position(0, 0) ;
	position.width(sprite_size().width() * current_col) ;
	position.height(sprite_size().height() * current_row) ;

	return position ;
}

VideoMode const GridSurface::videomode() const
{
	return mp_impl->m_ref->videomode() ;
}


void GridSurface::border(Border const & /*border*/)
{
}

void GridSurface::draw(Surface const & motif)
{
	mp_impl->m_ref->draw(motif) ;
}

void GridSurface::draw(Surface const & motif, Size const & at)
{
	mp_impl->m_ref->draw(motif, at) ;
}

void GridSurface::update() const
{
	mp_impl->m_ref->update() ;
}

void GridSurface::fill(RGBColor const & color)
{
	mp_impl->m_ref->fill(color) ;
}

void GridSurface::fill(Surface const & pattern, Size const & from, Size const & to)
{
	mp_impl->m_ref->fill(pattern, from, to) ;
}

void GridSurface::resize(Size const & new_size)
{
	mp_impl->m_ref->resize(new_size) ;
}

void GridSurface::dump(std::string const & filename)
{
	mp_impl->m_ref->dump(filename) ;
}

void GridSurface::crop(Surface & target, Size const & origin, Size const & size) const
{
	mp_impl->m_ref->crop(target, origin, size) ;
}


void GridSurface::write(std::string const & message, Style const & style)
{
	mp_impl->m_ref->write(message, style) ;
}


const GuiLayout & GridSurface::gui_layout() const
{
	return mp_impl->m_ref->gui_layout() ;
}

GuiLayout & GridSurface::gui_layout()
{
	return mp_impl->m_ref->gui_layout() ;
}


