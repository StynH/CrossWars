#include "Character.h"

Character::Character()
{
}


Character::~Character()
{
}

Character::Character(Character* _Character)
{
	ATKGrowth = _Character->GetATKGrowth();
	DEFGrowth = _Character->GetDEFGrowth();
	SPDGrowth = _Character->GetSPDGrowth();
	LCKGrowth = _Character->GetLCKGrowth();
	HPGrowth = _Character->GetHPGrowth();
	Name = _Character->GetNAME();
	AttackSprite = _Character->AttackSprite;
	BattleSprite = _Character->BattleSprite;
	DialougeSprite = _Character->DialougeSprite;
	SupportSprite = _Character->SupportSprite;
	HUDSprite = new sf::Sprite(*_Character->HUDSprite);
}
