#pragma once
#include "GameScreen.h"
#include <SFML/Graphics/Text.hpp>
#include "BattleTransition.h"
#include "BurnAway.h"
#include "CRTV.h"
#include <queue>

class CharScreen : public GameScreen
{
public:
	CharScreen(sf::RenderWindow* _pRenderWindow);
	~CharScreen();
	void Start();
	void Update();
	void Draw();
	void PollInput();
	void Unload();

	sf::Sprite* pBackground;
	std::string Name;
	sf::Font* Font;
	sf::Text* NameRenderer;
	sf::RenderTexture* pRenderTexture;

	bool EndScene;

	CRTV* pBattleTransitionShader;
	float Timer;
};

