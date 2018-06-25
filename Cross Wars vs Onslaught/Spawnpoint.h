#pragma once
#include <SFML/System/Vector2.hpp>

class Spawnpoint
{
public:
	Spawnpoint(sf::Vector2f _Position)
	{
		Position = _Position;
		IsTaken = false;
	};

	~Spawnpoint()
	{
		
	}

	sf::Vector2f Position;
	bool IsTaken;

};
