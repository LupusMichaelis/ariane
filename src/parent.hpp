#ifndef HPP_PARENT_QUEST
#	define HPP_PARENT_QUEST

#include "node.hpp"

class Parent
	: public Node
{
	protected:
		Parent();

	public:
		typedef std::shared_ptr<Parent>			SharedPtr;
		typedef std::weak_ptr<Parent>			WeakPtr;

	public:
		class Visitor
			: public Node::Visitor
		{
			public:
				Visitor(Node & w)
					: Node::Visitor {w} { };

				using Node::Visitor::operator();
				virtual
				void operator() (Parent & w) = 0;

		} /* class Visitor */;

		static
		SharedPtr make();

		void adopt(Node::SharedPtr new_child);
		virtual
		Node::SharedPtr abandon(Node::SharedPtr abandoned_child);
		bool const has_child() const;

		List const & children() const;
		List children();

		virtual ~Parent();

	private:
		virtual
		void visit(Node::Visitor & v);

	private:
		struct Impl;
		std::unique_ptr<Impl> mp_impl;

} /* class Parent */;

#endif // HPP_PARENT_QUEST
