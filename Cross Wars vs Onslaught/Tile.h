#pragma once
#include <vector>

class Tile
{
public:
	static const int TILESIZE = 32;

	Tile(int x, int y, bool isPassable);
	~Tile();

	bool IsPassable() { return isPassable; }

	int x, y;
private:
	bool isPassable;
};

