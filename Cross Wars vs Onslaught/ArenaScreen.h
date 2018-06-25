#pragma once
#include "GameScreen.h"
#include "Swapvector.h"
#include "UIButton.h"

class ArenaScreen : public GameScreen
{
public:
	enum DIFFICULTY { EASY, NORMAL, HARD, min = EASY, max = HARD };

	ArenaScreen(sf::RenderWindow* _pRenderWindow);
	~ArenaScreen();

	void Start() override;
	void Draw() override;
	void Unload() override;
	void PollInput() override;

	void UpdateCounter();
	void UpdateCursorPosition();
	void UpdateDifficultyCursor();
private:
	void StartArena();
	void ShowUnitSelectScreen();

	sf::Sprite* pBackground;
	sf::Sprite* pUnitCounter;
	sf::Sprite* pDifficultySprite;
	sf::Sprite* pDifficultySelector;
	sf::Sprite* pButtonCursorSprite;

	UIButton* pUnitSelectButton;
	UIButton* pStartButton;

	sf::Text* pUnitCounterText;

	std::vector<UIButton*> ButtonList;

	DIFFICULTY CurrentDifficulty;
	bool SelectingDifficulty;

	int Counter;
};

