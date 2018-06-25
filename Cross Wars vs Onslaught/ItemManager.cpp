#include "ItemManager.h"
#include "boost/filesystem.hpp"
#include "Folders.h"
using namespace boost::filesystem;

ItemManager::ItemManager()
{
}

//RIP Items
ItemManager::~ItemManager()
{
	ItemList.clear();
}

//Load all items (templates)
void ItemManager::LoadItems()
{
	//Thanks Boost, you made it alot easier for me.
	path WeaponPath(Folders::LUA_MAINFOLDER + "weapons");
	directory_iterator EndItr;

	//Iterate through every LUA file in the weapon folder.
	for (directory_iterator itr(WeaponPath); itr != EndItr; ++itr)
	{
		//Try to load that weapon, only if it's a file though.
		if (is_regular_file(itr->path())) {
			AddItem(LuaManager::GetInstance()->LoadWeapon(itr->path().stem().string()));
		}
	}

	path UseablePath(Folders::LUA_MAINFOLDER + "useables");
	//Iterate through every LUA file in the useable folder.
	for (directory_iterator itr(UseablePath); itr != EndItr; ++itr)
	{
		//Try to load that useable, only if it's a file though.
		if (is_regular_file(itr->path())) {
			AddItem(LuaManager::GetInstance()->LoadUseable(itr->path().stem().string()));
		}
	}

	/*
	for(std::string id: j["weapons"])
	{
		AddItem(LuaManager::GetInstance()->LoadWeapon(id));
	}
	*/
	std::cout << "All items loaded!" << std::endl;
}


//Adds and item template to the list.
void ItemManager::AddItem(Item* _pItem)
{
	ItemList.insert({ _pItem->getID(), _pItem });
}

//This works basically as a template factory.
//A request comes in and an item gets copied.
Item* ItemManager::GetItem(std::string _ID)
{
	auto search = ItemList.find(_ID);
	if (search != ItemList.end()) {
		switch(search->second->getType())
		{
		case Item::WEAPON: 
			return new Weapon(*static_cast<Weapon*>(search->second));
		case Item::ACTION: 
			return new Useable(*static_cast<Useable*>(search->second));
		case Item::UPGRADE: 
			return nullptr;
		default: ;
		}
	}
	return nullptr;
}

Item* const ItemManager::GetItemTemplate(std::string _ID)
{
	auto search = ItemList.find(_ID);
	if (search != ItemList.end()) {
		switch (search->second->getType())
		{
		case Item::WEAPON:
			return static_cast<Weapon*>(search->second);
		case Item::ACTION:
			return static_cast<Useable*>(search->second);
		case Item::UPGRADE:
			return nullptr;
		default:;
		}
	}
	return nullptr;
}

//Adds an item to the storage of the player.
void ItemManager::AddItemToStorage(Item* _pItem)
{
	ItemStorage.push_back(_pItem);
}

//Finds an item in the storage of the player and returns it.
Item* ItemManager::FindItemInStorage(std::string _GUID)
{
	for (Item* pItem : ItemStorage)
	{
		if (pItem->getGUID() == _GUID)
		{
			return pItem;
		}
	}
	return nullptr;
}

//Gets an item from the storage of the player.
//Quite literally "gets" it because it will be removed from storage.
Item* ItemManager::GetItemFromStorage(std::string _GUID)
{
	signed int i = 0;
	for(Item* pItem : ItemStorage)
	{
		if(pItem->getGUID() == _GUID)
		{
			ItemStorage.erase(ItemStorage.begin() + i);
			return pItem;
		}
		i++;
	}
	return nullptr;
}

const std::vector<Item*>& ItemManager::GetItemStorage()
{
	return ItemStorage;
}

void ItemManager::RemoveItemFromStorage(Item* _pItem, bool _Delete)
{
	for (std::vector<Item*>::iterator iter = ItemStorage.begin(); iter != ItemStorage.end(); ++iter)
	{
		if ((*iter)->getGUID() == _pItem->getGUID())
		{
			if(_Delete)
			{
				delete *iter;
			}

			ItemStorage.erase(iter);
			break;
		}
	}
}

int ItemManager::GetStorageSize()
{
	return ItemStorage.size();
}
