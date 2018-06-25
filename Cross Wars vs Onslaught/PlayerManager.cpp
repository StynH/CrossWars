#include <json.hpp>
#include "PlayerManager.h"
#include "UnitManager.h"
#include "FlagManager.h"
#include "InnerMight.h"
#include "UltimateLeader.h"
#include "StateMachine.h"
#include "CharScreen.h"
#include "EventLogger.h"
#include <boost/algorithm/string.hpp>

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::SwitchUnitFromTeams(Unit* _pUnit)
{
	if (_pUnit->InActiveSquad)
	{
		std::vector<Unit*>::iterator it = TeamUnits.begin();
		for (; it != TeamUnits.end(); ) {
			Unit* CurrentUnit = *it;
			if (CurrentUnit->getGUID() == _pUnit->getGUID()) {
				AddInactiveUnit(_pUnit);
				it = TeamUnits.erase(it);
				break;
			}
			++it;
		}
	}
	else
	{
		std::vector<Unit*>::iterator it = InactiveUnits.begin();
		for (; it != InactiveUnits.end(); ) {
			Unit* CurrentUnit = *it;
			if (CurrentUnit->getGUID() == _pUnit->getGUID()) {
				AddReadyUnit(_pUnit);
				it = InactiveUnits.erase(it);
				break;
			}
			++it;
		}
	}
}

void PlayerManager::SwitchUnitFromArena(Unit* _pUnit)
{
	if (_pUnit->InArenaSquad)
	{
		std::vector<Unit*>::iterator it = ArenaUnits.begin();
		for (; it != ArenaUnits.end(); ) {
			Unit* CurrentUnit = *it;
			if (CurrentUnit->getGUID() == _pUnit->getGUID()) {
				_pUnit->InArenaSquad = false;
				it = ArenaUnits.erase(it);
				break;
			}
			++it;
		}
	}
	else
	{
		_pUnit->InArenaSquad = true;
		ArenaUnits.push_back(_pUnit);
	}
}

Unit* PlayerManager::FindUnit(std::string _GUID)
{
	for(Unit* pUnit : TeamUnits)
	{
		if(pUnit->getGUID() == _GUID)
		{
			return pUnit;
		}
	}

	for (Unit* pUnit : InactiveUnits)
	{
		if (pUnit->getGUID() == _GUID)
		{
			return pUnit;
		}
	}

	return nullptr;
}

int PlayerManager::GetEnergyUnits()
{
	return EnergyAmount;
}

void PlayerManager::AddEnergyUnits(int _Amount)
{
	EnergyAmount += _Amount;
}

void PlayerManager::DeductEnergyUnits(int _Amount)
{
	EnergyAmount -= _Amount;
}

void PlayerManager::HealPlayerUnits()
{
	for(Unit* pUnit : TeamUnits)
	{
		pUnit->setHP(pUnit->getMAXHP());
	}
}

void PlayerManager::AddReadyUnit(Unit* _pUnit)
{
	_pUnit->InActiveSquad = true;
	TeamUnits.push_back(_pUnit);
}

void PlayerManager::AddInactiveUnit(Unit* _pUnit)
{
	_pUnit->InActiveSquad = false;
	InactiveUnits.push_back(_pUnit);
}

void PlayerManager::AddAvailableMission(PreloadChapter _Preload)
{
	AvailableMissions.push_back(_Preload);
}

void PlayerManager::Load()
{
	FlagManager::LoadFlags();
}

void PlayerManager::NewGame()
{
	Unit* Player = new Unit(1, 6, 4, 6, 4, 5, 19, Unit::ENTITY_ALLIANCE::PLAYER);
	Player->setID("0xPLAYER");
	Player->setCHAR(UnitManager::GetInstance()->GetCharacter("player"));
	Player->getCHAR()->setName(PlayerName);
	Player->setCLASS(UnitManager::GetInstance()->GetUnitClass("FIGHTER"));
	Player->CreateUnitSprite(Unit::ENTITY_ALLIANCE::PLAYER);

	Player->addItemToInventory(ItemManager::GetInstance()->GetItem("0x0000A2"));
	Player->addItemToInventory(ItemManager::GetInstance()->GetItem("0x000HO1"));
	Player->addSkill(new UltimateLeader(), false);
	Player->addSkill(new InnerMight(), false);

	AddReadyUnit(Player);

	ScreenManager::GetInstance()->GetItemCreationScreen()->AddItem("0x000CR1");
	ScreenManager::GetInstance()->GetItemCreationScreen()->AddItem("0x000HO1");

	ItemManager::GetInstance()->AddItemToStorage(ItemManager::GetInstance()->GetItem("0x000HO1"));
	ItemManager::GetInstance()->AddItemToStorage(ItemManager::GetInstance()->GetItem("0x000HO1"));
	ItemManager::GetInstance()->AddItemToStorage(ItemManager::GetInstance()->GetItem("0x000HO1"));
	ItemManager::GetInstance()->AddItemToStorage(ItemManager::GetInstance()->GetItem("0x000HO1"));

	EnergyAmount = 12500;

	StateMachine::GetInstance()->LoadChapter(LuaManager::GetInstance()->LoadChapter("intro"));
}


