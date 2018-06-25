weapon = {
    name = "Standard Laser",
	weapontype = "DAMAGE",
	price = 190,
	damage = 5,
	protection = 0,
	perc = 90,
	crt = 10,
	rng = 2,
	description = "An average laser for ranged attacks.",
    attack = function(weapon, attacker, defender)
		return weapon.DMG
    end
};
texture = {
	x = 12,
	y = 10
};