#include "KillBossObjective.h"

KillBossObjective::KillBossObjective(Unit* _pBoss, Map* _pMap) : MapObjective(_pMap)
{
	pBoss = _pBoss;
}

KillBossObjective::~KillBossObjective()
{
}

void KillBossObjective::CheckObjective()
{
	if(pBoss->IsDead)
	{
		ObjectiveAchieved = true;
	}
}
