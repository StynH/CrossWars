#include "UnitManager.h"
#include "TextureManager.h"
#include "EventLogger.h"

#include "boost/filesystem.hpp"
#include "TextSanitizer.h"
#include "InnerMight.h"
#include "UltimateLeader.h"
using namespace boost::filesystem;

UnitManager::UnitManager()
{
	PlayerUnitTexture = TextureManager::GetInstance()->LoadTexture("mapicons\\playerclasses.png", Folders::FolderType::BATTLE);
	EnemyUnitTexture = TextureManager::GetInstance()->LoadTexture("mapicons\\enemyclasses.png", Folders::FolderType::BATTLE);
	PartnerUnitTexture = TextureManager::GetInstance()->LoadTexture("mapicons\\partnerclasses.png", Folders::FolderType::BATTLE);
	HealthBarTexture = TextureManager::GetInstance()->LoadTexture("chardisplay\\healthbar.png", Folders::FolderType::HUD);
}


UnitManager::~UnitManager()
{
	delete PlayerUnitTexture;
	delete PartnerUnitTexture;
	delete EnemyUnitTexture;

	CharacterList.clear();
	UnitClassList.clear();
}

void UnitManager::LoadClasses()
{
	std::ifstream file(MAINCLASSFILE);
	nlohmann::json j = nlohmann::json::parse(file);

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		UnitClass* newClass = new UnitClass(
			it.value()["ATK"].get<int>(), 
			it.value()["DEF"].get<int>(), 
			it.value()["SPD"].get<int>(), 
			it.value()["LCK"].get<int>(), 
			it.value()["HP"].get<int>(), 
			it.value()["NAME"].get<std::string>(), 
			it.key(),
			sf::Rect<int>(it.value()["SPRITE"]["x"].get<int>() * 32, it.value()["SPRITE"]["y"].get<int>() * 32, 32, 32)
			);
		AddClass(it.key(), newClass);
	}

	std::cout << "All unit classes loaded!" << std::endl;
}

void UnitManager::AddClass(std::string _Name, UnitClass* _pClass)
{
	UnitClassList.insert({ _Name, _pClass });
}

Unit* UnitManager::LoadUnit(std::string _id, Unit::ENTITY_ALLIANCE _alliance, bool _isboss)
{
	Unit* NewUnit = LuaManager::GetInstance()->LoadUnit(_id, _alliance);
	NewUnit->IsBoss = _isboss;
	return NewUnit;
}

UnitClass* UnitManager::GetUnitClass(std::string _ID)
{
	auto search = UnitClassList.find(_ID);
	if (search != UnitClassList.end()) {
		return (UnitClass*)search->second;
	}
	return nullptr;
}

void UnitManager::LoadCharacters()
{
	//Thanks again Boost, you made it alot easier for me.
	path Path(Folders::ROOT_FOLDER + Folders::HUD_FOLDER + "characters");
	directory_iterator EndItr;

	//Iterate through every folder file in the character folder.
	for (directory_iterator itr(Path); itr != EndItr; ++itr)
	{
		//Only if it's a directory though.
		if (is_directory(itr->path())) {
			AddCharacter(LoadCharacter(itr->path().stem().string()));
		}
	}

	std::cout << "All character information loaded!" << std::endl;
}

void UnitManager::AddCharacter(Character* _pCharacter)
{
	CharacterList.insert({ _pCharacter->GetID(), _pCharacter });
}

Character* UnitManager::GetCharacter(std::string _ID)
{
	auto search = CharacterList.find(_ID);
	if (search != CharacterList.end()) {
		return static_cast<Character*>(search->second);
	}

	return new Character(CharacterList["unknown"]);
}

