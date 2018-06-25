#pragma once

//Forward declaration baby.
//Somehow caused circular dependency?
class ScreenManager;

#include "ScreenManager.h"
#include <memory>
#include "LuaBridge.h"

class Chapter
{
public:
	Chapter();
	~Chapter();

	void LoadBattle(const std::string& _id);
	void LoadIntermission(const std::string& _id);
	void LoadScene(const std::string& _id);
	void AddUnit(const std::string& _id);
	void AddItem(const std::string& _id);
	void UnlockNewMission(const std::string& _id);
	void ShowChapterScreen();
	void ChapterFlow();

	std::shared_ptr<luabridge::LuaRef> luaChapterFlow;

	int UnitLimit;
	std::string ChapterName;
	std::string ChapterDescription;
};