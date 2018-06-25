weapon = {
    name = "Giygas Laser",
	weapontype = "DAMAGE",
	price = 9999,
	damage = 9,
	protection = 0,
	perc = 85,
	crt = 0,
	rng = 1,
    attack = function(weapon, attacker, defender)
		return weapon.DMG;
    end
};
texture = {
	x = 14,
	y = 10
};