
#include "map_editor.hpp"

#include "engine.hpp"
#include "quest_engine.hpp"
#include "grid.hpp"
#include "gui_layout.hpp"
#include "style.hpp"
#include "screen.hpp"
#include "text_box.hpp"
#include "visitor.hpp"

#include <boost/filesystem.hpp>

struct MapEditorInterface::Impl
{
	boost::filesystem::path			m_filesave;
	TextBox::SharedPtr				mp_save_button;

	GridModel::SharedPtr			mp_canvas_model;
	GridModel::SharedPtr			mp_palette_model;
	GridModel::SharedPtr			mp_last_brushes_model;
	std::unique_ptr<Surface>		mp_image;
	GridSurface::SharedPtr			mp_reference;
	Grid::SharedPtr					mp_canvas_view;
	Grid::SharedPtr					mp_palette_view;
	Grid::SharedPtr					mp_last_brushes_view;

	Impl(QuestEngine & engine)
		: m_filesave("/tmp/output.bmp")
		, mp_save_button(engine.gui().text_box(engine.gui().screen(), engine.gui().style()))
		, mp_canvas_model(std::make_shared<GridModel>(210, 13, 17))
		, mp_palette_model(std::make_shared<GridModel>(210, 13, 5))
		, mp_last_brushes_model(std::make_shared<GridModel>(210, 13, 1))
		, mp_image(std::move(engine.gui().layout().surface("gfx/building.bmp")))
		, mp_reference(engine.gui().layout().grid_surface(*mp_image , Size {32, 32}))
		, mp_canvas_view(std::make_shared<Grid>(engine.gui(), mp_canvas_model, mp_reference))
		, mp_palette_view(std::make_shared<Grid>(engine.gui(), mp_palette_model, mp_reference))
		, mp_last_brushes_view(std::make_shared<Grid>(engine.gui(), mp_last_brushes_model, mp_reference))
	{
		mp_save_button->text("Save");
	}

} /* struct MapEditorInterface::Impl */;

MapEditorInterface::MapEditorInterface(QuestEngine & set_engine)
	: QuestInterface { set_engine }
	, mp_impl(std::make_unique<Impl>(set_engine))
{
	unsigned middle_width = canvas_model().columns() / 2;
	unsigned middle_height = canvas_model().rows() / 2;

	for(unsigned row = 0; row < canvas_model().rows() ; ++row)
		canvas_model().set(225, row, middle_width);
	for(unsigned col = 0; col < canvas_model().columns() ; ++col)
		canvas_model().set(227, middle_height, col);

	canvas_model().set(226, middle_height, middle_width);

	for(unsigned row = 0; row < palette_model().rows() ; ++row)
		for(unsigned col = 0; col < palette_model().columns() ; ++col)
			palette_model().set(col + row * palette_model().columns(), row, col);

	brush(234);
}

MapEditorInterface::~MapEditorInterface()
{
}

TextBox & MapEditorInterface::save_button() const
{
	return *mp_impl->mp_save_button;
}

GridModel & MapEditorInterface::palette_model()
{
	return *mp_impl->mp_palette_model;
}

GridModel & MapEditorInterface::canvas_model()
{
	return *mp_impl->mp_canvas_model;
}

GridModel & MapEditorInterface::last_brushes_model()
{
	return const_cast<GridModel &>(const_cast<MapEditorInterface const &>(*this).last_brushes_model());
}

GridModel const & MapEditorInterface::last_brushes_model() const
{
	return *mp_impl->mp_last_brushes_model;
}

Grid & MapEditorInterface::palette_view()
{
	return static_cast<Grid&>(*mp_impl->mp_palette_view);
}

Grid & MapEditorInterface::canvas_view()
{
	return static_cast<Grid&>(*mp_impl->mp_canvas_view);
}

Grid & MapEditorInterface::last_brushes_view()
{
	return static_cast<Grid&>(*mp_impl->mp_last_brushes_view);
}

boost::filesystem::path & MapEditorInterface::filesave() const
{
	return mp_impl->m_filesave;
}

void MapEditorInterface::display()
{
	auto s = canvas_view().style();
	s.border({create_color(0), 0u});
	s.padding({5, 5});

	s.size(Size(45, 25));
	save_button().style(s);

	/////
	s.size({int(32u * canvas_model().columns()), int(32u * canvas_model().rows())});
	s.position(s.position()
			+ Size {0, absolute_position(save_button()).height()
				+ save_button().style().size().height()});
	canvas_view().style(s);

	/////
	s.size({int(32u * last_brushes_model().columns()), int(32u * last_brushes_model().rows())});
	s.position( {32 + canvas_view().style().size().width(), canvas_view().style().position().height() } );
	last_brushes_view().style(s);

	/////
	s.size({int(32u * palette_model().columns()), int(32u * palette_model().rows())});
	s.position( {32 + last_brushes_view().style().position().width()
			+ last_brushes_view().style().size().width()
			, last_brushes_view().style().position().height() } );
	palette_view().style(s);

	Style bg_style = engine().gui().style();
	bg_style.position({0, 0});
	bg_style.size(engine().gui().screen().style().size() - Size {20, 20});
	bg_style.color(create_color(0xaaaaaa));

	auto p_container = engine().gui().box(engine().gui().screen(), bg_style);
	adopt(*p_container, save_button());
	adopt(*p_container, canvas_view());
	adopt(*p_container, last_brushes_view());
	adopt(*p_container, palette_view());

	set_container(p_container);
}

void MapEditorInterface::move(EventLoop &, MouseButtonEvent const & mbe)
{
	if(!mbe.pressing())
		return;

	Size relative = mbe.position() - absolute_position(canvas_view());
	unsigned row = relative.height() / 32;
	unsigned col = relative.width() / 32;

	if(Size {0, 0} < relative and relative < canvas_view().style().size())
		canvas_model().set(brush(), row, col);

	relative = mbe.position() - absolute_position(palette_view());
	row = relative.height() / 32;
	col = relative.width() / 32;

	if(Size {0, 0} < relative and relative < palette_view().style().size())
		brush(palette_model().get(row, col));

	relative = mbe.position() - absolute_position(last_brushes_view());
	row = relative.height() / 32;
	col = relative.width() / 32;
	if(Size {0, 0} < relative and relative < last_brushes_view().style().size())
		brush(last_brushes_model().get(row, col));

	relative = mbe.position() - absolute_position(save_button());
	if(Size {0, 0} < relative and relative < save_button().style().size())
		engine().gui().save(canvas_view(), filesave());
}

void MapEditorInterface::move(EventLoop &, MouseEvent const & )
{
}

void MapEditorInterface::move(EventLoop &, KeyEvent const & )
{
}

unsigned const MapEditorInterface::brush() const
{
	return last_brushes_model().get(0, 0);
}

void MapEditorInterface::brush(unsigned const set_brush)
{
	last_brushes_model().set(set_brush, 0, 0, GridModel::DOWN);
}

