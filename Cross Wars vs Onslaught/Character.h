#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "CharacterSprite.h"

class Character
{
public:
	Character();
	Character(Character* _Character);
	~Character();

	CharacterSprite* AttackSprite;
	CharacterSprite* BattleSprite;
	CharacterSprite* DialougeSprite;
	CharacterSprite* SupportSprite;
	sf::Sprite* HUDSprite;

	std::string GetNAME() const { return Name; }
	void setName(const std::string& _Name) { Name = _Name; }

	std::string GetDescription() const { return Description; }
	void setDescription(const std::string& _Description) { Description = _Description; }

	std::string GetID() const { return ID; }
	void setId(const std::string& _ID) { ID = _ID; }

	int GetATKGrowth() { return ATKGrowth; }
	void SetATKGrowth(int _ATKGrowth) { ATKGrowth = _ATKGrowth; }

	int GetDEFGrowth() { return DEFGrowth; }
	void SetDEFGrowth(int _DEFGrowth) { DEFGrowth = _DEFGrowth; }

	int GetSPDGrowth() { return SPDGrowth; }
	void SetSPDGrowth(int _SPDGrowth) { SPDGrowth = _SPDGrowth; }

	int GetLCKGrowth() { return LCKGrowth; }
	void SetLCKGrowth(int _LCKGrowth) { LCKGrowth = _LCKGrowth; }

	int GetHPGrowth() { return HPGrowth; }
	void SetHPGrowth(int _HPGrowth) { HPGrowth = _HPGrowth; }


private:
	//Character Growth.
	//This + Class Growth = Total Growth
	int ATKGrowth, DEFGrowth, SPDGrowth, LCKGrowth, HPGrowth;

	std::string Name;
	std::string Description;
	std::string ID;
};


