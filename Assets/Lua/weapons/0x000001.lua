--[[The Debug Saber is a template weapon]]--
weapon = {
    name = "Venge Katana",
	weapontype = "DAMAGE",
	price = 9999,
	damage = 1,
	protection = 1,
	perc = 85,
	crt = 0,
	rng = 1,
    attack = function(weapon, attacker, defender)
		return weapon.DMG;
    end
};
texture = {
	x = 0,
	y = 1
};