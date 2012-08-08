#ifndef HPP_GRID_SDL_QUEST
#	define HPP_GRID_SDL_QUEST

#	include "box.hpp"
#	include "grid_surface.hpp"

class Grid ;

class GridModel
{
	public:
		typedef std::shared_ptr<GridModel> SharedPtr ;

	public:
		GridModel(unsigned const default_value, unsigned const rows, unsigned const columns) ;
		unsigned const rows() const ;
		unsigned const columns() const ;

		unsigned const get(unsigned const rows, unsigned const columns) ;
		void set(unsigned const id, unsigned const rows, unsigned const columns) ;

		virtual
		~GridModel() ;

	private:
		class Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class GridModel */ ;

class Grid
	: public Box
{
	public:
		Grid(Gui & set_gui, GridModel::SharedPtr set_p_grid_model, GridSurface::SharedPtr p_reference = GridSurface::SharedPtr { }) ;

		virtual
		GridModel const & grid_model() const ;
		virtual
		GridModel & grid_model() ;

		virtual
		unsigned const rows() const ;

		virtual
		unsigned const columns() const ;

		virtual
		void draw() ;

		virtual ~Grid() ;

	private:
		class Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class Grid */ ;

#endif // HPP_GRID_SDL_QUEST
