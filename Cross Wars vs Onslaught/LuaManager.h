#pragma once

#include "Singleton.h";
#include <lua.hpp>
#include <LuaBridge.h>
#include "Item.h"
#include "Weapon.h"
#include <iostream>
#include <memory>
#include "PlayerManager.h"
#include "Useable.h"

class Entity;
class Chapter;

class LuaManager: public TSingleton<LuaManager>
{
public:
	LuaManager();
	~LuaManager();

	void CreateState();
	void Init();
	Useable* LoadUseable(std::string _ItemID);
	Weapon* LoadWeapon(std::string _ItemID);
	Unit* LoadUnit(std::string _UnitID, Unit::ENTITY_ALLIANCE _Alliance);
	PreloadChapter GetChapterInfo(std::string _ChapterName);
	Chapter* LoadChapter(std::string _ChapterName);

	static lua_State* getLuaState() { return pLuaState; };
private:
	static lua_State* pLuaState;
};

