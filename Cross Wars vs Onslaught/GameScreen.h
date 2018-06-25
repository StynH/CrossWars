#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class GameScreen
{
public:
	GameScreen(sf::RenderWindow* _pRenderWindow);
	virtual ~GameScreen();

	template<typename T> void GameScreen::SafeDelete(T*& a) {
		if (a) {
			delete a;
			a = NULL;
		}
	}

	virtual void Start() {}
	virtual void Update() {}
	virtual void Draw() = 0;
	virtual void Unload() = 0;
	virtual void PollInput();

	bool MustUnload;
	bool CloseGame;
protected:
	sf::RenderWindow* pWindow;
};

