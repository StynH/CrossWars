#pragma once
#include <SFML/System/Clock.hpp>

class GameTime
{
public:
	static void Update();
	static void Init();
	static float DeltaTime;
	static float Time;
private:
	static sf::Clock DeltaClock;
	static sf::Clock TimeClock;
};