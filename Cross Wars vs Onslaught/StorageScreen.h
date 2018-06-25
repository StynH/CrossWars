#pragma once
#include "GameScreen.h"
#include "ItemActionMenu.h"

struct StoredItem
{
	std::shared_ptr<sf::Sprite> pItemSprite;
	std::shared_ptr<sf::Sprite> pContainerSprite;
	std::shared_ptr<sf::Text> pItemText;
	std::string ItemID;
};

class StorageScreen : public GameScreen
{
public:
	StorageScreen(sf::RenderWindow* _pRenderWindow);
	~StorageScreen();
	
	void Start() override;
	void Draw() override;
	void Update() override;
	void RefreshStorage();
	void Unload() override;
	void PollInput() override;
private:
	void EmptyStorageStructs();
	void InitStorage();
	void ReadjustCounter();
	void UpdateCursorPosition();
	void SelectUnitToUseItemOn();
	void UseItemOn(Unit* _pUnit);

	ItemActionMenu* pItemActionMenu;
	UnitSelectScreen* pUnitSelectScreen;

	GameScreenOverlay* pCurrentOverlay;

	sf::Sprite* pBackground;
	sf::Sprite* pContainer;
	sf::Sprite* pCursorSprite;

	std::vector<StoredItem*> StoredItems;

	int Offset;
	int ItemCounter;

	bool CanMoveUp;
	bool CanMoveDown;
};

