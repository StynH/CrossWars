#include "Dialogue.h"

Dialogue::Dialogue(std::string _DialogueID) : GameState()
{
	DialogueID = _DialogueID;
}

Dialogue::~Dialogue()
{
}

void Dialogue::Run()
{
	ScreenManager::GetInstance()->GetDialogueScreen()->LoadDialogue(DialogueID);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetDialogueScreen());
}
