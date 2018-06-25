#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

class UnitClass
{
public:
	UnitClass(int _ATKGrowth, int _DEFGRowth, int _SPDGrowth, int _LCKGrowth, int _HPGrowth, std::string _NAME, std::string _ID, sf::Rect<int> _SPRITE);
	~UnitClass();

	std::string GetNAME() { return NAME; }
	std::string GetID() { return ID; }
	int GetATKGrowth() { return ATKGrowth; }
	int GetDEFGrowth() { return DEFGrowth; }
	int GetSPDGrowth() { return SPDGrowth; }
	int GetLCKGrowth() { return LCKGrowth; }
	int GetHPGrowth() { return HPGrowth; }

	sf::Rect<int>* GetUnitTextureCoords() { return &TextureCords; };

private:
	std::string NAME, ID;
	//Class Growth
	int ATKGrowth, DEFGrowth, SPDGrowth, LCKGrowth, HPGrowth;

	sf::Rect<int> TextureCords;
};

