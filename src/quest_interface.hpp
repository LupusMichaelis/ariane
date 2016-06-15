
#ifndef HPP_QUEST_INTERFACE_QUEST
#	define HPP_QUEST_INTERFACE_QUEST

#	include "box.hpp"
#	include "interface.hpp"

class QuestEngine;

class QuestInterface
	: public Interface
{
	public:
		explicit QuestInterface(QuestEngine & engine);

		QuestEngine & engine();

		virtual ~QuestInterface();

	protected:
		void container(Box::SharedPtr p_container);
		Box & container();

	private:
		struct Impl;
		std::unique_ptr<Impl>	mp_impl;

} /* QuestInterface */;

#endif // HPP_QUEST_INTERFACE_QUEST
