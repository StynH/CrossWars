weapon = {
    name = "Hacking Glove",
	weapontype = "DAMAGE",
	price = 0,
	damage = 14,
	protection = 2,
	perc = 85,
	crt = 1,
	rng = 2,
    attack = function(weapon, attacker, defender)
		dmg = weapon.DMG
		if attacker.ATK < defender.ATK then
			dmg = 16
		end
		return dmg
    end
};
texture = {
	x = 8,
	y = 13
};