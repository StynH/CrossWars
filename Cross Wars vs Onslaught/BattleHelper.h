#pragma once
#include "Unit.h"
#include "Weapon.h"

struct Stat
{
	std::shared_ptr<sf::Text> Name;
	std::shared_ptr<sf::Text> Value;
};

class BattleHelper
{
public:
	static int CalculateBattleDamage(Unit* _pAttacker, Unit* _pDefender, Weapon* _pEquipped, int _ATKBonus, int _DEFBonus)
	{
		int WeaponDamage = _pEquipped->getWeaponDamage(_pAttacker, _pDefender);
		int WeaponProtection = 0;

		if(_pDefender->getItemFromInventory(0) != nullptr && _pDefender->getItemFromInventory(0)->getType() == Item::WEAPON)
		{
			WeaponProtection = static_cast<Weapon*>(_pDefender->getItemFromInventory(0))->getPROT();
		}

		int TotalDamage = (_pAttacker->getATK() + WeaponDamage + _ATKBonus) - (_pDefender->getDEF() + WeaponProtection + _DEFBonus);
		return TotalDamage >= 0 ? TotalDamage : 0;
	}

	static int CalculateHitChance(Unit* _pAttacker, Unit* _pDefender, Weapon* _pEquipped, int _SPDBonus)
	{
		int HitRate = _pEquipped->getPERC() + _pAttacker->getLCK() * 2;
		int Evade = _pDefender->getSPD() + _SPDBonus + (_pDefender->getLCK() / 2);
		int HitChance = HitRate - Evade;

		if(HitChance < 0)
		{
			HitChance = 0;
		}
		else if(HitChance > 100)
		{
			HitChance = 100;
		}

		return HitChance;
	}

	static bool WillHit(int _Chance)
	{
		//A Mersenne Twister to generate a random number between 1 and 100.
		//The way a hit is calculated is through adding two random numbers together.
		//This makes sure that 90% hitrates for example almost dont fail and sub 50% fail alot more.
		boost::random::mt19937 Generator;
		Generator.seed(static_cast<unsigned int>(time(0)));
		boost::random::uniform_int_distribution<> Hit(1, 100);
		int RNG1 = Hit(Generator);
		int RNG2 = Hit(Generator);

		int RNG = (RNG1 + RNG2) / 2;

		if (RNG <= _Chance)
		{
			return true;
		}
		return false;
	}

	static bool WillCrit(int _Chance)
	{
		boost::random::mt19937 Generator;
		Generator.seed(static_cast<unsigned int>(std::time(0)));
		boost::random::uniform_int_distribution<> Hit(1, 100);
		if (Hit(Generator) <= _Chance)
		{
			return true;
		}
		return false;
	}
};
