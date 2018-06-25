#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class Item
{
public:
	enum ITEMTYPE
	{
		WEAPON,
		ACTION,
		UPGRADE
	};

	//Getters and Setters
	void setPrice(const int &_PRICE) { PRICE = _PRICE; }
	void setName(const std::string &_NAME) { NAME = _NAME; }
	void setDescription(const std::string &_DESCRIPTION) { DESCRIPTION = _DESCRIPTION; }
	void setID(const std::string &_ID) { ID = _ID; }
	void setTexture(sf::Sprite* _SPRITE) { SPRITE = _SPRITE; }
	void setType(const ITEMTYPE _TYPE) { TYPE = _TYPE; };

	int getPrice() const { return PRICE; }
	std::string getName() const { return NAME; }
	std::string getDescription() const { return DESCRIPTION; }
	std::string getID() const { return ID; }
	sf::Sprite* getTexture() const { return SPRITE; }
	std::string getGUID() { return GUID; }
	ITEMTYPE getType() const { return TYPE; }

	Item();
	~Item();
protected:
	//Every item has its price!
	int PRICE;
	std::string ID, NAME, DESCRIPTION;
	sf::Sprite* SPRITE;
	ITEMTYPE TYPE;

	std::string GenerateRandomGUID();
	std::string GUID;
};

