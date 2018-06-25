#pragma once
#include "GameScreen.h"
#include <memory>
#include "Unit.h"
#include "Camera.h"
#include "ItemTradeOverlay.h"

struct UnitContainer
{
	std::shared_ptr<sf::Sprite> ActiveContainerSprite;
	std::shared_ptr<sf::Sprite> InactiveContainerSprite;
	std::shared_ptr<sf::Sprite> UnitSprite;
	std::shared_ptr<sf::Text> UnitName;
	sf::Vector2f Position;
	Unit* pUnit;
	bool IsActive;
};

class ArmyScreen : public GameScreen
{
public:
	const int SCREEN_LIMIT = 14;
	ArmyScreen(sf::RenderWindow* _pRenderWindow);
	~ArmyScreen();

	void Start() override;
	void ShowContainers(int _Offset);
	void Update() override;
	void Draw() override;
	void Unload() override;
	void PollInput() override;
	void UpdateCursorPosition();
	void LimitUnits(int _UnitLimit);
	void TradeBetweenUnits(Unit* _pUnit);

private:
	std::shared_ptr<sf::Sprite> Background;
	sf::Sprite* pInactiveContainerSprite;
	sf::Sprite* pActiveContainerSprite;
	sf::Sprite* pCursorSprite;
	
	sf::Sprite* ScrollUpEnabled;
	sf::Sprite* ScrollUpDisabled;
	sf::Sprite* ScrollDownEnabled;
	sf::Sprite* ScrollDownDisabled;

	sf::Sprite* pVignetteSprite;

	sf::Text* pUnitLimitText;
	std::vector<UnitContainer> Containers;

	UnitSelectScreen* pUnitSelectScreen;
	ItemTradeOverlay* pItemTradeScreen;
	GameScreenOverlay* pActiveOverlay;

	UnitContainer* pActiveContainer;
	Camera* pCamera;

	bool CanMoveUp;
	bool CanMoveDown;

	int UnitCounter;
	int UnitLimit;
	int Offset;
};

