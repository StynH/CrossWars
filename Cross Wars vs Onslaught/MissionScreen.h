#pragma once
#include "GameScreen.h"
#include <memory>
#include "Camera.h"
#include "Swapvector.h"

struct MissionContainer
{
	std::shared_ptr<sf::Sprite> ActiveContainerSprite;
	std::shared_ptr<sf::Sprite> InactiveContainerSprite;
	std::shared_ptr<sf::Text> MissionName;
	std::string ID;
	sf::Vector2f Position;
	int UnitLimit;
	bool Selected;
};

class MissionScreen : public GameScreen
{
public:
	MissionScreen(sf::RenderWindow* _pRenderWindow);
	~MissionScreen();

	void Start() override;
	void Draw() override;
	void Unload() override;
	void PollInput() override;

private:
	Swapvector<MissionContainer*> AvailableMissions;
	std::shared_ptr<sf::Sprite> Background;
	Camera* pCamera;

	sf::Sprite* pActiveContainerSprite;
	sf::Sprite* pInactiveContainerSprite;
};

