#pragma once
#include "GameScreen.h"
#include <memory>
#include <queue>
#include "UIButton.h"
#include "Swapvector.h"

class MenuScreen : public GameScreen
{
public:
	MenuScreen(sf::RenderWindow* _pRenderWindow);
	~MenuScreen();

	void Start() override;
	void Update() override;
	void Draw() override;
	void Unload() override;
	void PollInput() override;

	void NewGame();
	void LoadGame();

private:
	void UpdateCursor();

	std::shared_ptr<sf::Sprite> pGlow;
	std::shared_ptr<sf::Sprite> pBackground;
	std::shared_ptr<sf::Sprite> pBlack;

	std::shared_ptr<sf::Sprite> pButtonGlow;

	std::shared_ptr<sf::Sprite> pCursorIcon;

	Swapvector<UIButton*> ButtonList;

	std::queue<std::pair<float, sf::Sprite*>> IntroQueue;

	UIButton* pNewGameButton;
	UIButton* pLoadGameButton;

	float Timer;
	float Alpha;

	bool InIntro;
	bool FadeIn;
	bool FadeOut;
	bool FinalWait;

	bool ButtonClicked;
};

