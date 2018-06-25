useable = {
    name = "Health Flask",
	description = "A flask with a liquid that can be injected to heal 25HP.",
	price = 1000,
	usetype = "HEAL",
    use = function(user)
		user:HEAL(25)
    end
};
texture = {
	x = 9,
	y = 11
};