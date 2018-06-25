#pragma once
#include "GameScreen.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "BurnAway.h"

class ChapterScreen : public GameScreen 
{
public:
	ChapterScreen(sf::RenderWindow* _pRenderWindow);
	~ChapterScreen();

	void Update();
	void Start();
	void Draw();
	void Unload();
	void PollInput();
	void SetChapterName(std::string _ChapterName);

private:
	sf::Sprite* pBackground;
	sf::RenderTexture* pRenderTexture;
	sf::Text* pText;

	float Timer;

	BurnAway* pBurnAwayShader;
};

