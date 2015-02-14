
#include "quest_engine.hpp"

////////////////////////////////////////////////////////////////////////////////
#include "tortoise.hpp"
void QuestEngine::tortoise()
{
	Engine::set_interface<TortoiseInterface, QuestEngine>();
}


#include "menu.hpp"
void QuestEngine::menu()
{
	Engine::set_interface<MenuInterface, QuestEngine>();
}

#include "map_editor.hpp"
void QuestEngine::map_editor()
{
	Engine::set_interface<MapEditorInterface, QuestEngine>();
}

