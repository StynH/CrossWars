#pragma once
#include "ScreenManager.h"

class GameState
{
public:
	GameState()
	{
		
	};

	virtual ~GameState()
	{
		
	};

	virtual void Run() = 0;
};

