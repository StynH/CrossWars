#include "Battle.h"
#include "MapManager.h"

Battle::Battle(std::string _MapID) : GameState()
{
	MapID = _MapID;
}

Battle::~Battle()
{
}

void Battle::Run()
{
	ScreenManager::GetInstance()->GetLevelScreen()->SetNewMap(MapManager::GetInstance()->LoadMap(MapID));
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetLevelScreen());
}