void PlayerManager::LoadSaveFile()
{
	std::ifstream file("save.txt");
	nlohmann::json j = nlohmann::json::parse(file);

	PlayerName = j["playername"].get<std::string>();
	EnergyAmount = j["energy"].get<int>();

	for (nlohmann::json::iterator it = j["activeunits"].begin(); it != j["activeunits"].end(); ++it) {
		auto UnitInfo = *it;
		Unit* pUnit = new Unit(Unit::PLAYER);

		pUnit->setLVL(UnitInfo["LVL"].get<int>());
		pUnit->setEXP(UnitInfo["EXP"].get<int>());
		pUnit->setHP(UnitInfo["HP"].get<int>());
		pUnit->setMAXHP(UnitInfo["HP"].get<int>());
		pUnit->setATK(UnitInfo["ATK"].get<int>());
		pUnit->setDEF(UnitInfo["DEF"].get<int>());
		pUnit->setSPD(UnitInfo["SPD"].get<int>());
		pUnit->setMOV(UnitInfo["MOV"].get<int>());
		pUnit->setLCK(UnitInfo["LCK"].get<int>());

		pUnit->setArenaKills(UnitInfo["ARENAKILLS"].get<int>());

		pUnit->setCHAR(UnitManager::GetInstance()->GetCharacter(UnitInfo["CHAR"].get<std::string>()));
		pUnit->setCLASS(UnitManager::GetInstance()->GetUnitClass(boost::to_upper_copy<std::string>(UnitInfo["CLASS"].get<std::string>())));

		if(UnitInfo["CHAR"].get<std::string>() == "player")
		{
			pUnit->getCHAR()->setName(PlayerName);
		}

		for (nlohmann::json::iterator invit = UnitInfo["INV"].begin(); invit != UnitInfo["INV"].end(); ++invit) {
			pUnit->addItemToInventory(ItemManager::GetInstance()->GetItem(invit->get<std::string>()));
		}

		for (nlohmann::json::iterator skillit = UnitInfo["SKILLS"].begin(); skillit != UnitInfo["SKILLS"].end(); ++skillit) {
			pUnit->addSkill(UnitManager::GetInstance()->GetSkill(skillit->get<std::string>()), true);
		}

		if (pUnit->getCLASS())
		{
			pUnit->CreateUnitSprite(Unit::PLAYER);
		}

		AddReadyUnit(pUnit);
	}

	for (nlohmann::json::iterator it = j["inactiveunits"].begin(); it != j["inactiveunits"].end(); ++it) {
		auto UnitInfo = *it;
		Unit* pUnit = new Unit(Unit::PLAYER);

		pUnit->setLVL(UnitInfo["LVL"].get<int>());
		pUnit->setEXP(UnitInfo["EXP"].get<int>());
		pUnit->setHP(UnitInfo["HP"].get<int>());
		pUnit->setMAXHP(UnitInfo["HP"].get<int>());
		pUnit->setATK(UnitInfo["ATK"].get<int>());
		pUnit->setDEF(UnitInfo["DEF"].get<int>());
		pUnit->setSPD(UnitInfo["SPD"].get<int>());
		pUnit->setMOV(UnitInfo["MOV"].get<int>());
		pUnit->setLCK(UnitInfo["LCK"].get<int>());

		pUnit->setArenaKills(UnitInfo["ARENAKILLS"].get<int>());

		pUnit->setCHAR(UnitManager::GetInstance()->GetCharacter(UnitInfo["CHAR"].get<std::string>()));
		pUnit->setCLASS(UnitManager::GetInstance()->GetUnitClass(boost::to_upper_copy<std::string>(UnitInfo["CLASS"].get<std::string>())));

		if (UnitInfo["CHAR"].get<std::string>() == PlayerName)
		{
			pUnit->getCHAR()->setName(PlayerName);
		}

		for (nlohmann::json::iterator invit = UnitInfo["INV"].begin(); invit != UnitInfo["INV"].end(); ++invit) {
			pUnit->addItemToInventory(ItemManager::GetInstance()->GetItem(invit->get<std::string>()));
		}

		for (nlohmann::json::iterator invit = UnitInfo["SKILLS"].begin(); invit != UnitInfo["SKILLS"].end(); ++invit) {
			pUnit->addSkill(UnitManager::GetInstance()->GetSkill(it->get<std::string>()), true);
		}

		if (pUnit->getCLASS())
		{
			pUnit->CreateUnitSprite(Unit::PLAYER);
		}

		AddInactiveUnit(pUnit);
	}

	for (nlohmann::json::iterator it = j["createableitems"].begin(); it != j["createableitems"].end(); ++it) {
		ScreenManager::GetInstance()->GetItemCreationScreen()->AddItem(it->get<std::string>());
	}

	for (nlohmann::json::iterator it = j["storage"].begin(); it != j["storage"].end(); ++it) {
		ItemManager::GetInstance()->AddItemToStorage(ItemManager::GetInstance()->GetItem(it->get<std::string>()));
	}

	for (nlohmann::json::iterator it = j["chapters"].begin(); it != j["chapters"].end(); ++it) {
		AddAvailableMission(LuaManager::GetInstance()->GetChapterInfo(it->get<std::string>()));
	}

	for (nlohmann::json::iterator it = j["trueflags"].begin(); it != j["trueflags"].end(); ++it) {
		FlagManager::SetFlag(it->get<std::string>());
	}

	EventContext EC("Game Succesfully loaded: ", "save.txt");
}

