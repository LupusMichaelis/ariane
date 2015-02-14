#ifndef HPP_QUEST_ENGINE_QUEST
#	define HPP_QUEST_ENGINE_QUEST

#	include "engine.hpp"

class QuestEngine
	: public Engine
{
	public:
		void menu();
		void map_editor();
		void tortoise();

		virtual
		void first_interface() { menu(); }

} /* class QuestEngine */;


#endif // HPP_QUEST_ENGINE_QUEST
