#pragma once
#include "GameScreenOverlay.h"
#include <memory>
#include <SFML/Graphics/Text.hpp>
#include "Event.h"
#include "Unit.h"

struct UnitName
{
	std::shared_ptr<sf::Text> Name;
	std::shared_ptr<sf::Sprite> Sprite;
	std::string GUID;
};

class UnitSelectScreen : public GameScreenOverlay
{
public:
	UnitSelectScreen(sf::RenderWindow* _pWindow);
	~UnitSelectScreen();

	void Draw() override;
	void Unload() override;
	void PollInput() override;
	void Load() override;
	void Hide() override;
	void UpdateCursorPosition();

	template<typename T>
	void SetCallback(void(T::* _CallbackFunction)(Unit*), T* _Class)
	{
		Callback += bind(_CallbackFunction, _Class, std::placeholders::_1);
	};
private:
	std::vector<UnitName> UnitList;
	Event<Unit*> Callback;

	int Offset;
	int UnitCounter;

	bool CanMoveUp;
	bool CanMoveDown;

	std::unique_ptr<sf::Sprite> pContainer;
	sf::Sprite* pCursorSprite;
	sf::Texture* pUnitslotTexture;
};

