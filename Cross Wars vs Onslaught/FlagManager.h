#pragma once
#include "GameFlag.h"
#include <vector>
#include <map>

class FlagManager
{
public:

	static void LoadFlags();

	static bool FlagSet(std::string _Adress);
	static void SetFlag(std::string _Adress);
	static void UnsetFlag(std::string _Adress);

	static std::map<std::string, GameFlag*> GetFlags();
private:
	static std::map<std::string, GameFlag*> GameFlags;
};


