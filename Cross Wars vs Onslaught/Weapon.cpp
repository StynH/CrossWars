#include "Weapon.h"
#include <iostream>
#include <LuaBridge.h>

Weapon::Weapon() : luaWeaponDamage(nullptr)
{
	TYPE = WEAPON;
}

Weapon::Weapon(const Weapon& _Weapon)
{
	GUID = GenerateRandomGUID();

	PRICE = _Weapon.PRICE;
	ID = _Weapon.ID;
	NAME = _Weapon.NAME;
	DESCRIPTION = _Weapon.DESCRIPTION;
	SPRITE = new sf::Sprite(*_Weapon.SPRITE);
	TYPE = _Weapon.TYPE;
	
	DMG = _Weapon.DMG;
	PROT = _Weapon.PROT;
	PERC = _Weapon.PERC;
	CRT = _Weapon.CRT;
	RNG = _Weapon.RNG;

	WeaponType = _Weapon.WeaponType;
	IsRanged = _Weapon.IsRanged;
	
	luaWeaponDamage = _Weapon.luaWeaponDamage;
}

Weapon::~Weapon()
{

}

void Weapon::Debug(const std::string& text)
{
	std::cout << text << std::endl;
}

int Weapon::getWeaponDamage(Unit * Attacker, Unit * Defender)
{
	if (luaWeaponDamage) {
		try {
			return (*luaWeaponDamage)(this, Attacker, Defender);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "LuaException: " << e.what() << std::endl;
		}
	}
	return 0;
}
