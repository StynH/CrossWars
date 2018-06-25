#pragma once
#include "Item.h"
#include <memory>
#include "LuaBridge.h"
#include "Unit.h"

class Weapon : public Item
{
public:
	std::string WeaponType;

	bool IsRanged;

	enum WeaponType { SWORD, BOW, GUN, MECH, MELEE };

	Weapon();
	Weapon(const Weapon& Weapon);
	~Weapon();

	void Debug(const std::string& text);

	//Getters and Setters
	int getDMG() const{ return DMG; }
	void setDMG(int _DMG){ DMG = _DMG; }

	int getPROT() const { return PROT; }
	void setPROT(int _PROT) { PROT = _PROT; }

	int getPERC() const{ return PERC; }
	void setPERC(int _PERC){ PERC = _PERC; }
	
	int getCRT() const { return CRT; }
	void setCRT(int _CRT){ CRT = _CRT; }

	int getRNG() const { return RNG; }
	void setRNG(int _RNG){ RNG = _RNG;}

	int getWeaponDamage(Unit* Attacker, Unit* Defender);
	std::shared_ptr<luabridge::LuaRef> luaWeaponDamage;

private:
	//Damage, Hit Percentage, Crit, Range;
	int DMG, PROT, PERC, CRT, RNG;
};

