--[[Proto Sentinel]]--
unit = {
    CHAR = "sentinel",
	CLASS = "SENTINEL",
	LVL = 1,
	HP = 12,
	ATK = 5,
	DEF = 5,
	LCK = 0,
	MOV = 4,
	SPD = 4,
	INV = {
		"0x000003",
		"0x0000A3"
	},
    onEngaged = function(self, engager)
		if engager == "0x000001" and not FlagSet("0xF2") then
			SetBattleDialogue("0x000002")
			SetFlag("0xF2")
		elseif engager == "0x000PA1" and not FlagSet("0xF3") then
			SetBattleDialogue("0x000003")
			SetFlag("0xF3")
		end
    end;
	
	onDefeated = function(self, engager)

    end;
};