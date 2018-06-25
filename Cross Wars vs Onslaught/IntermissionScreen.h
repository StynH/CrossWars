#pragma once
#include "GameScreen.h"
#include "CRTV.h"
#include <queue>
class IntermissionScreen : public GameScreen
{
public:
	IntermissionScreen(sf::RenderWindow* _pRenderWindow);
	~IntermissionScreen();

	void Start() override;
	void Update() override;
	void Draw() override;
	void Unload() override;

	void LoadIntermission(std::string _IntermissionName);
private:
	CRTV* pBattleTransitionShader;

	float Timer;

	bool FinalWait;

	std::queue<sf::Sprite*> SlideQueue;
	sf::RenderTexture* pRenderTexture;
};

