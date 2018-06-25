#include "UnlockMission.h"
#include "StateMachine.h"

UnlockMission::UnlockMission(std::string _MissionID)
{
	MissionID = _MissionID;
}

UnlockMission::~UnlockMission()
{
}

void UnlockMission::Run()
{
	PreloadChapter ChapterInfo = LuaManager::GetInstance()->GetChapterInfo(MissionID);
	PlayerManager::GetInstance()->AddAvailableMission(ChapterInfo);
	ScreenManager::GetInstance()->GetRecieveScreen()->RecieveMission(ChapterInfo);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetRecieveScreen());
}
