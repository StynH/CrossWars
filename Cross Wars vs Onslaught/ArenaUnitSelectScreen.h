#pragma once
#include "GameScreen.h"
#include "UnitSelectScreen.h"
#include "ItemTradeOverlay.h"
#include "Camera.h"

struct ArenaUnitContainer
{
	std::shared_ptr<sf::Sprite> ActiveContainerSprite;
	std::shared_ptr<sf::Sprite> InactiveContainerSprite;
	std::shared_ptr<sf::Sprite> UnitSprite;
	std::shared_ptr<sf::Text> UnitName;
	sf::Vector2f Position;
	Unit* pUnit;
	bool IsActive;
};

class ArenaUnitSelectScreen : public GameScreen
{
public:
	const int SCREEN_LIMIT = 14;
	ArenaUnitSelectScreen(sf::RenderWindow* _pRenderWindow);
	~ArenaUnitSelectScreen();

	void Start() override;
	void Draw() override;
	void Unload() override;
	void PollInput() override;

	void InitContainers(int _Offset);
	void UpdateCursorPosition();
private:
	std::shared_ptr<sf::Sprite> Background;
	sf::Sprite* pInactiveContainerSprite;
	sf::Sprite* pActiveContainerSprite;
	sf::Sprite* pCursorSprite;

	sf::Sprite* ScrollUpEnabled;
	sf::Sprite* ScrollUpDisabled;
	sf::Sprite* ScrollDownEnabled;
	sf::Sprite* ScrollDownDisabled;

	std::vector<ArenaUnitContainer> Containers;

	ArenaUnitContainer* pActiveContainer;
	Camera* pCamera;

	bool CanMoveUp;
	bool CanMoveDown;

	int UnitCounter;
	int UnitLimit;
	int Offset;
};

