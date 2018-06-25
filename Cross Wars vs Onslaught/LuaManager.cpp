#include "LuaManager.h"
#include "ItemManager.h"
#include "Entity.h"
#include "TextureManager.h"
#include "Chapter.h"
#include "EventLogger.h"
#include "FlagManager.h"
#include "PlayerManager.h"
#include "Useable.h"

using namespace luabridge;

lua_State* LuaManager::pLuaState;
LuaManager::LuaManager()
{

}


LuaManager::~LuaManager()
{
	lua_close(GetInstance()->getLuaState());
}

void LuaManager::CreateState()
{
	//Make a new luastate.
	pLuaState = luaL_newstate();
	luaL_openlibs(pLuaState);
}

void LuaManager::Init()
{
	CreateState();

	EventContext EC("Initializing Lua Functions", "(LoadFunction)");

	bool* Debug = new bool(false);
	#ifdef _DEBUG
		Debug = new bool(true);
	#endif

	//Registers the CHAPTER class, its the backbone of the whole game basically.
	getGlobalNamespace(pLuaState)
		.beginClass<Chapter>("Chapter")
		.addConstructor<void(*)(void)>()
		.addFunction("LoadBattle", &Chapter::LoadBattle)
		.addFunction("LoadIntermission", &Chapter::LoadIntermission)
		.addFunction("LoadScene", &Chapter::LoadScene)
		.addFunction("AddUnit", &Chapter::AddUnit)
		.addFunction("AddItem", &Chapter::AddItem)
		.addFunction("UnlockMission", &Chapter::UnlockNewMission)
		.addFunction("ShowChapterScreen", &Chapter::ShowChapterScreen)
		.endClass();

	//Registers the ITEM class and WEAPON class in LUA, wich derives from ITEM, complete with attributes.
	getGlobalNamespace(pLuaState)
		.beginClass<Item>("Item")
		.addConstructor<void(*)(void)>()
		.addProperty("PRICE", &Item::getPrice, &Item::setPrice)
		.addProperty("ID", &Item::getID, &Item::setID)
		.addProperty("NAME", &Item::getName, &Item::setName)
		.endClass()
		.deriveClass<Weapon, Item>("Weapon")
		.addProperty("DMG", &Weapon::getDMG, &Weapon::setDMG)
		.addProperty("CRT", &Weapon::getCRT, &Weapon::setCRT)
		.addProperty("PERC", &Weapon::getPERC, &Weapon::setPERC)
		.addProperty("RNG", &Weapon::getRNG, &Weapon::setRNG)
		.addFunction("DEBUG", &Weapon::Debug)
		.endClass();

	//Registers the ENTITY class and UNIT class in LUA, complete with attributes.
	getGlobalNamespace(pLuaState)
		.beginClass<Unit>("Unit")
		.addConstructor<void(*)(void)>()
		.addProperty("LVL", &Unit::getLVL, &Unit::setLVL)
		.addProperty("ATK", &Unit::getATK, &Unit::setATK)
		.addProperty("DEF", &Unit::getDEF, &Unit::setDEF)
		.addProperty("SPD", &Unit::getSPD, &Unit::setSPD)
		.addProperty("LCK", &Unit::getLCK, &Unit::setLCK)
		.addProperty("MOV", &Unit::getMOV, &Unit::setMOV)
		.addProperty("HP", &Unit::getHP, &Unit::setHP)
		.addProperty("MAXHP", &Unit::getMAXHP, &Unit::setMAXHP)
		.addFunction("DEBUG", &Unit::Debug)
		.addFunction("HEAL", &Unit::addHP)
		.addFunction("INCREASEATK", &Unit::addATK)
		.addFunction("INCREASEDEF", &Unit::addDEF)
		.addFunction("INCREASESPD", &Unit::addSPD)
		.addFunction("INCREASELCK", &Unit::addLCK)
		.addFunction("INCREASEHP", &Unit::addMAXHP)
		.endClass();

	//Register the Flag Manager so we can use Flags in LUA.
	getGlobalNamespace(pLuaState)
		.addFunction("FlagSet", FlagManager::FlagSet)
		.addFunction("SetFlag", FlagManager::SetFlag)
		.addFunction("UnsetFlag", FlagManager::UnsetFlag);

	//We wanna trigger Dialogue in battles from LUA too.
	//Same goes for unlocking items. I want this done globally.
	getGlobalNamespace(pLuaState)
		.addFunction("SetBattleDialogue", ScreenManager::SetBattleDialogue)
		.addFunction("AddCreateableItem", PlayerManager::AddCreateableItem);

	//Set global debug state.
	getGlobalNamespace(pLuaState)
		.beginNamespace("GAME")
			.addVariable("DEBUG", Debug, false)
		.endNamespace();

	//All done.
	std::cout << "LUA Succesfully initialised!" << std::endl;
}

