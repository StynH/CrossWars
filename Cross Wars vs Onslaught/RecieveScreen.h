#pragma once
#include "GameScreen.h"
#include "Unit.h"
struct PreloadChapter;

class RecieveScreen : public GameScreen
{
public:
	RecieveScreen(sf::RenderWindow* _pRenderWindow);
	~RecieveScreen();

	void Start() override;
	void Draw() override;
	void Update() override;
	void Unload() override;
	void PollInput() override;

	void RecieveItem(Item* _pItem);
	void RecieveUnit(Unit* _pUnit);
	void RecieveMission(const PreloadChapter& _pChapter);
private:
	sf::Sprite* pBackground;
	sf::Sprite* pBlackScreen;

	sf::Sprite* pItemContainer;
	sf::Sprite* pUnitContainer;

	sf::Text* pText;

	sf::Sprite* pCurrentContainer;

	Unit* pUnit;
	Item* pItem;

	sf::RenderTexture* pRenderTexture;

	bool BounceBack;
	bool InIntro;
	float ScaleSpeed;
	float Scale;

	bool InOutro;
	float Alpha;

	float Timer;
};