Character* UnitManager::LoadCharacter(std::string _ID)
{
	Character* newChar = new Character();

	EventContext EC("Loading Character: ", &_ID);

	std::ifstream file(Folders::ROOT_FOLDER + Folders::HUD_FOLDER + "characters\\" + _ID + "\\char.json");
	nlohmann::json j = nlohmann::json::parse(file);
	newChar->setName(j["Name"].get<std::string>());
	newChar->setDescription(j["Description"].get<std::string>());

	newChar->setId(_ID);
	newChar->SetATKGrowth(j["ATK"].get<int>());
	newChar->SetDEFGrowth(j["DEF"].get<int>());
	newChar->SetSPDGrowth(j["SPD"].get<int>());
	newChar->SetLCKGrowth(j["LCK"].get<int>());
	newChar->SetHPGrowth(j["HP"].get<int>());

	sf::Texture* CharacterTexture = TextureManager::GetInstance()->LoadTexture("characters\\" + _ID + "\\char.png", Folders::FolderType::HUD);

	newChar->AttackSprite = new CharacterSprite(CharacterTexture, sf::IntRect(0, 0, 256, 134));

	sf::Texture* BattleSprite = TextureManager::GetInstance()->LoadTexture("characters\\" + _ID + "\\battle.png", Folders::FolderType::HUD);

	newChar->BattleSprite = new CharacterSprite(BattleSprite, sf::Rect<int>(0, 0, BattleSprite->getSize().x, BattleSprite->getSize().y));

	newChar->DialougeSprite = new CharacterSprite(CharacterTexture, sf::IntRect(0, 290, 284, 256));
	newChar->DialougeSprite->Reversed->setPosition(512 - newChar->DialougeSprite->Reversed->getTexture()->getSize().x, 0);

	newChar->SupportSprite = new CharacterSprite(CharacterTexture, sf::IntRect(116, 134, 168, 42));
	
	newChar->HUDSprite = new sf::Sprite(*CharacterTexture, sf::IntRect(0, 134, 116, 156));
	newChar->HUDSprite->setPosition(50, 49);

	LoadCostumes(_ID);

	return newChar;
}

void UnitManager::LoadCostumes(std::string _ID)
{
	Costumes.insert_or_assign(_ID, std::vector<Unit::Costume*>());

	path Path(Folders::ROOT_FOLDER + Folders::HUD_FOLDER + "characters" + "\\" + _ID + "\\costume");
	directory_iterator EndItr;

	if (is_directory(Path)) {
		for (directory_iterator itr(Path); itr != EndItr; ++itr)
		{
			AddCostume(_ID, LoadCostume(_ID, itr->path().stem().string()));
		}
	}
}

void UnitManager::AddCostume(std::string _ID, Unit::Costume* _newCostume)
{
	Costumes.at(_ID).push_back(_newCostume);
}

Unit::Costume* UnitManager::LoadCostume(std::string _ID, std::string _CostumeName)
{
	Unit::Costume* newCostume = new Unit::Costume();
	std::string Folder = "characters\\" + _ID + "\\costume\\" + _CostumeName;

	newCostume->Name = _CostumeName;

	sf::Texture* CharacterTexture = TextureManager::GetInstance()->LoadTexture(Folder + "\\char.png", Folders::FolderType::HUD);
	newCostume->AttackSprite = new CharacterSprite(CharacterTexture, sf::IntRect(0, 0, 256, 134));
	sf::Texture* BattleSprite = TextureManager::GetInstance()->LoadTexture(Folder + "\\battle.png", Folders::FolderType::HUD);
	newCostume->BattleSprite = new CharacterSprite(BattleSprite, sf::Rect<int>(0, 0, BattleSprite->getSize().x, BattleSprite->getSize().y));
	newCostume->DialougeSprite = new CharacterSprite(CharacterTexture, sf::IntRect(0, 290, 284, 256));
	newCostume->DialougeSprite->Reversed->setPosition(512 - newCostume->DialougeSprite->Reversed->getTexture()->getSize().x, 0);
	newCostume->SupportSprite = new CharacterSprite(CharacterTexture, sf::IntRect(116, 134, 168, 42));
	newCostume->HUDSprite = new sf::Sprite(*CharacterTexture, sf::IntRect(0, 134, 116, 156));
	newCostume->HUDSprite->setPosition(50, 49);

	return newCostume;
}

UnitSkill* UnitManager::GetSkill(std::string _ID)
{
	if(_ID == "Inner Might")
	{
		return new InnerMight();
	}
	
	if (_ID == "Ultimate Leader")
	{
		return new UltimateLeader();
	}

	return nullptr;
}

