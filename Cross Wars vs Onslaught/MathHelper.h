#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

class MathHelper
{
public:
	static int DistanceBetweenTiles(sf::Vector2f _origin, sf::Vector2f _destination)
	{
		int x = _origin.x - _destination.x;
		int y = _origin.y - _destination.y;
		int dist;

		dist = pow(x, 2) + pow(y, 2);
		dist = sqrt(dist);

		return (dist / 32);
	}

	static int CompareTo(int _currentInt, int _compareInt)
	{
		if (_currentInt > _compareInt) return 1;
		if (_currentInt == _compareInt) return 0;
		if (_currentInt < _compareInt) return -1;
		return 0;
	}

	static int RandomRange(int min, int max)
	{
		return floor((rand() / (float)RAND_MAX) * (1 + max - min) + min);
	}

	static int RandomNormalized()
	{
		return rand() / (float)RAND_MAX;
	}

};
