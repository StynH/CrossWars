#pragma once
#include "GameScreenOverlay.h"
#include "Unit.h"

class LevelScreen;

struct TradeableItem
{
	std::shared_ptr<sf::Sprite> Sprite;
	std::shared_ptr<sf::Text> Title;
	bool IsSelected;
	std::string GUID;
};

class ItemTradeOverlay : public GameScreenOverlay
{
public:
	ItemTradeOverlay(sf::RenderWindow* _pRenderWindow);
	~ItemTradeOverlay();

	void Draw();
	void SwapItem(Unit* _pUnit);
	void SwapBetweenUnits(int _FirstIndex, int _SecondIndex);
	void Reset();
	void Update();
	void Load() override;
	void Unload();
	void Hide();
	void PollInput();
	
	void UpdateCursor();
	void SetTraders(Unit* _pUnitOne, Unit* _pUnitTwo);
	void InitInventories();
	void DrawInventories();
	void EmptyInventories();
private:
	Unit* pUnitOne;
	Unit* pUnitTwo;

	std::shared_ptr<sf::Sprite> ItemContainerOne;
	std::shared_ptr<sf::Sprite> ItemContainerTwo;

	std::shared_ptr<sf::Sprite> TradeArrows;

	std::vector<TradeableItem*> InventoryOne;
	std::vector<TradeableItem*> InventoryTwo;

	TradeableItem* pSelectedItem;
	bool SelectedItemInPlayerTwoInventory;

	int Counter;
	int SelectedItemIndex;
	bool InPlayerTwoInventory;

	sf::Sprite* pInvslotSprite;
	sf::Sprite* pInvslotActiveSprite;

	sf::Sprite* pCursorSprite;
};

