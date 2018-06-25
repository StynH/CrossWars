#pragma once
#include <vector>
#include "Tile.h"
#include <stack>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <queue>
#include <memory>

class MovementTile
{
public:
	sf::Vector2f DataPosition;
	int x, y; //Position in the world
	int DataX, DataY; //Position in the Vector
	bool CanMoveTo;
	bool IsPassable;
	bool Visited;
	bool HasEnemy;
	int Distance;

	MovementTile(int _x, int _y, int _DataX, int _DataY, bool _IsPassable)
	{
		x = _x;
		y = _y;
		DataPosition = sf::Vector2f(_x, _y);
		DataX = _DataX;
		DataY = _DataY;
		IsPassable = _IsPassable;
		Visited = false;
		CanMoveTo = false;
		HasEnemy = false;
		Distance = 0;
	}

	~MovementTile()
	{}
};

class FloodFill
{
public:
	static std::vector<std::vector<MovementTile*>> TileList;
	static std::vector<sf::Vector2f> EnemyList;
	static int MoveDistance;

	static std::vector<MovementTile*> FloodFillTiles(const std::vector<std::vector<Tile*>> &_Tiles, sf::Vector2f _Origin, int _MoveDistance, const std::vector<sf::Vector2f> &_EnemyPos);

	//Flood fill using recursion.
	static void Fill(MovementTile* _tile, int _x, int _y);

	//Flood fill using a queue.
	static void QueueFill(MovementTile* _tile);
};
