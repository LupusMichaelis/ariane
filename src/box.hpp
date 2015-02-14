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
		Box() = delete;
		Box(Gui & gui);

		virtual
		void init();

	public:
		typedef std::shared_ptr<Box>		SharedPtr;
		typedef std::weak_ptr<Box>			WeakPtr;

		class Visitor
		{
				Box const & m_box;
			public:
				Visitor(Box const & w)
					: m_box(w) { };

				virtual
				void operator() () { m_box.visit(*this); }

				virtual
				void operator() (Box const & w) = 0;

		} /* class Visitor */;

		static SharedPtr make(Gui & gui);

		virtual
		void draw();

		virtual ~Box();

	private:
		virtual
		void visit(Visitor & v) const { v(*this); }

} /* class Box */;

Size const absolute_position(Box const & b);

#endif // HPP_BOX_QUEST
