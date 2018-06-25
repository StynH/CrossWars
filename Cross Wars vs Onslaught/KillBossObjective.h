#pragma once
#include "MapObjective.h"
#include "Unit.h"

class KillBossObjective : public MapObjective
{
public:
	KillBossObjective(Unit* _pBoss, Map* _pMap);
	~KillBossObjective();

	void CheckObjective();

private:
	Unit* pBoss;
};

