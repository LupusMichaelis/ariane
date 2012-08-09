
#include "map_editor.hpp"

#include "engine.hpp"
#include "grid.hpp"
#include "gui_layout.hpp"
#include "style.hpp"
#include "screen.hpp"
#include "visitor.hpp"


struct MapEditorInterface::Impl
{
	GridModel::SharedPtr			mp_canvas_model ;
	GridModel::SharedPtr			mp_palette_model ;
	std::unique_ptr<Surface>		mp_image ;
	GridSurface::SharedPtr			mp_reference ;
	Grid::SharedPtr					mp_canvas_view ;
	Grid::SharedPtr					mp_palette_view ;

	Impl(QuestEngine & engine)
		: mp_canvas_model { std::make_shared<GridModel>(210, 10, 10) }
		, mp_palette_model { std::make_shared<GridModel>(210, 10, 5) }
		, mp_image { std::move(engine.gui().layout().surface("gfx/building.bmp")) }
		, mp_reference { engine.gui().layout().grid_surface(*mp_image , Size {32, 32}) }
		, mp_canvas_view { std::make_shared<Grid>(engine.gui(), mp_canvas_model, mp_reference) }
		, mp_palette_view { std::make_shared<Grid>(engine.gui(), mp_palette_model, mp_reference) }
	{ }

} /* struct MapEditorInterface::Impl */ ;

MapEditorInterface::MapEditorInterface(QuestEngine & set_engine)
	: QuestInterface { set_engine }
	, mp_impl(std::make_unique<Impl>(set_engine))
{
	unsigned middle_width = canvas_model().columns() / 2 ;
	unsigned middle_height = canvas_model().rows() / 2 ;

	for(unsigned row = 0 ; row < canvas_model().rows() ; ++row)
		canvas_model().set(225, row, middle_width) ;
	for(unsigned col = 0 ; col < canvas_model().columns() ; ++col)
		canvas_model().set(227, middle_height, col) ;

	canvas_model().set(226, middle_width, middle_height) ;

	for(unsigned row = 0 ; row < palette_model().rows() ; ++row)
		for(unsigned col = 0 ; col < palette_model().columns() ; ++col)
			palette_model().set(col + row * palette_model().columns(), row, col) ;
}

MapEditorInterface::~MapEditorInterface()
{
}

GridModel & MapEditorInterface::palette_model()
{
	return *mp_impl->mp_palette_model ;
}

GridModel & MapEditorInterface::canvas_model()
{
	return *mp_impl->mp_canvas_model ;
}

Grid & MapEditorInterface::palette_view()
{
	return static_cast<Grid&>(*mp_impl->mp_palette_view) ;
}

Grid & MapEditorInterface::canvas_view()
{
	return static_cast<Grid&>(*mp_impl->mp_canvas_view) ;
}

void MapEditorInterface::display()
{
	auto s = canvas_view().style() ;
	s.border({create_color(0), 0u}) ;
	s.padding({5, 5}) ;

	/////
	s.size({int(32u * canvas_model().columns()), int(32u * canvas_model().rows())}) ;
	canvas_view().style(s) ;

	/////
	s.size({int(32u * palette_model().columns()), int(32u * palette_model().rows())}) ;
	s.position( {32 + canvas_view().style().size().width(), canvas_view().style().position().height() } ) ;
	palette_view().style(s) ;

	Style bg_style = engine().gui().style() ;
	bg_style.position({0, 0}) ;
	bg_style.size(engine().gui().screen().style().size() - Size {20, 20}) ;
	bg_style.color(create_color(0xaaaaaa)) ;

	auto p_container = engine().gui().box(engine().gui().screen(), bg_style) ;
	adopt(*p_container, canvas_view()) ;
	adopt(*p_container, palette_view()) ;

	set_container(p_container) ;
}

void MapEditorInterface::move(EventLoop &, MouseButtonEvent const & mbe)
{
	Size relative = mbe.position() - absolute_position(canvas_view()) ;
	unsigned row = relative.height() / 32 ;
	unsigned col = relative.width() / 32 ;

	if(row < canvas_model().rows() && col < canvas_model().columns())
		canvas_model().set(234, row, col) ;
}

void MapEditorInterface::move(EventLoop &, MouseEvent const & )
{
}

void MapEditorInterface::move(EventLoop &, KeyEvent const & )
{
}


