weapon = {
    name = "Dark Laser",
	weapontype = "DAMAGE",
	price = 9999,
	damage = 5,
	protection = 0,
	perc = 80,
	crt = 0,
	rng = 2,
    attack = function(weapon, attacker, defender)
		return weapon.DMG;
    end
};
texture = {
	x = 15,
	y = 10
};