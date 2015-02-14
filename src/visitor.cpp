
#include "visitor.hpp"
#include "parent.hpp"

#include <stdexcept>

bool has_child(Node & w)
{
	class impl
		: public Parent::Visitor
	{
			bool yes;

		public:
			impl(Node & w)
				: Parent::Visitor { w }
				, yes(false)
			{ }

			using Node::Visitor::operator();

			virtual
			void operator() (Parent & w)
			{
				yes = w.has_child();
			}

			operator bool() { (*this)(); return yes ; }

	} /* class impl */;

	return impl(w);
}

Node::List children(Node & w)
{
	class impl
		: public Parent::Visitor
	{
		Node::List list;

		public:
			impl(Node & w)
				: Parent::Visitor { w }
				, list()
			{ }

			using Node::Visitor::operator();

			virtual
			void operator() (Parent & w)
			{
				list = w.children();
			}

			operator Node::List() { (*this)(); return list ; }
	} /* class impl */;

	return impl(w);
}

void adopt(Node & parent, Node & child)
{
	class impl
		: public Parent::Visitor
	{
			Node & m_child;
		public:
			impl(Node & parent, Node & child)
				: Parent::Visitor { parent }
				, m_child(child)
			{ }

			using Node::Visitor::operator();

			virtual
			void operator() (Parent & w)
			{
				w.adopt(m_child.self());
			}

	} /* class impl */;

	impl {parent, child } ();
}

bool has_parent(Node const & w)
{
	return w.has_parent();
}

void abandon(Node & child)
{
	class impl
		: public Parent::Visitor
	{
		public:
			impl(Node & w)
				: Parent::Visitor { w }
			{ }

			using Parent::Visitor::operator();

			virtual
			void operator() (Parent & w)
			{
				if(!has_parent(w))
					throw std::logic_error("Orphan can't be abandonned again");

				w.parent()->abandon(w.self());
			}

	} /* class impl */;

	impl {child} ();
}

