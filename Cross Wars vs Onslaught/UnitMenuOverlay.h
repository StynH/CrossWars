#pragma once
#include "UIButton.h"
#include "Camera.h"
#include "GameScreen.h"
#include "Swapvector.h"
#include "GameScreenOverlay.h"

class LevelScreen;
class Cursor;

class UnitMenuOverlay : public GameScreenOverlay
{
public:
	UnitMenuOverlay(sf::RenderWindow* _pRenderWindow, Cursor* _pCursor, LevelScreen* _pLevelScreen);
	~UnitMenuOverlay();

	void Draw() override;
	void UpdateCursorPosition();
	void Update() override;
	void Unload() override;
	void Hide() override;
	void PollInput() override;
	void Load() override;

	//Options
	void UnitWait();
	void ShowInventoryScreen();
private:
	UIButton* pAttackButton;
	UIButton* pInventoryButton;
	UIButton* pWaitButton;

	bool CanAttack;

	sf::Sprite* pCursorSprite;

	Swapvector<UIButton*> ButtonList;

	Cursor* pCursor;

	LevelScreen* pLevelScreen;
};

