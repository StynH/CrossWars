#pragma once
#include "GameScreen.h"
#include "BattleScreen.h"
#include "DialogueScreen.h"
#include "LevelScreen.h"
#include "UnitOverviewScreen.h"
#include "BarracksScreen.h"
#include "ChapterScreen.h"
#include "ArmyScreen.h"
#include "MissionScreen.h"
#include "GameoverScreen.h"
#include "MenuScreen.h"
#include "IntermissionScreen.h"
#include "RecieveScreen.h"
#include "ItemCreationScreen.h"
#include "StorageScreen.h"
#include "ArenaScreen.h"
#include "ArenaUnitSelectScreen.h"

class ScreenManager : public TSingleton<ScreenManager>
{
public:
	ScreenManager();
	~ScreenManager();

	void LoadScreen(GameScreen* _pScreen);
	void CloseScreen();
	void CloseAllScreens();
	void UpdateScreen();
	void DrawScreen();
	
	MenuScreen* GetMenuScreen() { return pMenuScreen; }
	GameoverScreen* GetGameoverScreen() { return pGameoverScreen; }
	BattleScreen* GetBattleScreen() { return pBattleScreen; }

	IntermissionScreen* GetIntermissionScreen() { return pIntermissionScreen; }
	DialogueScreen* GetDialogueScreen() { return pDialogueScreen; };
	LevelScreen* GetLevelScreen() { return pLevelScreen; };
	UnitOverviewScreen* GetUnitOverviewScreen() { return pUnitOverviewScreen; }
	BarracksScreen* GetBarracksScreen() { return pBarracksScreen; }
	ArmyScreen* GetArmyScreen() { return pArmyScreen; }
	MissionScreen* GetMissionScreen() { return pMissionScreen; }
	ItemCreationScreen* GetItemCreationScreen() { return pItemCreationScreen; }
	ChapterScreen* GetChapterScreen() { return pChapterScreen; }
	RecieveScreen* GetRecieveScreen() { return pRecieveScreen; }
	StorageScreen* GetStorageScreen() { return pStorageScreen; }
	ArenaScreen* GetArenaScreen() { return pArenaScreen; }
	ArenaUnitSelectScreen* GetArenaUnitSelectScreen() { return pArenaUnitSelectScreen; }

	void SetMenuScreen(MenuScreen* _pMenuScreen) { pMenuScreen = _pMenuScreen; }
	void SetGameoverScreen(GameoverScreen* _pGameoverScreen) { pGameoverScreen = _pGameoverScreen; }
	void SetBattleScreen(BattleScreen* _pBattleScreen) { pBattleScreen = _pBattleScreen; }
	void SetIntermissionScreen(IntermissionScreen* _pIntermissionScreen) { pIntermissionScreen = _pIntermissionScreen; }
	void SetDialogueScreen(DialogueScreen* _pDialogueScreen) { pDialogueScreen = _pDialogueScreen; }
	void SetLevelScreen(LevelScreen* _pLevelScreen) { pLevelScreen = _pLevelScreen; }
	void SetUnitOverviewScreen(UnitOverviewScreen* _pUnitOverviewScreen) { pUnitOverviewScreen = _pUnitOverviewScreen; }
	void SetBarracksScreen(BarracksScreen* _pBarracksScreen) { pBarracksScreen = _pBarracksScreen; }
	void SetArmyScreen(ArmyScreen* _pArmyScreen) { pArmyScreen = _pArmyScreen; }
	void SetMissionScreen(MissionScreen* _pMissionScreen) { pMissionScreen = _pMissionScreen; }
	void SetItemCreationScreen(ItemCreationScreen* _pItemCreationScreen) { pItemCreationScreen = _pItemCreationScreen; }
	void SetChapterScreen(ChapterScreen* _pChapterScreen) { pChapterScreen = _pChapterScreen; }
	void SetRecieveScreen(RecieveScreen* _pRecieveScreen) { pRecieveScreen = _pRecieveScreen; }
	void SetStorageScreen(StorageScreen* _pStorageScreen) { pStorageScreen = _pStorageScreen; }
	void SetArenaScreen(ArenaScreen* _pArenaScreen) { pArenaScreen = _pArenaScreen; }
	void SetArenaUnitSelectScreen(ArenaUnitSelectScreen* _pArenaUnitSelectScreen) { pArenaUnitSelectScreen = _pArenaUnitSelectScreen; }

	static void SetBattleDialogue(std::string _SceneID);

	bool CloseGame;
private:
	MenuScreen* pMenuScreen;
	GameoverScreen* pGameoverScreen;
	BattleScreen* pBattleScreen;
	IntermissionScreen* pIntermissionScreen;
	DialogueScreen* pDialogueScreen;
	LevelScreen* pLevelScreen;
	UnitOverviewScreen* pUnitOverviewScreen;
	BarracksScreen* pBarracksScreen;
	ArmyScreen* pArmyScreen;
	MissionScreen* pMissionScreen;
	ItemCreationScreen* pItemCreationScreen;
	ChapterScreen* pChapterScreen;
	RecieveScreen* pRecieveScreen;
	StorageScreen* pStorageScreen;
	ArenaScreen* pArenaScreen;
	ArenaUnitSelectScreen* pArenaUnitSelectScreen;

	std::stack<GameScreen*> ScreenQueue;
};

