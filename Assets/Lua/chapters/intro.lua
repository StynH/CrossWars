chapter = {

	name = "Prologue",
	description = "",
	unitlimit = 1,
	
	chapterFlow = function(chapter)
		if GAME._DEBUG == false then
			chapter:LoadIntermission("intro")
			chapter:LoadScene("intro")
			chapter:LoadScene("intro1")
			chapter:LoadScene("intro2")
			chapter:LoadBattle("firstassault")
			chapter:LoadScene("intro3")
			chapter:LoadIntermission("intro2")
			chapter:LoadScene("intro4")
			chapter:AddUnit("0x000PA1")
			chapter:AddUnit("0x000001")
			chapter:UnlockMission("chapter2")
		else
			chapter:LoadIntermission("intro")
			chapter:LoadScene("intro")
			chapter:LoadScene("intro1")
			chapter:LoadScene("intro2")
			chapter:LoadBattle("firstassault")
			chapter:LoadScene("intro3")
			chapter:LoadIntermission("intro2")
			chapter:LoadScene("intro4")
			chapter:AddUnit("0x000PA1")
			chapter:AddUnit("0x000001")
			chapter:UnlockMission("chapter2")
		end
    end
	
};