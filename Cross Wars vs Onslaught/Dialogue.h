#pragma once
#include "GameState.h"

class Dialogue : public GameState
{
public:
	Dialogue(std::string _DialogueID);
	~Dialogue();

	void Run() override;

private:
	std::string DialogueID;
};

