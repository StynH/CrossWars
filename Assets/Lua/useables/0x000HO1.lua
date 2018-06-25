useable = {
    name = "Small Health Flask",
	description = "A flask with a liquid that can be injected to heal 10HP.",
	price = 200,
	usetype = "HEAL",
    use = function(user)
		user:HEAL(10)
    end
};
texture = {
	x = 7,
	y = 11
};