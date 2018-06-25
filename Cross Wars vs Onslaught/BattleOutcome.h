#pragma once
#include "Unit.h"

class StatInfo
{
public:
	StatInfo() {};
	~StatInfo() {};
	StatInfo(std::string _Name, int _Value)
	{
		Name = _Name;
		Value = _Value;
	};
	std::string Name;
	int Value;
};

class BattleOutcome
{
public:
	void CalculateBattleOutcome();
	StatInfo GetSupportBestStat(Unit* _pSupportUnit);

	BattleOutcome(Unit* _attacker, Unit* _defender, Unit* _attacksupport, Unit* _defendsupport);
	~BattleOutcome();

	Unit* Attacker;
	Unit* Defender;
	Unit* AttackerSupport;
	Unit* DefenderSupport;

	int AttackerDamage;
	int AttackerHitChance;
	int AttackerCritChance;

	int DefenderDamage;
	int DefenderHitChance;
	int DefenedrCritChance;

	int DefenderFutureHP;
	int AttackerFutureHP;

	int AttackerATKBonus;
	int AttackerDEFBonus;
	int AttackerSPDBonus;

	int DefenderATKBonus;
	int DefenderDEFBonus;
	int DefenderSPDBonus;

	bool WillCounter;
};

