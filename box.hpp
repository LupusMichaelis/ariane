#ifndef HPP_BOX_QUEST
#	define HPP_BOX_QUEST

#	include "widget.hpp"
#	include "parent.hpp"

#	include <memory>

class Box
	: public Widget
	, public Parent
{
	protected:
		Box() = delete ;
		Box(Gui & gui) ;

		virtual
		void init() ;

	public:
		typedef std::shared_ptr<Box>		SharedPtr ;
		typedef std::weak_ptr<Box>			WeakPtr ;

		static SharedPtr make(Gui & gui) ;

		virtual
		void draw() ;

		virtual ~Box() ;

} /* class Box */ ;

#endif // HPP_BOX_QUEST
