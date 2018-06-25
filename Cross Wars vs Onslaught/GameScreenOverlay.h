#pragma once
#include "GameScreen.h"

class GameScreenOverlay : public GameScreen
{
public:
	GameScreenOverlay(sf::RenderWindow* _pWindow);
	~GameScreenOverlay();

	virtual void Load() = 0;
	virtual void Hide(){};

	bool IsHiding;
	bool DeselectUnitAfterClose;
	bool CloseScreen;
};

