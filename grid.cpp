
#include "grid.hpp"
#include "surface.hpp"
#include "gui_layout.hpp"

#include "tools.hpp"

#include <cassert>

struct Grid::Impl
{
	Impl(Surface const & surface, Size const & sprite_size)
		: m_sprite_size(sprite_size)
		, m_ref(surface.gui_layout().surface(surface))
	{ }

	Size						m_sprite_size ;
	std::unique_ptr<Surface>	m_ref ;

} /* class Grid::Impl */ ;

Grid::~Grid()
{
}

Grid::Grid(Surface const & surface, Size const & sprite_size)
	: Surface {surface}
	, mp_impl { std::make_unique<Impl>(surface, sprite_size) }
{
}

Size const & Grid::sprite_size() const
{
	return mp_impl->m_sprite_size ;
}

std::unique_ptr<Surface> Grid::extract(int const index)
{
	auto p_target = gui_layout().surface(sprite_size()) ;

	Size position = compute_position(index) ;
	crop(*p_target, position, sprite_size()) ;

	return std::move(p_target) ;
}

int const Grid::box_per_row() const
{
	return videomode().size().width() / sprite_size().width() ;
}

int const Grid::box_per_col() const
{
	return videomode().size().height() / sprite_size().height() ;
}

Size const Grid::compute_position(int index) const
{
	int current_row = index / box_per_row() ;
	int current_col = index - current_row * box_per_row() ;

	assert(current_col < box_per_row()) ;
	assert(current_row < box_per_col()) ;

	Size position(0, 0) ;
	position.width(sprite_size().width() * current_col) ;
	position.height(sprite_size().height() * current_row) ;

	return position ;
}

VideoMode const Grid::videomode() const
{
	return mp_impl->m_ref->videomode() ;
}


void Grid::draw(Surface const & motif)
{
	mp_impl->m_ref->draw(motif) ;
}

void Grid::draw(Surface const & motif, Size const & at)
{
	mp_impl->m_ref->draw(motif, at) ;
}

void Grid::update() const
{
	mp_impl->m_ref->update() ;
}

void Grid::fill(RGBColor const & color)
{
	mp_impl->m_ref->fill(color) ;
}

void Grid::fill(Surface const & pattern, Size const & from, Size const & to)
{
	mp_impl->m_ref->fill(pattern, from, to) ;
}

void Grid::resize(Size const & new_size)
{
	mp_impl->m_ref->resize(new_size) ;
}

void Grid::dump(std::string const & filename)
{
	mp_impl->m_ref->dump(filename) ;
}

void Grid::crop(Surface & target, Size const & origin, Size const & size) const
{
	mp_impl->m_ref->crop(target, origin, size) ;
}


void Grid::write(std::string const & message, Style const & style)
{
	mp_impl->m_ref->write(message, style) ;
}


const GuiLayout & Grid::gui_layout() const
{
	return mp_impl->m_ref->gui_layout() ;
}

GuiLayout & Grid::gui_layout()
{
	return mp_impl->m_ref->gui_layout() ;
}


