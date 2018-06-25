#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include "Tile.h"
#include "Unit.h"
#include "Spawnpoint.h"
#include "MapObjective.h"
#include "Trigger.h"

class Map
{
public:
	Map(sf::Sprite* _MapSprite);
	~Map();

	void AddUnit(Unit* _pUnit, Unit::ENTITY_ALLIANCE _alliance);

	std::string Name;
	std::string Description;
	std::string Style;

	std::vector<std::vector<Tile*>>* GetCollisionLayer() { return &CollisionLayer; }
	Tile* GetTileAt(int _y, int _x) { return CollisionLayer[_y][_x]; };

	sf::Vector2i GetDimensions() { return Dimensions; }
	sf::Vector2i GetSpritePosition() { return sf::Vector2i(MapSprite->getPosition().x, MapSprite->getPosition().y); }
	sf::Sprite* GetSprite() { return MapSprite; }

	bool PosIsWalkable(sf::Vector2f _pos);

	std::vector<Unit*> EnemyUnits;
	std::vector<Unit*> PlayerUnits;
	std::vector<Unit*> PartnerUnits;

	void AddSpawnPoint(Spawnpoint* _pSpawnPoint)
	{
		Spawnpoints.push_back(_pSpawnPoint);
	};

	void AddTrigger(Trigger* _pTrigger)
	{
		Triggers.push_back(_pTrigger);
	}
	
	void SetTriggerStates(int _familyID, bool activate);
	Trigger* OnTrigger(sf::Vector2f _Position);

	Spawnpoint* GetFreeSpawnzone();
	std::shared_ptr<MapObjective> Objective;

	std::string LevelMusic;
	std::string BattleMusic;
	std::string BossMusic;

	std::string LevelDescription;
private:
	sf::Vector2i Dimensions;
	std::vector<std::vector<Tile*>> CollisionLayer;
	sf::Sprite* MapSprite;
	std::vector<Spawnpoint*> Spawnpoints;
	std::vector<Trigger*> Triggers;
};

