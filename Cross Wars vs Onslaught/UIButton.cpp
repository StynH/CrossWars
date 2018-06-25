#include "UIButton.h"
#include <SFML/Graphics/RenderWindow.hpp>

UIButton::UIButton(sf::Sprite* _pButtonImage, sf::RenderWindow* _pWindow, sf::Vector2f _Position)
{
	pButtonImage = _pButtonImage;
	pWindow = _pWindow;
	SetPosition(_Position);
	Available = true;
}

UIButton::~UIButton()
{
	delete pButtonImage;
}

void UIButton::Draw()
{
	pWindow->draw(*pButtonImage);
}

void UIButton::SetPosition(sf::Vector2f _Position)
{
	Position = _Position;
	pButtonImage->setPosition(Position);
}
