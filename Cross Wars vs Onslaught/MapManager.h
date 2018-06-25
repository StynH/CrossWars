#pragma once
#include "Map.h"
#include <string>
#include <fstream>
#include "Singleton.h"

class MapManager : public TSingleton<MapManager>
{
public:
	MapManager();
	~MapManager();

	Map* LoadMap(std::string _id);

	const std::string LEVEL_FOLDER = "..\\Assets\\Levels\\";
};

