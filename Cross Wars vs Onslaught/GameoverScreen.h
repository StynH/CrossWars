#pragma once
#include "GameScreen.h"
#include <memory>
#include "Camera.h"

class GameoverScreen : public GameScreen
{
public:
	GameoverScreen(sf::RenderWindow* _pRenderWindow);
	~GameoverScreen();

	void Start() override;
	void Draw() override;
	void Unload() override;
	void PollInput() override;
private:
	std::shared_ptr<sf::Sprite> pBackground;
	Camera* pCamera;
};

