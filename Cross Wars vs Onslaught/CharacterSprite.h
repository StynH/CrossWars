#pragma once
#include <SFML/Graphics/Sprite.hpp>

class CharacterSprite
{
public:
	CharacterSprite(sf::Texture* _text, sf::IntRect _rect)
	{
		Normal = new sf::Sprite(*_text, _rect);

		//Reverse the normal sprite.
		Reversed = new sf::Sprite(*_text, _rect);
		Reversed->setOrigin({ Reversed->getLocalBounds().width, 0 });
		Reversed->setScale({ -1, 1 });
	};

	~CharacterSprite()
	{
		delete Normal;
		delete Reversed;
	}

	sf::Sprite* Normal;
	sf::Sprite* Reversed;
};
