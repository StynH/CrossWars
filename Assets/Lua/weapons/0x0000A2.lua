weapon = {
    name = "Fists",
	weapontype = "DAMAGE",
	price = 0,
	damage = 3,
	protection = 0,
	perc = 80,
	crt = 0,
	rng = 1,
	description = "Sometimes it has to be done with bare hands.\n\nIf the Equippers ATK is higher than the Defenders DEF, this weapon gains +1 DMG.",
    attack = function(weapon, attacker, defender)
		dmg = weapon.DMG
		if attacker.ATK > defender.DEF then
			dmg = dmg + 1
		end
		return dmg
    end
};
texture = {
	x = 5,
	y = 13
};