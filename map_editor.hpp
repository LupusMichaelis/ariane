#ifndef HPP_MAP_EDITOR_QUEST
#	define HPP_MAP_EDITOR_QUEST

#	include "interface.hpp"

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
		GridModel & palette_model() ;
		GridModel & canvas_model() ;
		Grid & palette_view() ;
		Grid & canvas_view() ;

	private:
		class Impl ;
		std::unique_ptr<Impl>			mp_impl ;

} /* class MapEditorInterface */ ;



#endif // HPP_MAP_EDITOR_QUEST
