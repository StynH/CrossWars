#pragma once
#include <queue>
#include "GameState.h"

//The big controller behind it all.
//The HAL-9000 so to say. (Can't let you do that, ScreenManager)
//Controls the flow of the game and overall very scary.
class StateMachine : public TSingleton<StateMachine>
{
public:
	StateMachine();
	~StateMachine();

	void LoadChapter(Chapter* _Chapter);

	void NextState();
	void ReturnToBarracks();
	void ShowChapterScreen();
	void AddBattle(std::string _MapID);
	void AddIntermission(std::string _IntermissionName);
	void AddDialogue(std::string _DialogueID);
	void AddNewUnit(std::string _UnitID);
	void AddNewItem(std::string _ItemID);
	void UnlockNewMission(const std::string _MissionID);
	std::queue<GameState*> TodoList;
	
private:
	Chapter* pCurrentChapter;
};

