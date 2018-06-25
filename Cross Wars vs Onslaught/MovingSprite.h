#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class MovingSprite
{
public:
	MovingSprite(sf::RenderWindow* _pWindow, sf::Sprite* _pSprite, float _EndTime)
	{
		pWindow = _pWindow;
		pSprite = _pSprite;
		EndTime = _EndTime;
		Timer = 0;
		IsRunning = false;
	}
	
	virtual ~MovingSprite(){};

	virtual void Start(){};
	virtual void Update(){};
	virtual void Draw(){};
	virtual void End(){};

	float Timer;
	float EndTime;
	float IsRunning;
	
protected:
	sf::Sprite* pSprite;
	sf::RenderWindow* pWindow;
};
