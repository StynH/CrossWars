#pragma once
#include <SFML/System/Vector2.hpp>
#include "UnitSkill.h"

class BattleOutcome;

class UltimateLeader : public UnitSkill
{
public:
	void Trigger(Unit* _Equipped, Unit* _Enemy, BattleOutcome* _Battle) override;

	UltimateLeader();
	~UltimateLeader();
};

