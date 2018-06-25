#include "Map.h"
#include <iostream>

Map::Map(sf::Sprite* _MapSprite)
{
	MapSprite = _MapSprite;
	Dimensions = sf::Vector2i(_MapSprite->getGlobalBounds().width, _MapSprite->getGlobalBounds().height);
}

Map::~Map()
{
	for(Unit* pUnit : EnemyUnits)
	{
		delete pUnit;
	}
	EnemyUnits.clear();

	for (Unit* pUnit : PartnerUnits)
	{
		delete pUnit;
	}
	PartnerUnits.clear();

	for (Spawnpoint* pSpawnpoint : Spawnpoints)
	{
		delete pSpawnpoint;
	}
	Spawnpoints.clear();

	for(std::vector<Tile*> pTileVector : CollisionLayer)
	{
		for(Tile* pTile : pTileVector)
		{
			delete pTile;
		}
		pTileVector.clear();
	}
	CollisionLayer.clear();
}

void Map::AddUnit(Unit* _pUnit, Unit::ENTITY_ALLIANCE _alliance)
{
	switch(_alliance)
	{
	case Unit::ENTITY_ALLIANCE::PLAYER: 
		PlayerUnits.push_back(_pUnit);
		break;
	case Unit::ENTITY_ALLIANCE::ENEMY:
		EnemyUnits.push_back(_pUnit);
		break;
	case Unit::ENTITY_ALLIANCE::PARTNER: 
		PartnerUnits.push_back(_pUnit);
		break;
	default: 
		PlayerUnits.push_back(_pUnit);
		break;
	}
}

bool Map::PosIsWalkable(sf::Vector2f _pos)
{
	//X and Y are switched here because of how the collision is loaded from JSON.
	return CollisionLayer[_pos.y][_pos.x]->IsPassable();
}

void Map::SetTriggerStates(int _familyID, bool activate)
{
	for(Trigger* pTrigger : Triggers)
	{
		if(pTrigger->GetFamilyID() == _familyID)
		{
			if(activate)
			{
				pTrigger->Activate();
			}
			else
			{
				pTrigger->Deactivate();
			}
		}
	}
}

Trigger* Map::OnTrigger(sf::Vector2f _Position)
{
	for (Trigger* pTrigger : Triggers)
	{
		if(pTrigger->IsTriggerActive() && pTrigger->getPosition().x == _Position.x && pTrigger->getPosition().y == _Position.y)
		{
			return pTrigger;
		}
	}
	return nullptr;
}

Spawnpoint* Map::GetFreeSpawnzone()
{
	for (auto SpawnPoint : Spawnpoints) {
		if(!SpawnPoint->IsTaken)
		{
			SpawnPoint->IsTaken = true;
			return SpawnPoint;
		}
	}
}
