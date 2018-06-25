#pragma once
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void Update() = 0;

	std::string getGUID() const { return GUID; }
	void generateGUID();

	int getX() { return pPosition.x; };
	int getY() { return pPosition.y; };

	sf::Vector2f getPosition() { return pPosition; };
	sf::Vector2f getDataPosition() { return pDataPosition; }
	sf::Vector2f getPreviousPosition() { return pPrevPos; }

	void setPosition(sf::Vector2f _pPosition) { pPrevPos = pPosition; pPosition = _pPosition; }
	void setPosition(sf::Vector2f _pPosition, sf::Vector2f _pDataPosition) { pPrevPos = pPosition; pPosition = _pPosition; pDataPosition = _pDataPosition; }

	void setX(int _x) { pPosition.x = _x; };
	void setY(int _y) { pPosition.y = _y; };

	friend bool operator== (const Entity &Left, const Entity &Right);
	friend bool operator!= (const Entity &Left, const Entity &Right);
private:
	sf::Texture* pSprite;
	sf::Vector2f pPosition;
	sf::Vector2f pPrevPos;
	sf::Vector2f pDataPosition;

	std::string GUID;
};

