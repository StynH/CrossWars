#pragma once
#include "Singleton.h"
#include <unordered_map>
#include "Character.h"
#include <json.hpp>
#include "ItemManager.h"
#include <string>
#include "UnitClass.h"
#include "TextureManager.h"
#include "ArenaScreen.h"

class UnitManager: public TSingleton<UnitManager>
{
public:
	UnitManager();
	~UnitManager();

	void LoadClasses();
	void AddClass(std::string _Name, UnitClass* _pClass);
	Unit* LoadUnit(std::string _id, Unit::ENTITY_ALLIANCE _alliance, bool _isboss);
	UnitClass* GetUnitClass(std::string _ID);

	void LoadCharacters();

	void AddCharacter(Character* _character);
	Character* GetCharacter(std::string _ID);
	Character* LoadCharacter(std::string _ID);
	void LoadCostumes(std::string _ID);
	void AddCostume(std::string _ID, Unit::Costume* _newCostume);
	Unit::Costume* LoadCostume(std::string _ID, std::string _CostumeName);
	UnitSkill* GetSkill(std::string _ID);

	Unit* GenerateUnit(ArenaScreen::DIFFICULTY _Difficulty, int _MedianLevel);

	std::string GetName(std::string _ID){ return CharacterNames[_ID]; }

	sf::Texture* GetEnemyUnitTexture() { return EnemyUnitTexture; }
	sf::Texture* GetPlayerUnitTexture() { return PlayerUnitTexture; }
	sf::Texture* GetPartnerUnitTexture() { return PartnerUnitTexture; }
	sf::Texture* GetHealthBarTexture() { return HealthBarTexture; }

	UnitClass* GetRandomClass();
private:
	std::unordered_map<std::string, std::vector<Unit::Costume*>> Costumes;

	std::unordered_map<std::string, Character*> CharacterList;
	std::unordered_map<std::string, std::string> CharacterNames;
	std::unordered_map<std::string, UnitClass*> UnitClassList;
	const std::string UnitManager::MAINCLASSFILE = Folders::ROOT_FOLDER + Folders::BATTLE_FOLDER + "classes.json";

	sf::Texture* EnemyUnitTexture;
	sf::Texture* PlayerUnitTexture;
	sf::Texture* PartnerUnitTexture;

	sf::Texture* HealthBarTexture;
};

