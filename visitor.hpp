#ifndef HPP_VISITOR_QUEST
#	define HPP_VISITOR_QUEST

#	include "node.hpp"

bool has_child(Node & w) ;
Node::List children(Node & w) ;
void adopt(Node & parent, Node & child) ;
bool has_parent(Node & w) ;
void abandon(Node & child) ; 

#endif // HPP_VISITOR_QUEST
