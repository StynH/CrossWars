weapon = {
    name = "Metal Fist",
	weapontype = "DAMAGE",
	price = 9999,
	damage = 5,
	protection = 1,
	perc = 100,
	crt = 0,
	rng = 1,
    attack = function(weapon, attacker, defender)
		return weapon.DMG;
    end
};
texture = {
	x = 4,
	y = 13
};