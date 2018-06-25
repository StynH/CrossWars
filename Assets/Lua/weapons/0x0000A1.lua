weapon = {
    name = "Jokull",
	weapontype = "DAMAGE",
	price = 0,
	damage = 6,
	protection = 1,
	perc = 95,
	crt = 5,
	rng = 1,
    attack = function(weapon, attacker, defender)
		dmg = weapon.DMG
		if attacker.ATK > defender.DEF then
			dmg = dmg - 1
		else
			dmg = dmg + 1
		end
		return dmg
    end
};
texture = {
	x = 7,
	y = 13
};