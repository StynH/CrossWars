weapon = {
    name = "Villieldr",
	weapontype = "DAMAGE",
	price = 0,
	damage = 19,
	protection = 0,
	perc = 100,
	crt = 15,
	rng = 1,
    attack = function(weapon, attacker, defender)
		return weapon.DMG
    end
};
texture = {
	x = 6,
	y = 1
};