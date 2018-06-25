#include "GameTime.h"
#include <iostream>

float GameTime::DeltaTime = 0;
float GameTime::Time = 0;
sf::Clock GameTime::DeltaClock = sf::Clock();
sf::Clock GameTime::TimeClock = sf::Clock();

void GameTime::Update()
{
	DeltaTime = DeltaClock.restart().asSeconds();
	Time = TimeClock.getElapsedTime().asSeconds();
}

void GameTime::Init()
{
	DeltaClock.restart();
	TimeClock.restart();
}
