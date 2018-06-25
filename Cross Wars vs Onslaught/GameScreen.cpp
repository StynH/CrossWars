#include "GameScreen.h"
#include <SFML/Window/Event.hpp>
#include <iostream>

GameScreen::GameScreen(sf::RenderWindow* _pRenderWindow)
{
	pWindow = _pRenderWindow;
	MustUnload = false;
	CloseGame = false;
}


GameScreen::~GameScreen()
{
}

void GameScreen::PollInput()
{
	sf::Event event;

	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			CloseGame = true;
		}
	}
}
