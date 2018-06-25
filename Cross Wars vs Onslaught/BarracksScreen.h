#pragma once
#include "GameScreen.h"
#include <memory>
#include "UIButton.h"
#include "Swapvector.h"
#include "Camera.h"
#include "PlayerManager.h"

class BarracksScreen : public GameScreen
{
public:
	void Draw() override;
	void Unload() override;
	void Update() override;
	void Start() override;
	void PollInput() override;

	void SetChapter(std::string _ID, std::string _MissionName, int _UnitLimit);
	void ResetBarracks();
	void StartMission();
	
	BarracksScreen(sf::RenderWindow* _pRenderWindow);
	~BarracksScreen();

private:
	void OpenArmyScreen();
	void OpenMissionScreen();
	void OpenItemCreationScreen();
	void OpenItemStorageScreen();
	void OpenArenaScreen();

	std::shared_ptr<sf::Sprite> Background;
	std::shared_ptr<sf::Sprite> MissionIndicator;
	std::shared_ptr<sf::Text> MissionName;

	std::string ChapterID;

	UIButton* pArmyButton;
	UIButton* pMissionButton;
	UIButton* pItemCreationButton;
	UIButton* pItemStorageButton;
	UIButton* pStartButton;
	UIButton* pArenaButton;
	UIButton* pSaveButton;

	Swapvector<UIButton*> ButtonList;
	Camera* pCamera;

	sf::Sprite* pCursorSprite;
};

