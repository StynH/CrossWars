#pragma once
#include "GameState.h"

class Battle : public GameState
{
public:
	Battle(std::string _BattleID);
	~Battle();

	void Run() override;

private:
	std::string MapID;
};

