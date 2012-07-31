
#include "node.hpp"
#include "parent.hpp"
#include "tools.hpp"

#include <stdexcept>

struct Node::Impl
{
	Impl(Parent::WeakPtr p_parent)
		: mp_parent { p_parent }
	{}

	Parent::WeakPtr mp_parent ;

} /* struct Node::Impl */ ;

//////////////////////////////////////////////////////////////////////////

Node::Node()
	: mp_impl(std::make_unique<Node::Impl>(Parent::WeakPtr {}))
{
}

// Node::Impl isn't know outside this module, so we have to be sure the complete type is
// known at its deletion. If the dtor wasn't here, the implicit dtor would have been
// invoked in derivate class compile unit.
Node::~Node()
{
}

bool const Node::has_parent() const
{
	return mp_impl->mp_parent.use_count()
		&& bool(Node::SharedPtr(mp_impl->mp_parent)) ;
}

void Node::parent(Parent::WeakPtr new_parent)
{
	// XXX must notify parent ?
	mp_impl->mp_parent = new_parent ;
}

Parent::SharedPtr Node::parent()
{
	if(!has_parent())
		throw std::logic_error("This node has no parent") ;

	return mp_impl->mp_parent.lock() ;
}

Node::SharedPtr Node::self()
{
	return shared_from_this() ;
}
