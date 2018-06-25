#pragma once
#include "UnitSkill.h"

class InnerMight : public UnitSkill
{
public:
	void Trigger(Unit* _Equipped, Unit* _Enemy, BattleOutcome* _Battle) override;

	InnerMight();
	~InnerMight();
};

