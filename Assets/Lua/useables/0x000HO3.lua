useable = {
    name = "Super Health Flask",
	description = "A flask with a strong liquid that can be injected to heal to Max HP.",
	price = 5000,
	usetype = "HEAL",
    use = function(user)
		user:HEAL(user.MAXHP - user.HP)
    end
};
texture = {
	x = 8,
	y = 11
};