#include "ScreenManager.h"
#include "GameScreen.h"
#include "DialogueScreen.h"
#include "ArmyScreen.h"
#include "MenuScreen.h"
#include "ItemCreationScreen.h"

ScreenManager::ScreenManager()
{
	CloseGame = false;
}

ScreenManager::~ScreenManager()
{
	SafeDelete(pMenuScreen);
	SafeDelete(pGameoverScreen);
	SafeDelete(pBattleScreen);
	SafeDelete(pIntermissionScreen);
	SafeDelete(pDialogueScreen);
	SafeDelete(pLevelScreen);
	SafeDelete(pUnitOverviewScreen);
	SafeDelete(pBarracksScreen);
	SafeDelete(pArmyScreen);
	SafeDelete(pMissionScreen);
	SafeDelete(pItemCreationScreen);
	SafeDelete(pChapterScreen);
	SafeDelete(pRecieveScreen);
	SafeDelete(pStorageScreen);
	SafeDelete(pArenaScreen);
}

void ScreenManager::LoadScreen(GameScreen* _pScreen)
{
	ScreenQueue.push(_pScreen);
	ScreenQueue.top()->Start();
}

void ScreenManager::CloseScreen()
{
	if (ScreenQueue.top()->MustUnload)
	{
		ScreenQueue.top()->Unload();
	}
	ScreenQueue.pop();
}

void ScreenManager::CloseAllScreens()
{
	for (int i = 0; i < ScreenQueue.size(); ++i)
	{
		CloseScreen();
	}
}


void ScreenManager::UpdateScreen()
{
	if (!ScreenQueue.top()) return;

	if (ScreenQueue.top()->CloseGame)
	{
		CloseGame = true;
	}else
	{
		ScreenQueue.top()->PollInput();
		ScreenQueue.top()->Update();
	}
}

void ScreenManager::DrawScreen()
{
	if (!ScreenQueue.top()) return;

	ScreenQueue.top()->Draw();
}

void ScreenManager::SetBattleDialogue(std::string _SceneID)
{
	GetInstance()->GetBattleScreen()->pDialogueOverlay->LoadDialogue(_SceneID);
}