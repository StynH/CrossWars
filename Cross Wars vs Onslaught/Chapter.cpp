#include "Chapter.h"
#include <iostream>
#include "PlayerManager.h"
#include "MapManager.h"
#include "StateMachine.h"

Chapter::Chapter()
{
}

Chapter::~Chapter()
{
}

void Chapter::LoadBattle(const std::string& _id)
{
	StateMachine::GetInstance()->AddBattle(_id);
}

void Chapter::LoadIntermission(const std::string& _id)
{
	StateMachine::GetInstance()->AddIntermission(_id);
}

void Chapter::LoadScene(const std::string& _id)
{
	StateMachine::GetInstance()->AddDialogue(_id);
}

void Chapter::AddUnit(const std::string& _id)
{
	StateMachine::GetInstance()->AddNewUnit(_id);
}

void Chapter::AddItem(const std::string& _id)
{
	StateMachine::GetInstance()->AddNewItem(_id);
}

void Chapter::UnlockNewMission(const std::string& _id)
{
	StateMachine::GetInstance()->UnlockNewMission(_id);
}

void Chapter::ShowChapterScreen()
{
	StateMachine::GetInstance()->ShowChapterScreen();
}

void Chapter::ChapterFlow()
{
	if (luaChapterFlow) {
		try {
			(*luaChapterFlow)(this);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "LuaException: " << e.what() << std::endl;
		}
	}
}