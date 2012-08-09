
#include "grid.hpp"
#include "memory.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
struct Grid::Impl
{
	Impl(GridModel::SharedPtr set_p_grid_model, GridSurface::SharedPtr set_p_reference)
		: mp_grid_model(set_p_grid_model)
		, mp_reference(set_p_reference)
	{ }

	GridModel::SharedPtr		mp_grid_model ;
	GridSurface::SharedPtr		mp_reference ;

} /* class Grid::Impl */ ;

Grid::Grid(Gui & set_gui, GridModel::SharedPtr set_p_grid_model, GridSurface::SharedPtr set_p_reference)
	: Box {set_gui}
	, mp_impl(std::make_unique<Impl>(set_p_grid_model, set_p_reference))
{
	assert(mp_impl->mp_grid_model) ;
	assert(mp_impl->mp_reference) ;
}

Grid::~Grid()
{
}

unsigned const Grid::rows() const
{
	return mp_impl->mp_grid_model->rows() ;
}

unsigned const Grid::columns() const
{
	return mp_impl->mp_grid_model->columns() ;
}

GridModel const & Grid::grid_model() const
{
	return *mp_impl->mp_grid_model ;
}

GridModel & Grid::grid_model()
{
	return const_cast<GridModel&>(const_cast<Grid const &>(*this).grid_model()) ;
}

void Grid::draw()
{
	Box::draw() ;

	for(unsigned row = 0 ; row < grid_model().rows() ; ++row)
		for(unsigned col = 0 ; col < grid_model().columns() ; ++col)
		{
			auto p_tile = mp_impl->mp_reference->extract(grid_model().get(row, col)) ;
			surface().draw(*p_tile, Size {32 * (int)col, 32 * (int)row}) ;
		}
}

////////////////////////////////////////////////////////////////////////////////
#include <vector>

struct GridModel::Impl
{
	Impl(unsigned const default_value, unsigned const set_rows, unsigned const set_columns)
		: m_rows(set_rows)
		, m_columns(set_columns)
		// WARNING: don't use initializer syntax because it will interpret it as a list of
		// ints to init the most nested vector
		, m_data(set_rows, std::vector<unsigned> (set_columns, default_value ))
	{
	}

	unsigned const	m_rows ;
	unsigned const	m_columns ;

	std::vector<std::vector<unsigned>>
					m_data ;

} /* class Grid::Impl */ ;

GridModel::GridModel(unsigned const default_value, unsigned const set_rows, unsigned const set_columns)
	: mp_impl(std::make_unique<Impl>(default_value, set_rows, set_columns))
{
}

GridModel::~GridModel()
{
}

unsigned const GridModel::columns() const
{
	return mp_impl->m_columns ;
}

unsigned const GridModel::rows() const
{
	return mp_impl->m_rows ;
}

unsigned const GridModel::get(unsigned const rows, unsigned const columns) const
{
	return mp_impl->m_data.at(rows).at(columns) ;
}

void GridModel::set(unsigned const id, unsigned const rows, unsigned const columns)
{
	mp_impl->m_data[rows][columns] = id ;
}


void GridModel::set(unsigned const id, unsigned const row, unsigned const column, enum Direction direction)
{
	if(direction == UP)
	{
		throw "Todo" ;
	}
	else if(direction == DOWN)
	{
		unsigned previous = id ;
		for(unsigned i = row ; i < mp_impl->m_data.size() ; ++i)
		{
			unsigned buffer = mp_impl->m_data[i][column] ;
			mp_impl->m_data[i][column] = previous ;
			previous = buffer ;
		}

	}
	else if(direction == RIGHT)
	{
		throw "Todo" ;
	}
	else if(direction == LEFT)
	{
		throw "Todo" ;
	}
}