Useable* LuaManager::LoadUseable(std::string _ItemID)
{
	Useable* NewUseable = new Useable();

	std::string filename = Folders::LUA_MAINFOLDER + Folders::USEABLE_FOLDER + _ItemID + ".lua";

	EventContext EC("Loading Useable LUA: ", _ItemID.c_str());

	if (luaL_dofile(pLuaState, filename.c_str()) == 0) {
		LuaRef table = getGlobal(pLuaState, "useable");
		if (table.isTable()) {
			NewUseable->setID(_ItemID);

			if (table["name"].isString()) {
				NewUseable->setName(table["name"].cast<std::string>());
			}
			else {
				NewUseable->setName("Null");
			}

			if (table["description"].isString()) {
				NewUseable->setDescription(table["description"].cast<std::string>());
			}
			else {
				NewUseable->setDescription("");
			}

			if (table["price"].isNumber()) {
				NewUseable->setPrice(table["price"].cast<int>());
			}
			else {
				NewUseable->setPrice(0);
			}

			if (table["usetype"].isString()) {
				if(table["usetype"] == "HEAL")
				{
					NewUseable->SetUsetype(Useable::HEAL);
				}
				else
				{
					NewUseable->SetUsetype(Useable::BUFF);
				}
			}

			LuaRef texture = getGlobal(pLuaState, "texture");
			NewUseable->setTexture(new sf::Sprite(*TextureManager::GetInstance()->GetItemTexture(texture["x"].cast<int>(), texture["y"].cast<int>())));

			if (table["use"].isFunction()) {
				NewUseable->luaUse = std::make_shared<LuaRef>(table["use"]);
			}
			else {
				NewUseable->luaUse.reset();
			}
		}
	}
	else {
		std::cout << "Error, can't open useable script " << filename << std::endl;
	}

	return NewUseable;
}

Weapon* LuaManager::LoadWeapon(std::string _ItemID)
{
	Weapon* newWeapon = new Weapon();

	std::string filename = Folders::LUA_MAINFOLDER + Folders::WEAPON_FOLDER + _ItemID + ".lua";

	EventContext EC("Loading Weapon LUA: ", _ItemID.c_str());

	if (luaL_dofile(pLuaState, filename.c_str()) == 0) {
		LuaRef table = getGlobal(pLuaState, "weapon");
		if (table.isTable()) {

			//This looks incredibly ugly but this is the only way for now.
			newWeapon->setID(_ItemID);

			if (table["name"].isString()) {
				newWeapon->setName(table["name"].cast<std::string>());
			}
			else {
				newWeapon->setName("Null");
			}

			if (table["description"].isString()) {
				newWeapon->setDescription(table["description"].cast<std::string>());
			}
			else {
				newWeapon->setDescription("");
			}

			if (table["weaponname"].isString()) {
				newWeapon->WeaponType = table["weaponname"].cast<std::string>();
			}
			else {
				newWeapon->WeaponType = "ATTACK";
			}

			if (table["price"].isNumber()) {
				newWeapon->setPrice(table["price"].cast<int>());
			}
			else {
				newWeapon->setPrice(0);
			}

			if (table["damage"].isNumber()) {
				newWeapon->setDMG(table["damage"].cast<int>());
			}
			else {
				newWeapon->setDMG(0);
			}

			if (table["protection"].isNumber()) {
				newWeapon->setPROT(table["protection"].cast<int>());
			}
			else {
				newWeapon->setPROT(0);
			}

			if (table["perc"].isNumber()) {
				newWeapon->setPERC(table["perc"].cast<int>());
			}
			else {
				newWeapon->setPERC(0);
			}

			if (table["crt"].isNumber()) {
				newWeapon->setCRT(table["crt"].cast<int>());
			}
			else {
				newWeapon->setCRT(0);
			}

			if (table["rng"].isNumber()) {
				int Range = table["rng"].cast<int>();
				newWeapon->setRNG(Range);

				if (Range > 1) {
					newWeapon->IsRanged = true;
				}
				else {
					newWeapon->IsRanged = false;
				}
			}
			else {
				newWeapon->setRNG(0);
			}

			LuaRef texture = getGlobal(pLuaState, "texture");
			newWeapon->setTexture(new sf::Sprite(*TextureManager::GetInstance()->GetItemTexture(texture["x"].cast<int>(), texture["y"].cast<int>())));

			if (table["attack"].isFunction()) {
				newWeapon->luaWeaponDamage = std::make_shared<LuaRef>(table["attack"]);
			}
			else {
				newWeapon->luaWeaponDamage.reset();
			}
		}
	}
	else {
		std::cout << "Error, can't open weapon script "<< filename << std::endl;
	}

	return newWeapon;
}

