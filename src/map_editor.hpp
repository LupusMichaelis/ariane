#ifndef HPP_MAP_EDITOR_QUEST
#	define HPP_MAP_EDITOR_QUEST

#	include "interface.hpp"
#	include "quest_interface.hpp"

class Grid ;
class GridModel ;

class MapEditorInterface
	: public QuestInterface
{
	public: 
		explicit MapEditorInterface(QuestEngine & set_engine) ;
		~MapEditorInterface() ;

		virtual
		void move(EventLoop &, KeyEvent const & ke) ;
		virtual
		void move(EventLoop &, MouseEvent const & me) ;
		virtual
		void move(EventLoop &, MouseButtonEvent const & mbe) ;

		virtual
		void display() ;

	protected:
		TextBox & save_button() const ;
		boost::filesystem::path & filesave() const ;

		Grid & palette_view() ;
		GridModel & palette_model() ;

		Grid & canvas_view() ;
		GridModel & canvas_model() ;

		Grid & last_brushes_view() ;
		GridModel & last_brushes_model() ;
		GridModel const & last_brushes_model() const ;

		unsigned const brush() const ;
		void brush(unsigned const set_brush) ;

	private:
		struct Impl ;
		std::unique_ptr<Impl>			mp_impl ;

} /* class MapEditorInterface */ ;



#endif // HPP_MAP_EDITOR_QUEST
