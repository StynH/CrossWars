#pragma once
#include "GameState.h"

class AddItem : public GameState
{
public:
	AddItem(std::string _ItemID);
	~AddItem();

	void Run() override;

private:
	std::string ItemID;
};

