#include "KillAllObjective.h"
#include "Map.h"

KillAllObjective::KillAllObjective(Map* _pMap) : MapObjective(_pMap)
{

}

KillAllObjective::~KillAllObjective()
{

}

void KillAllObjective::CheckObjective()
{
	if(pMap->EnemyUnits.size() == 0)
	{
		ObjectiveAchieved = true;
	}
}