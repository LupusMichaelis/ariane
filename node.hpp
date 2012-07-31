#ifndef HPP_NODE_QUEST
#	define HPP_NODE_QUEST

class Node ;
class Parent ;

#	include <memory>
#	include <vector>

class Node
	: public std::enable_shared_from_this<Node>
{
	public:
		typedef std::shared_ptr<Node>				SharedPtr ;
		typedef std::weak_ptr<Node>					WeakPtr ;
		typedef std::vector<Node::SharedPtr>		List ;

		class Visitor
		{
			public:
				Visitor(Node & w)
					: m_w(w) { } ;

				virtual
				void operator() ()
				{ m_w.visit(*this) ; }

				virtual
				void operator() (Parent & w) = 0 ;

			private:
				Node & m_w ;

		} /* class Visitor */ ;

	private:
		virtual
		void visit(Visitor & v) = 0 ;

	public:
		Node() ;
		virtual
		~Node() ;

		virtual
		SharedPtr self() ;

		virtual
		bool const has_parent() const ;
		virtual
		std::shared_ptr<Parent> parent() ;
		virtual
		void parent(std::weak_ptr<Parent> new_parent) ;

	private:
		struct Impl ;
		std::unique_ptr<Impl> mp_impl ;

} /* class Node */ ;

#endif // HPP_NODE_QUEST
