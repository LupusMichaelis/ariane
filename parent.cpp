
#include "parent.hpp"
#include "tools.hpp"

#include <algorithm>

struct Parent::Impl
{
	Impl()
		: m_children()
	{}

	List m_children ;

} /* struct Parent::Impl */ ;

//////////////////////////////////////////////////////////////////////////
Parent::SharedPtr Parent::make()
{
	return Parent::SharedPtr(new Parent()) ;
}

Parent::Parent()
	: Node()
	, mp_impl(std::make_unique<Parent::Impl>())
{
}

Parent::~Parent()
{
	for(auto p_w: mp_impl->m_children)
		p_w->parent(Parent::SharedPtr { }) ;
}

bool const Parent::has_child() const
{
	return mp_impl->m_children.size() > 0 ;
}

Parent::List const & Parent::children() const
{
	return mp_impl->m_children ;
}

void Parent::adopt(Node::SharedPtr new_child)
{
	if(new_child->has_parent())
		new_child->parent()->abandon(new_child) ;

	new_child->parent(std::static_pointer_cast<Parent, Node>(self())) ;
	mp_impl->m_children.push_back(new_child) ;
}

Node::SharedPtr Parent::abandon(Node::SharedPtr abandoned_child)
{
	auto it = std::find_if(mp_impl->m_children.begin(), mp_impl->m_children.end()
			, [& abandoned_child] (List::value_type & p_w) -> bool
			{ return abandoned_child == p_w ; }) ;

	if(it == mp_impl->m_children.end())
		throw "Can't abandon non-child Node" ;

	auto r = *it ;
	mp_impl->m_children.erase(it) ;

	r->parent(Parent::SharedPtr { }) ;

	return r ;
}

Node::List Parent::children()
{
	return const_cast<Parent const &>(*this).children() ;
}

void Parent::visit(Node::Visitor & v)
{
	v(*this) ;
}
