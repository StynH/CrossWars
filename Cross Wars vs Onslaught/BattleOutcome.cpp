#include "BattleOutcome.h"
#include "BattleHelper.h"
#include <iostream>
#include "UnitSkill.h"

BattleOutcome::BattleOutcome(Unit* _pAttacker, Unit* _pDefender, Unit* _pAttacksupport, Unit* _pDefendsupport)
{
	Attacker = _pAttacker;
	Defender = _pDefender;
	AttackerSupport = _pAttacksupport;
	DefenderSupport = _pDefendsupport;
	WillCounter = false;

	AttackerATKBonus = 0;
	AttackerDEFBonus = 0;
	AttackerSPDBonus = 0;

	DefenderATKBonus = 0;
	DefenderDEFBonus = 0;
	DefenderSPDBonus = 0;

	CalculateBattleOutcome();
}


BattleOutcome::~BattleOutcome()
{
	Attacker = nullptr;
	Defender = nullptr;
	AttackerSupport = nullptr;
	DefenderSupport = nullptr;
}

void BattleOutcome::CalculateBattleOutcome()
{
	Weapon* DefenderEquipped = nullptr;
	WillCounter = false;
	if(Defender->getItemFromInventory(0) != nullptr && Defender->getItemFromInventory(0)->getType() == Item::WEAPON)
	{
		WillCounter = true;
		DefenderEquipped = static_cast<Weapon*>(Defender->getItemFromInventory(0));
	}
	Weapon* AttackerEquipped = static_cast<Weapon*>(Attacker->getItemFromInventory(0));

	if(AttackerSupport != nullptr)
	{
		StatInfo SupportStat = GetSupportBestStat(AttackerSupport);

		if (SupportStat.Name == "ATK")
			AttackerATKBonus = SupportStat.Value / 2;
		else if (SupportStat.Name == "DEF")
			AttackerDEFBonus = SupportStat.Value / 2;
		else if (SupportStat.Name == "SPD")
			AttackerSPDBonus = SupportStat.Value / 2;
	}

	if (DefenderSupport != nullptr)
	{
		StatInfo SupportStat = GetSupportBestStat(DefenderSupport);

		//Boy do I wish switch would work with strings.
		if(SupportStat.Name == "ATK")
			DefenderATKBonus = SupportStat.Value / 2;
		else if (SupportStat.Name == "DEF")
			DefenderDEFBonus = SupportStat.Value / 2;
		else if (SupportStat.Name == "SPD")
			DefenderSPDBonus = SupportStat.Value / 2;
	}

	for(UnitSkill* Skill : Attacker->getSkills())
	{
		if(Skill->TriggerType == UnitSkill::PREBATTLE)
		{
			Skill->Trigger(Attacker, Defender, this);
		}
	}

	for (UnitSkill* Skill : Defender->getSkills())
	{
		if (Skill->TriggerType == UnitSkill::PREBATTLE)
		{
			Skill->Trigger(Defender, Attacker, this);
		}
	}

	AttackerDamage = BattleHelper::CalculateBattleDamage(Attacker, Defender, AttackerEquipped, AttackerATKBonus, DefenderDEFBonus);
	AttackerHitChance = BattleHelper::CalculateHitChance(Attacker, Defender, AttackerEquipped, DefenderSPDBonus);
	AttackerCritChance = AttackerEquipped->getCRT() + Attacker->getLCK() / 2;

	if(WillCounter)
	{
		DefenderDamage = BattleHelper::CalculateBattleDamage(Defender, Attacker, DefenderEquipped, DefenderATKBonus, AttackerDEFBonus);
		DefenderHitChance = BattleHelper::CalculateHitChance(Defender, Attacker, DefenderEquipped, AttackerSPDBonus);
		DefenedrCritChance = DefenderEquipped->getCRT() + Defender->getLCK() / 2;
	}
	else
	{
		DefenderDamage = 0;
		DefenderHitChance = 0;
		DefenedrCritChance = 0;
	}

	DefenderFutureHP = Defender->getHP() - AttackerDamage;
	DefenderFutureHP = DefenderFutureHP < 0 ? 0 : DefenderFutureHP;

	AttackerFutureHP = Attacker->getHP() - DefenderDamage;
	AttackerFutureHP = AttackerFutureHP < 0 ? 0 : AttackerFutureHP;
}


StatInfo BattleOutcome::GetSupportBestStat(Unit* _pSupportUnit)
{
	StatInfo ATK("ATK", _pSupportUnit->getATK());
	StatInfo DEF("DEF", _pSupportUnit->getDEF());
	StatInfo SPD("SPD", _pSupportUnit->getSPD());

	std::vector<StatInfo> Stats;
	Stats.push_back(ATK);
	Stats.push_back(DEF);
	Stats.push_back(SPD);

	int BestStatValue = INT_MIN;
	StatInfo BestStat;
	for (StatInfo Stat : Stats)
	{
		if (Stat.Value > BestStatValue)
		{
			BestStatValue = Stat.Value;
			BestStat = Stat;
		}
	}

	return BestStat;
}