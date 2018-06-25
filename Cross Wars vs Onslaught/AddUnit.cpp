#include "AddUnit.h"
#include "PlayerManager.h"

AddUnit::AddUnit(std::string _UnitID) : GameState()
{
	UnitID = _UnitID;
}

AddUnit::~AddUnit()
{
}

void AddUnit::Run()
{
	Unit* pUnit = UnitManager::GetInstance()->LoadUnit(UnitID, Unit::PLAYER, false);
	ScreenManager::GetInstance()->GetRecieveScreen()->RecieveUnit(pUnit);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetRecieveScreen());
	PlayerManager::GetInstance()->AddInactiveUnit(pUnit);
}
