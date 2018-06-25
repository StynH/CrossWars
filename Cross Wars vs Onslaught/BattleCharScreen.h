#pragma once
#include "Folders.h"
#include "GameScreen.h"
#include <SFML/Graphics/Sprite.hpp>
#include "Camera.h"
#include "Unit.h"
#include "BattleHelper.h"

struct InventoryItem
{
	std::shared_ptr<sf::Sprite> Sprite;
	std::shared_ptr<sf::Text> Title;
};

class BattleCharScreen : public GameScreen
{
public:

	bool IsActive, EmptyScreen;
	void DrawStats();
	void InitInventory();
	void InitSkills();
	void DrawInventory();
	void DrawSkills();
	void Draw();
	void InitStats();
	void Update();
	void Unload();
	void PollInput();

	void ChangeUnit(Unit* _pNewUnit);

	void CallForClearance() { if (pCurrentUnit) EmptyScreen = true; }
	void UnsetUnit();

	sf::Sprite* ActiveHud;
	sf::Sprite* InactiveHud;
	sf::Sprite* InvSlot;
	sf::Sprite* SkillContainer;

	sf::Font* HUDFont;

	sf::Text* NameText;
	sf::Text* HPText;
	sf::Text* LVLText;
	sf::Text* ClassText;

	std::vector<Stat*> Stats;
	std::vector<InventoryItem*> Inventory;

	Camera* pSubCamera;

	BattleCharScreen(sf::RenderWindow* _pRenderWindow);
	~BattleCharScreen();

private:
	Unit* pCurrentUnit;

	std::map<std::string, std::string> UnitStats;

	sf::RenderTexture* pRenderTexture;
	sf::Sprite* pRenderSprite;
};

