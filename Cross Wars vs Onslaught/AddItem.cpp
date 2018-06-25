#include "AddItem.h"

AddItem::AddItem(std::string _ItemID) : GameState()
{
	ItemID = _ItemID;
}

AddItem::~AddItem()
{
}

void AddItem::Run()
{
	Item* pItem = ItemManager::GetInstance()->GetItem(ItemID);
	ScreenManager::GetInstance()->GetRecieveScreen()->RecieveItem(pItem);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetRecieveScreen());
	ItemManager::GetInstance()->AddItemToStorage(pItem);
}
