weapon = {
    name = "Ragnarok",
	weapontype = "DAMAGE",
	price = 0,
	damage = 25,
	protection = 0,
	perc = 100,
	crt = 25,
	rng = 1,
    attack = function(weapon, attacker, defender)
		return weapon.DMG
    end
};
texture = {
	x = 7,
	y = 4
};