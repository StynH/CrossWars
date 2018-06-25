#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Unit.h"

class Cursor : public Entity
{
public:
	enum OWNER { PLAYER, AI };

	bool IsOnUnit;
	bool HasUnitSelected;

	void Think(){};
	void Update();
	void Move(int _x, int _y);

	Unit* UnitUnderCursor();
	void CheckUnderCursor();

	sf::Sprite* getSprite() { return pCursorSprite; };

	Cursor(sf::Sprite* _pSprite, OWNER _Owner);
	~Cursor();

	void SelectUnit();
	void DeselectUnit();

	Unit* GetSelectedUnit() { return pSelectedUnit; };
	Unit* GetUnitBeneathCursor() { return pUnitBeneath; }

private:
	sf::Sprite* pCursorSprite;

	Unit* pSelectedUnit;
	Unit* pUnitBeneath;

	OWNER Owner;
};

