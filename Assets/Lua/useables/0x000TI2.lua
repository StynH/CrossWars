useable = {
    name = "ATK Boost",
	description = "Raises the users ATK by 1.",
	price = 200,
	usetype = "BUFF",
    use = function(user)
		user:INCREASEATK(1)
    end
};
texture = {
	x = 14,
	y = 10
};