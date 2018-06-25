#pragma once
#include "GameScreen.h"
#include <SFML/Graphics/Sprite.hpp>
#include "BattleCharScreen.h"
#include "TextContainer.h"
#include "UnitSelectScreen.h"
#include "ItemActionMenu.h"

class UnitOverviewScreen : public GameScreen
{
public:
	enum STATE
	{
		CHAR,
		ITEMS,
		SKILLS
	};

	UnitOverviewScreen(sf::RenderWindow* _pRenderWindow);
	~UnitOverviewScreen();

	void Start() override;
	void Draw() override;
	void Unload() override;
	void Update();
	void PollInput();

	void SetUnit(Unit* _pUnit);
	void ShowSkill(UnitSkill* _pSkill);
	void ShowItem(Item* _pItem);
	void ShowUnit();

	template <class T>
	void AddEventOnItemUse(void(T::* _CallbackFunction)(), T* _Class)
	{
		ExternOnItemUse += std::bind(_CallbackFunction, _Class);
	}
private:
	void ItemSendToStorage();
	void ReadjustCounter();
	void ItemUse();
	void ItemDelete();

	ItemActionMenu* pItemActionMenu;
	
	Event<nullptr_t> ExternOnItemUse;

	Item* pSelectedItem;

	std::string SanitizedCharacterDescription;

	sf::Sprite* pCursor;
	sf::Sprite* pCharCursor;
	sf::Sprite* pItemCursor;
	sf::Sprite* pSkillCursor;

	sf::Sprite* pBackground;
	sf::Sprite* pForeground;

	sf::Text* pHeaderText;
	TextContainer* pInfoContainer;

	Camera* pCamera;

	Unit* pCurrentUnit;

	BattleCharScreen* pUnitOverviewScreen;

	STATE CurrentState;

	bool CanRearrangeItems;

	int SkillIndex;
	int ItemIndex;
	int SelectedItemIndex;
};

