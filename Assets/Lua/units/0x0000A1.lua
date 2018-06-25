unit = {
	CHAR = "agent",
	CLASS = "SOLDIER",
	LVL = 1,
	HP = 18,
	ATK = 7,
	DEF = 2,
	LCK = 0,
	MOV = 5,
	SPD = 10,
	INV = {
		"0x0000A2",
	},
	onEngaged = function(self, engager)
		if engager == "0xPLAYER" and not FlagSet("0xF1") then
			SetBattleDialogue("0x000001")
			SetFlag("0xF1")
		end
	end;
	onDefeated = function(self, engager)

	end;
};