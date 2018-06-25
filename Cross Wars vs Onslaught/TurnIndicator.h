#pragma once
#include "MovingSprite.h"
#include "GameTime.h"

class TurnIndicator : public MovingSprite
{
public:
	const float MAXSPEED = 2000;
	const float MINSPEED = 60;

	TurnIndicator(sf::RenderWindow* _pWindow, sf::Sprite* _pSprite, float _EndTime) : MovingSprite{_pWindow, _pSprite, _EndTime}
	{
		pWindow = _pWindow;
		pSprite = _pSprite;
		EndTime = _EndTime;
		Speed = MAXSPEED;
	}

	float Speed;

	void Start() override
	{
		pSprite->setPosition(512 + pSprite->getLocalBounds().width, 128 - pSprite->getLocalBounds().height / 2);
		Timer = 0;
		IsRunning = true;
	};

	void Update() override
	{
		if(pSprite->getPosition().x > -pSprite->getLocalBounds().width)
		{
			if (pSprite->getPosition().x < (512 - pSprite->getLocalBounds().width) && pSprite->getPosition().x >= 0)
			{
				Speed = MINSPEED;
			}
			else
			{
				Speed = MAXSPEED;
			}
			pSprite->move(-GameTime::DeltaTime * Speed, 0);
		}else
		{
			End();
		}

		Timer += GameTime::DeltaTime;
	};

	void Draw() override
	{
		pWindow->draw(*pSprite);
	}

	void End() override
	{
		IsRunning = false;
	};
};
