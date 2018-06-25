#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "Event.h"
#include <SFML/Graphics/RenderWindow.hpp>

class UIButton
{
public:
	UIButton(sf::Sprite* _pButtonImage, sf::RenderWindow* _pWindow, sf::Vector2f _Position);
	~UIButton();

	void Draw();
	void SetPosition(sf::Vector2f _Position);
	sf::Vector2f GetPosition() { return Position; }

	Event<nullptr_t> Fire;

	bool Available;
private:
	sf::Sprite* pButtonImage;
	sf::RenderWindow* pWindow;

	sf::Vector2f Position;
};

