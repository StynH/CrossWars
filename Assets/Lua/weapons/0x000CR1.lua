weapon = {
    name = "Standard Blade",
	weapontype = "DAMAGE",
	price = 100,
	damage = 5,
	protection = 0,
	perc = 75,
	crt = 0,
	rng = 1,
	description = "A standard blade for infantry.\nIt's a starter weapon for a starter unit.",
    attack = function(weapon, attacker, defender)
		return weapon.DMG;
    end
};
texture = {
	x = 0,
	y = 0
};