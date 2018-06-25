#include "MapManager.h"
#include <json.hpp>
#include "TextureManager.h"
#include "UnitManager.h"
#include "EventLogger.h"
#include "KillAllObjective.h"
#include "KillBossObjective.h"

MapManager::MapManager()
{
	
}


MapManager::~MapManager()
{
}

Map* MapManager::LoadMap(std::string _id)
{
	Map* NewMap = new Map(new sf::Sprite(*TextureManager::GetInstance()->LoadTexture(_id + "\\map.png", Folders::FolderType::LEVELS)));

	std::string FileDir = LEVEL_FOLDER + _id + "\\data.json";

	std::ifstream DataFile(FileDir);
	
	EventContext EC("Loading Map: ", &FileDir);

	nlohmann::json ParsedJson = nlohmann::json::parse(DataFile);

	if (ParsedJson["name"].is_string())
	{
		NewMap->Name = ParsedJson["name"].get<std::string>();
	}
	
	if (ParsedJson["description"].is_string())
	{
		NewMap->Description = ParsedJson["description"].get<std::string>();
	}

	if (ParsedJson["style"].is_string())
	{
		NewMap->Style = ParsedJson["style"].get<std::string>();
	}

	int x = 0;
	int y = 0;

	if (ParsedJson["collision"].is_array())
	{
		for (auto Row : ParsedJson["collision"])
		{
			std::vector<Tile*> newRow;

			for (auto RawTile : Row)
			{
				newRow.push_back(new Tile(x, y, RawTile.get<int>()));	

				y += 1;
			}
			NewMap->GetCollisionLayer()->push_back(newRow);

			++x;
			y = 0;
		}
	}
	else
	{
		EventContext EX("\tThere is no collision layer.", "NO_COLLISION_LAYER: ");
		EventLogger::GetInstance()->Dump();
	}

	if(ParsedJson["enemies"].is_array())
	{
		for (auto EnemyUnit : ParsedJson["enemies"])
		{
			std::string UnitID = EnemyUnit["id"].get<std::string>();
			int UnitX = EnemyUnit["x"].get<int>();
			int UnitY = EnemyUnit["y"].get<int>();

			Unit* NewUnit = UnitManager::GetInstance()->LoadUnit(UnitID, Unit::ENTITY_ALLIANCE::ENEMY, EnemyUnit["boss"].get<bool>());
			NewUnit->setPosition(sf::Vector2f(UnitX * 32, UnitY * 32), sf::Vector2f(UnitX, UnitY));

			std::string AIType = EnemyUnit["ai"].get<std::string>();

			if(AIType == "IDLE")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::IDLE);
			}
			else if (AIType == "SENTRY")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::SENTRY);
			}
			else if (AIType == "GROUPUP")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::GROUPUP);
			}
			else if (AIType == "TARGET")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::TARGET);
				NewUnit->AIPositionTarget = sf::Vector2i(EnemyUnit["target"]["x"].get<int>(), EnemyUnit["target"]["y"].get<int>());
			}
			else if (AIType == "CHASER")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::CHASER);
			}

			NewMap->AddUnit(NewUnit, Unit::ENTITY_ALLIANCE::ENEMY);
		}
	}

	if (ParsedJson["partners"].is_array())
	{
		for (auto PartnerUnit : ParsedJson["partners"])
		{
			std::string UnitID = PartnerUnit["id"].get<std::string>();
			int UnitX = PartnerUnit["x"].get<int>();
			int UnitY = PartnerUnit["y"].get<int>();

			Unit* NewUnit = UnitManager::GetInstance()->LoadUnit(UnitID, Unit::ENTITY_ALLIANCE::PARTNER, false);
			NewUnit->setPosition(sf::Vector2f(UnitX * 32, UnitY * 32), sf::Vector2f(UnitX, UnitY));

			std::string AIType = PartnerUnit["ai"].get<std::string>();

			if (AIType == "IDLE")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::IDLE);
			}
			else if (AIType == "SENTRY")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::SENTRY);
			}
			else if (AIType == "GROUPUP")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::GROUPUP);
			}
			else if (AIType == "TARGET")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::TARGET);
				NewUnit->AIPositionTarget = sf::Vector2i(PartnerUnit["target"]["x"].get<int>(), PartnerUnit["target"]["y"].get<int>());
			}
			else if (AIType == "CHASER")
			{
				NewUnit->setAIType(Unit::ENTITY_AI_TYPE::CHASER);
			}

			NewMap->AddUnit(NewUnit, Unit::ENTITY_ALLIANCE::PARTNER);
		}
	}

	if (ParsedJson["spawnpoints"].is_array())
	{
		for (auto SpawnPoint : ParsedJson["spawnpoints"])
		{
			NewMap->AddSpawnPoint(new Spawnpoint(sf::Vector2f(SpawnPoint["x"].get<int>(), SpawnPoint["y"].get<int>())));
		}
	}
	else
	{
		EventContext EX("\tNo Spawnpoints found! (obviously)", "NO_SPAWNPOINTS_FOUND: ");
	}

	if (ParsedJson["triggers"].is_array())
	{
		for (auto TextTrigger : ParsedJson["triggers"])
		{
			NewMap->AddTrigger(new Trigger(sf::Vector2f(TextTrigger["x"].get<int>(), TextTrigger["y"].get<int>()), TextTrigger["familyid"].get<int>(), TextTrigger["scriptid"].get<std::string>()));
		}
	}

	if (ParsedJson["objective"].is_object())
	{
		std::string Type = ParsedJson["objective"]["type"].get<std::string>();
		MapObjective* Objective = nullptr;

		if(Type == "KILLALL")
		{
			Objective = new KillAllObjective(NewMap);
		}
		else if(Type == "KILLBOSS")
		{
			std::string Target = ParsedJson["objective"]["target"].get<std::string>();
			auto it = std::find_if(
				NewMap->EnemyUnits.begin(), 
				NewMap->EnemyUnits.end(), 
				[&Target](Unit* obj) {return obj->getID() == Target; }
			);
			
			if (it != NewMap->EnemyUnits.end())
			{
				Unit* pTarget = *it;
				Objective = new KillBossObjective(pTarget, NewMap);
				EventContext("This map's target is: ", &Target);
			}
			else
			{
				EventContext EX("Missing target: ", &Target);
				EventLogger::GetInstance()->Dump();
			}
		}

		NewMap->Objective = std::shared_ptr<MapObjective>(Objective);
	}

	NewMap->LevelMusic = ParsedJson["levelmusic"].get<std::string>();
	NewMap->BattleMusic = ParsedJson["battlemusic"].get<std::string>();
	NewMap->BossMusic = ParsedJson["bossmusic"].get<std::string>();

	NewMap->Description = ParsedJson["description"].get<std::string>();

	return NewMap;
}
