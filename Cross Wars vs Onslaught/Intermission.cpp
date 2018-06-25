#include "Intermission.h"
#include <string>
#include "Dialogue.h"

Intermission::Intermission(std::string _IntermissionName) : GameState()
{
	IntermissionName = _IntermissionName;
}

Intermission::~Intermission()
{
}

void Intermission::Run()
{
	ScreenManager::GetInstance()->GetIntermissionScreen()->LoadIntermission(IntermissionName);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetIntermissionScreen());
}
