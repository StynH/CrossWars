#pragma once
#include <vector>
#include "Unit.h"
#include "Map.h"
#include "Cursor.h"
#include "FloodFill.h"
#include "BattleOutcome.h"
#include "Useable.h"

struct AINode
{
	int Score;
	MovementTile* Tile;

	bool HasSupport;
	Unit* SupportInRange;

	bool InRangeOfAttack;
	Unit* UnitInRange;
	std::shared_ptr<BattleOutcome> Battle;

	friend bool operator < (const AINode& a, const AINode& b) {
		return a.Score < b.Score;
	}

	~AINode()
	{
		Tile = nullptr;
		SupportInRange = nullptr;
		UnitInRange = nullptr;
	}
};

//The artificial intelligence.
class AI
{
public:
	enum AIState
	{
		NOUNIT,
		PICKPOSITION,
		SETUNIT,
		PREPARINGATTACK,
		ATTACKING,
		DONE
	};

	AI(Unit::ENTITY_ALLIANCE _Alliance, Cursor* _pCursor);
	~AI();

	MovementTile* GetFarestMovableTileFromPath(std::vector<MovementTile> _pPath, int _Distance);
	void Think();
	void Acivate();
	void SetMap(Map* _pMap);

	Unit* GetFreeUnit();
	std::vector<Unit*> GetEnemiesInRange(std::vector<MovementTile*>& _Tiles);
	Unit* FindEnemyToAttack();
	bool UnitIsLowHP(Unit* _pUnit);
	Useable* GetHealingItem(Unit* _pUnit);

	bool IsDone() { return AIDone; };

	template<typename T> void AI::SafeDelete(T*& a) {
		if (a) {
			delete a;
			a = NULL;
		}
	}

private:
	bool AIDone;
	float AITimer;

	std::unique_ptr<AINode> TargetNode;

	Map* pCurrentMap;

	Cursor* pAICursor;

	Unit::ENTITY_ALLIANCE AIAlliance;
	AIState CurrentState;

	std::priority_queue<AINode> AvailableNodes;
	std::vector<MovementTile*> PossibleUnitMovement;
};