void PlayerManager::Reset()
{
	
}

void PlayerManager::CreateSaveFile()
{
	nlohmann::json JSON;

	JSON["playername"] = PlayerName;

	JSON["activeunits"] = {};
	for(Unit* pUnit : TeamUnits)
	{
		nlohmann::json JUnit = {
			{ "CHAR", pUnit->getCHAR()->GetNAME() == PlayerName ? "player" : pUnit->getCHAR()->GetID() },
			{ "CLASS", pUnit->getCLASS()->GetID() },
			{ "LVL", pUnit->getLVL() },
			{ "HP", pUnit->getMAXHP() },
			{ "ATK", pUnit->getATK() },
			{ "DEF", pUnit->getDEF() },
			{ "MOV", pUnit->getMOV() },
			{ "SPD", pUnit->getSPD() },
			{ "LCK", pUnit->getLCK() },
			{ "EXP", pUnit->getEXP() },
			{ "SKIN", pUnit->getCostume()->Name }
		};

		JUnit["INV"] = {};
		for (Item* pItem : pUnit->getInventory())
		{
			JUnit["INV"].push_back(pItem->getID());
		}

		JUnit["SKILLS"] = {};
		for (UnitSkill* pSkill : pUnit->getSkills())
		{
			JUnit["SKILLS"].push_back(pSkill->GetName());
		}

		JUnit["ARENAKILLS"] = pUnit->getArenaKills();

		JSON["activeunits"].push_back(JUnit);
	}

	JSON["inactiveunits"] = {};
	for (Unit* pUnit : InactiveUnits)
	{
		nlohmann::json JUnit = {
			{ "CHAR", pUnit->getCHAR()->GetNAME() == PlayerName ? "player" : pUnit->getCHAR()->GetID() },
			{ "CLASS", pUnit->getCLASS()->GetID() },
			{ "LVL", pUnit->getLVL() },
			{ "HP", pUnit->getMAXHP() },
			{ "ATK", pUnit->getATK() },
			{ "DEF", pUnit->getDEF() },
			{ "MOV", pUnit->getMOV() },
			{ "SPD", pUnit->getSPD() },
			{ "LCK", pUnit->getLCK() },
			{ "EXP", pUnit->getEXP() },
		};

		JUnit["INV"] = {};
		for (Item* pItem : pUnit->getInventory())
		{
			JUnit["INV"].push_back(pItem->getID());
		}

		JUnit["SKILLS"] = {};
		for (UnitSkill* pSkill : pUnit->getSkills())
		{
			JUnit["SKILLS"].push_back(pSkill->GetName());
		}

		JUnit["ARENAKILLS"] = pUnit->getArenaKills();

		JSON["inactiveunits"].push_back(JUnit);
	}

	JSON["chapters"] = {};
	for(PreloadChapter Chapter : GetAvailableMissions())
	{
		JSON["chapters"].push_back(Chapter.ID);
	}

	JSON["createableitems"] = {};
	for (CreatableItem* Item : ScreenManager::GetInstance()->GetItemCreationScreen()->GetCreateableItems())
	{
		JSON["createableitems"].push_back(Item->ItemID);
	}

	JSON["storage"] = {};
	for (Item* pItem : ItemManager::GetInstance()->GetItemStorage())
	{
		JSON["storage"].push_back(pItem->getID());
	}

	JSON["energy"] = EnergyAmount;

	JSON["trueflags"] = {};
	JSON["falseflags"] = {};
	typedef std::map<std::string, GameFlag*>::iterator it_type;
	std::map<std::string, GameFlag*> Flags = FlagManager::GetFlags();
	for (it_type iterator = Flags.begin(); iterator != Flags.end(); ++iterator) {
		if(iterator->second->IsFlagSet())
		{
			JSON["trueflags"].push_back(iterator->first);
		}
	}

	std::string JSONSave = JSON.dump(4);

	std::ofstream newFile("save.txt");

	if (newFile.is_open())
	{
		newFile << JSONSave;
	}
	
	newFile.close(); 

	EventContext EC("Game Succesfully saved: ", "save.txt");
}

void PlayerManager::AddCreateableItem(std::string _ItemID)
{
	ScreenManager::GetInstance()->GetItemCreationScreen()->AddItem(_ItemID);
}
