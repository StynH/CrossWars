#pragma once
#include "Singleton.h"
#include <vector>
#include "Tile.h"

class MapGenerator : public TSingleton<MapGenerator>
{
public:

	std::vector<Tile> GenerateMap();

};


