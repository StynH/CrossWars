#include "GameScreenOverlay.h"


GameScreenOverlay::GameScreenOverlay(sf::RenderWindow* _pWindow) : GameScreen(_pWindow)
{
	CloseScreen = false;
	IsHiding = false;
	DeselectUnitAfterClose = false;
}


GameScreenOverlay::~GameScreenOverlay()
{
}
