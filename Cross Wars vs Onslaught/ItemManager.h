#pragma once
#include "Singleton.h"
#include "LuaManager.h"
#include <vector>
#include "Item.h"
#include "json.hpp"
#include <fstream>
#include <unordered_map>

class ItemManager: public TSingleton<ItemManager>
{
public:
	ItemManager();
	~ItemManager();

	void LoadItems();

	void AddItem(Item* _pItem);
	Item* GetItem(std::string _ID);
	Item* const GetItemTemplate(std::string _ID);

	void AddItemToStorage(Item* _pItem);
	Item* FindItemInStorage(std::string _GUID);
	Item* GetItemFromStorage(std::string _GUID);
	const std::vector<Item*>& GetItemStorage();
	void RemoveItemFromStorage(Item* _pItem, bool _Delete);
	int GetStorageSize();
private:
	std::unordered_map<std::string, Item*> ItemList;
	std::vector<Item*> ItemStorage;
};
