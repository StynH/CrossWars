#pragma once
#include "MapObjective.h"

class KillAllObjective : public MapObjective
{
public:
	KillAllObjective(Map* _pMap);
	~KillAllObjective();
	void CheckObjective();
};