Unit* UnitManager::GenerateUnit(ArenaScreen::DIFFICULTY _Difficulty, int _MedianLevel)
{
	Unit* pArenaUnit = new Unit(Unit::ENEMY);
	Character* pCharacter = GetInstance()->GetCharacter("hologram");
	pArenaUnit->setCHAR(pCharacter);

	UnitClass* UnitsClass = GetInstance()->GetRandomClass();
	while (UnitsClass->GetNAME() == "Destroyer" || UnitsClass->GetNAME() == "Psychic God")
	{
		UnitsClass = GetInstance()->GetRandomClass();
	}

	pArenaUnit->setCLASS(UnitsClass);

	float Multiplier = 1.0f;
	float Enhancer = 1.0f;
	switch (_Difficulty)
	{
	case ArenaScreen::EASY:
		Multiplier = 0.5f;
		Enhancer = 1.0f;
		break;
	case ArenaScreen::NORMAL:
		Multiplier = 1.0f;
		Enhancer = 1.5f;
		break;
	case ArenaScreen::HARD:
		Multiplier = 2.0f;
		Enhancer = 2.0f;
		break;
	}

	boost::random::mt19937 Generator;
	Generator.seed(static_cast<unsigned int>(time(0)));

	boost::random::uniform_int_distribution<> LevelRange;
	switch (_Difficulty)
	{
	case ArenaScreen::EASY:
		LevelRange = boost::random::uniform_int_distribution<>(_MedianLevel - 7, _MedianLevel);
		break;
	case ArenaScreen::NORMAL:
		LevelRange = boost::random::uniform_int_distribution<>(_MedianLevel - 3, _MedianLevel + 3);
		break;
	case ArenaScreen::HARD:
		LevelRange = boost::random::uniform_int_distribution<>(_MedianLevel, _MedianLevel + 7);
		break;
	}

	int Level = LevelRange(Generator);
	if (Level < 1)
	{
		Level = 1;
	}
	else if (Level > 40)
	{
		Level = 40;
	}

	int BaseATK = (UnitsClass->GetATKGrowth() / 10.0f + pCharacter->GetATKGrowth() / 10.0f * Multiplier) / 2 * Enhancer + ((UnitsClass->GetATKGrowth() / 100.0f + pCharacter->GetATKGrowth() / 100.0f) * Level) + 1.0f;
	int BaseDEF = (UnitsClass->GetDEFGrowth() / 10.0f + pCharacter->GetDEFGrowth() / 10.0f * Multiplier) / 2 * Enhancer + ((UnitsClass->GetDEFGrowth() / 100.0f + pCharacter->GetDEFGrowth() / 100.0f) * Level) + 1.0f;
	int BaseSPD = (UnitsClass->GetSPDGrowth() / 10.0f + pCharacter->GetSPDGrowth() / 10.0f * Multiplier) / 2 * Enhancer + ((UnitsClass->GetSPDGrowth() / 100.0f + pCharacter->GetSPDGrowth() / 100.0f) * Level) + 1.0f;
	int BaseLCK = (UnitsClass->GetLCKGrowth() / 10.0f + pCharacter->GetLCKGrowth() / 10.0f * Multiplier) / 2 * Enhancer + ((UnitsClass->GetLCKGrowth() / 100.0f + pCharacter->GetLCKGrowth() / 100.0f) * Level) + 1.0f;
	int BaseHP = (UnitsClass->GetHPGrowth() / 10.0f + pCharacter->GetHPGrowth() / 10.0f * Multiplier) / 2 * (Enhancer + 2.0f + Multiplier) + ((UnitsClass->GetHPGrowth() / 100.0f + pCharacter->GetHPGrowth() / 100.0f) * Level) + 1.0f;

	pArenaUnit->setLVL(Level);
	pArenaUnit->setATK(BaseATK);
	pArenaUnit->setDEF(BaseDEF);
	pArenaUnit->setSPD(BaseSPD);
	pArenaUnit->setLCK(BaseLCK);
	pArenaUnit->setHP(BaseHP);
	pArenaUnit->setMAXHP(BaseHP);

	pArenaUnit->CreateUnitSprite(Unit::ENEMY);
	pArenaUnit->setMOV(3 + _Difficulty);

	pArenaUnit->addItemToInventory(ItemManager::GetInstance()->GetItem("0x0000A3"));

	switch (_Difficulty)
	{
	case ArenaScreen::EASY: 
		pArenaUnit->addItemToInventory(ItemManager::GetInstance()->GetItem("0x000HO1"));
		break;
	case ArenaScreen::NORMAL: 
		pArenaUnit->addItemToInventory(ItemManager::GetInstance()->GetItem("0x000HO2"));
		break;
	case ArenaScreen::HARD: 
		pArenaUnit->addItemToInventory(ItemManager::GetInstance()->GetItem("0x000HO3"));
		break;
	}

	return pArenaUnit;
}

UnitClass* UnitManager::GetRandomClass()
{
	auto it = UnitClassList.begin();
	advance(it, rand() % UnitClassList.size());
	return it->second;
}
