#pragma once
#include <SFML/Graphics.hpp>

class Camera : public sf::View
{
public:
	Camera(sf::FloatRect _rect);
	~Camera();

	void DrawUpdate();

	static const int SCREEN_HEIGHT = 512;
	static const int SCREEN_WIDTH = 512;
protected:
	sf::RenderWindow* pRenderWindow;
};
