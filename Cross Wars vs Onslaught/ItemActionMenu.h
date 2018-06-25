#pragma once
#include "GameScreenOverlay.h"
#include "UIButton.h"
#include "Swapvector.h"

class UnitSelectScreen;

class ItemActionMenu : public GameScreenOverlay
{
public:
	enum STATE { INSTORAGE, ININVENTORY };

	ItemActionMenu(sf::RenderWindow* _pRenderWindow, STATE _State);
	~ItemActionMenu();

	void Draw() override;
	void Load() override;
	void Update() override;
	void Unload() override;
	void Hide() override;
	void PollInput() override;
	void SetItem(Item* _pItem);
	void SetUnit(Unit* _pUnit);

	template <class T>
	void AddEventOnSendToUnit(void(T::* _CallbackFunction)(), T* _Class)
	{
		pOnSendToUnit += std::bind(_CallbackFunction, _Class);
	}

	template <class T>
	void AddEventOnSendToStorage(void(T::* _CallbackFunction)(), T* _Class)
	{
		pStoreInStorageButton->Fire += std::bind(_CallbackFunction, _Class);
	}

	template <class T>
	void AddEventOnItemUse(void(T::* _CallbackFunction)(), T* _Class)
	{
		pUseButton->Fire += std::bind(_CallbackFunction, _Class);
	}

	template <class T>
	void AddEventOnItemDelete(void(T::* _CallbackFunction)(), T* _Class)
	{
		pDeleteButton->Fire += std::bind(_CallbackFunction, _Class);
	}

	UIButton* pSendToUnitButton;
	UIButton* pStoreInStorageButton;
	UIButton* pUseButton;
	UIButton* pDeleteButton;
private:
	void UpdateCursorPosition();
	//Options
	void SelectUnit();
	void SendToUnit(Unit* _pUnit);
	void SendItemToStorage();
	void DeleteItem();

	STATE CurrentState;

	//Overlay-sception
	UnitSelectScreen* pUnitSelectScreen;

	Swapvector<UIButton*> ButtonList;

	sf::Sprite* pCursorSprite;

	Item* pSelectedItem;
	Unit* pSelectedUnit;

	Event<nullptr_t> pOnSendToUnit;
};

