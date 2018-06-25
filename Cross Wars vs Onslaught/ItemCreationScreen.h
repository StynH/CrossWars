#pragma once
#include "GameScreen.h"
#include <memory>
#include "TypeWriter.h"

struct CreatableItem
{
	std::shared_ptr<sf::Sprite> pItemSprite;
	std::shared_ptr<sf::Sprite> pContainerSprite;
	std::shared_ptr<sf::Text> pItemText;
	std::shared_ptr<sf::Text> pItemPrice;
	std::string ItemID;
	int Price;
};

class ItemCreationScreen : public GameScreen
{
public:
	ItemCreationScreen(sf::RenderWindow* _pRenderWindow);
	~ItemCreationScreen();

	void Start() override;
	void PollInput() override;
	void Update() override;
	void Draw() override;
	void Unload() override;

	void AddItem(std::string _ItemID);
	const std::vector<CreatableItem*>& GetCreateableItems();
private:
	void UpdateCounter();
	void UpdateCursorPosition();

	bool CanCreateItem(CreatableItem* pItem);
	void CreateItem(std::string _ItemID);

	void LoadSombraText();
	std::string GetSombraText();

	TypeWriter* pTypeWriter;

	sf::Sprite* pBackground;
	sf::Sprite* pContainer;
	sf::Sprite* pCursorSprite;

	sf::Sprite* pScrollUp;
	sf::Sprite* pScrollDown;

	sf::Sprite* pSpeechBubble;

	sf::Text* pEnergyText;

	std::vector<CreatableItem*> CreateableItems;

	std::vector<std::string> SombraMorningText;
	std::vector<std::string> SombraAfternoonText;
	std::vector<std::string> SombraEveningText;
	std::vector<std::string> SombraNightText;

	int ItemCounter;
	int Offset;

	bool CanMoveUp;
	bool CanMoveDown;
};

