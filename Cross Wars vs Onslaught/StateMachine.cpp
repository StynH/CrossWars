#include "StateMachine.h"
#include "Battle.h"
#include "Dialogue.h"
#include "AddUnit.h"
#include "Chapter.h"
#include "ShowChapter.h"
#include "Intermission.h"
#include "AddItem.h"
#include "UnlockMission.h"

StateMachine::StateMachine()
{
	pCurrentChapter = nullptr;
}


StateMachine::~StateMachine()
{
}

void StateMachine::LoadChapter(Chapter* _Chapter)
{
	pCurrentChapter = _Chapter;
	_Chapter->ChapterFlow();
	NextState();
}

void StateMachine::NextState()
{
	if(TodoList.size() > 0)
	{
		TodoList.front()->Run();

		SafeDelete(TodoList.front());

		TodoList.pop();
	}
	else
	{
		SafeDelete(pCurrentChapter);
		ReturnToBarracks();
	}
}

void StateMachine::ReturnToBarracks()
{
	ScreenManager::GetInstance()->GetBarracksScreen()->ResetBarracks();
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetBarracksScreen());
}

void StateMachine::ShowChapterScreen()
{
	TodoList.push(new ShowChapter(pCurrentChapter->ChapterName));
}

void StateMachine::AddBattle(std::string _MapID)
{
	TodoList.push(new Battle(_MapID));
}

void StateMachine::AddIntermission(std::string _IntermissionName)
{
	TodoList.push(new Intermission(_IntermissionName));
}

void StateMachine::AddDialogue(std::string _DialogueID)
{
	TodoList.push(new Dialogue(_DialogueID));
}

void StateMachine::AddNewUnit(std::string _UnitID)
{
	TodoList.push(new AddUnit(_UnitID));
}

void StateMachine::AddNewItem(std::string _ItemID)
{
	TodoList.push(new AddItem(_ItemID));
}

void StateMachine::UnlockNewMission(const std::string _MissionID)
{
	TodoList.push(new UnlockMission(_MissionID));
}
