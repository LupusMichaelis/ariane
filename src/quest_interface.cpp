
#include "quest_interface.hpp"
#include "quest_engine.hpp"

#include "box.hpp"
#include "visitor.hpp"

////////////////////////////////////////////////////////////////////////////////

struct QuestInterface::Impl
{
	Box::SharedPtr	mp_container;
} /* struct QuestInterface::Impl */;

QuestInterface::QuestInterface(QuestEngine & engine)
	: Interface {engine}
	, mp_impl {std::make_unique<Impl>()}
{
}

QuestInterface::~QuestInterface()
{
	set_container(Box::SharedPtr());
}

QuestEngine & QuestInterface::engine()
{
	return static_cast<QuestEngine &>(Interface::engine());
}

void QuestInterface::set_container(Box::SharedPtr p_container)
{
	if(mp_impl->mp_container && has_parent(*mp_impl->mp_container))
		abandon(*mp_impl->mp_container);

	std::swap(mp_impl->mp_container, p_container);
}
