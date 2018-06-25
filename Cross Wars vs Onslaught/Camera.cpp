#include "Camera.h"
#include "ResourceManager.h"

Camera::Camera(sf::FloatRect _rect) : sf::View(_rect)
{
	pRenderWindow = ResourceManager::GetWindow();
}

Camera::~Camera()
{

}

void Camera::DrawUpdate()
{
	pRenderWindow->setView(*this);
}

sf::RenderWindow* pRenderWindow = nullptr;