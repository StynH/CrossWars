#pragma once
#include <string>
#include "GameState.h"

class Intermission : public GameState
{
public:
	Intermission(std::string _IntermissionName);
	~Intermission();

	void Run() override;
private:
	std::string IntermissionName;
};

