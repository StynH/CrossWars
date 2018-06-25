#pragma once
#include "GameState.h"

class AddUnit : public GameState
{
public:
	AddUnit(std::string _UnitID);
	~AddUnit();
	
	void Run() override;

private:
	std::string UnitID;
};