Unit* LuaManager::LoadUnit(std::string _UnitID, Unit::ENTITY_ALLIANCE _Alliance)
{
	Unit* newUnit = new Unit(_Alliance);

	std::string filename = Folders::LUA_MAINFOLDER + Folders::UNIT_FOLDER  + _UnitID + ".lua";

	EventContext EC("Loading Unit LUA: ", _UnitID.c_str());

	if (luaL_dofile(pLuaState, filename.c_str()) == 0) {
		LuaRef table = getGlobal(pLuaState, "unit");
		if (table.isTable()) {

			newUnit->setID(_UnitID);

			if (table["CHAR"].isString()) {
				newUnit->setCHAR(UnitManager::GetInstance()->GetCharacter(table["CHAR"].cast<std::string>()));
			}
			else
			{
				newUnit->setCHAR(nullptr);
			}

			if (table["CLASS"].isString()) {
				newUnit->setCLASS(UnitManager::GetInstance()->GetUnitClass(table["CLASS"].cast<std::string>()));
				if(newUnit->getCLASS())
				{
					newUnit->CreateUnitSprite(_Alliance);
				}
			}
			else
			{
				newUnit->setCLASS(nullptr);
			}

			if(table["INV"].isTable())
			{
				LuaRef INV = table["INV"];

				for (auto i = 1; i <= INV.length(); ++i)
				{
					std::string ItemID = INV[i].cast<std::string>();
					newUnit->addItemToInventory(ItemManager::GetInstance()->GetItem(ItemID));
				}
			}

			if (table["LVL"].isNumber()) {
				newUnit->setLVL(table["LVL"].cast<int>());
			}
			else {
				newUnit->setLVL(1);
			}

			if (table["HP"].isNumber()) {
				newUnit->setHP(table["HP"].cast<int>());
				newUnit->setMAXHP(table["HP"].cast<int>());
			}
			else {
				newUnit->setHP(99);
				newUnit->setMAXHP(99);
			}

			if (table["ATK"].isNumber()) {
				newUnit->setATK(table["ATK"].cast<int>());
			}
			else {
				newUnit->setATK(0);
			}

			if (table["DEF"].isNumber()) {
				newUnit->setDEF(table["DEF"].cast<int>());
			}
			else {
				newUnit->setDEF(0);
			}

			if (table["LCK"].isNumber()) {
				newUnit->setLCK(table["LCK"].cast<int>());
			}
			else {
				newUnit->setLCK(0);
			}

			if (table["MOV"].isNumber()) {
				newUnit->setMOV(table["MOV"].cast<int>());
			}
			else {				
				newUnit->setMOV(0);
			}

			if (table["SPD"].isNumber()) {
				newUnit->setSPD(table["SPD"].cast<int>());
			}
			else {
				newUnit->setSPD(0);
			}

			if (table["onEngaged"].isFunction()) {
				newUnit->luaOnEngaged = std::make_shared<LuaRef>(table["onEngaged"]);
			}
			else {
				newUnit->luaOnEngaged.reset();
			}

			if (table["onDefeated"].isFunction()) {
				newUnit->luaOnDefeated = std::make_shared<LuaRef>(table["onDefeated"]);
			}
			else {
				newUnit->luaOnDefeated.reset();
			}
		}
	}
	else {
		std::cout << "Error, can't open script " << filename << std::endl;
		EventContext ER("Cannot load Unit: ", _UnitID.c_str());
	}

	return newUnit;
}

PreloadChapter LuaManager::GetChapterInfo(std::string _ChapterName)
{
	PreloadChapter NewChapterInfo;

	std::string filename = Folders::LUA_MAINFOLDER + Folders::CHAPTER_FOLDER + _ChapterName + ".lua";

	if (luaL_dofile(pLuaState, filename.c_str()) == 0) {
		LuaRef table = getGlobal(pLuaState, "chapter");
		if (table.isTable()) {
			if (table["name"].isString()) {
				NewChapterInfo.Name = table["name"].cast<std::string>();
			}

			if (table["unitlimit"].isNumber()) {
				NewChapterInfo.UnitLimit = table["unitlimit"].cast<int>();
			}

			NewChapterInfo.ID = _ChapterName;
		}
	}

	return NewChapterInfo;
}

Chapter* LuaManager::LoadChapter(std::string _ChapterName)
{
	Chapter* NewChapter = new Chapter();

	std::string filename = Folders::LUA_MAINFOLDER + Folders::CHAPTER_FOLDER + _ChapterName + ".lua";

	EventContext EC("Loading Chapter LUA: ", _ChapterName.c_str());

	if (luaL_dofile(pLuaState, filename.c_str()) == 0) {
		LuaRef table = getGlobal(pLuaState, "chapter");
		if (table.isTable()) {

			if (table["name"].isString()) {
				NewChapter->ChapterName = table["name"].cast<std::string>();
			}
			else {
				NewChapter->ChapterName = "Untitled Chapter";
			}

			if (table["description"].isString()) {
				NewChapter->ChapterDescription = table["description"].cast<std::string>();
			}
			else {
				NewChapter->ChapterDescription = "An unknown journey awaits!";
			}

			if (table["unitlimit"].isNumber()) {
				NewChapter->UnitLimit = table["unitlimit"].cast<int>();
			}
			else {
				NewChapter->UnitLimit = 999;
			}

			if (table["chapterFlow"].isFunction()) {
				NewChapter->luaChapterFlow = std::make_shared<LuaRef>(table["chapterFlow"]);
			}
			else {
				NewChapter->luaChapterFlow.reset();
			}
		}
	}
	else {
		std::cout << "Error, can't open chapter " << filename << std::endl;
		EventContext ER("Cannot load Chapter: ", _ChapterName.c_str());
	}

	return NewChapter;
}
