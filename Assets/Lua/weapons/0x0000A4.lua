weapon = {
    name = "Vib. Shield",
	weapontype = "DAMAGE",
	price = 0,
	damage = 5,
	protection = 3,
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
	x = 6,
	y = 13
};