#pragma once
#include "GameState.h"

class UnlockMission : public GameState
{
public:
	UnlockMission(std::string _MissionID);
	~UnlockMission();

	void Run() override;

private:
	std::string MissionID;
};

