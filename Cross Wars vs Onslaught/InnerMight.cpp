#include "InnerMight.h"

InnerMight::InnerMight()
{
	TriggerType = ONGAIN;
	SkillName = "Inner Might";
	SkillDescription = "Grands ATK +2";
	SkillSprite = std::shared_ptr<sf::Sprite>(TextureManager::GetInstance()->GetSkillTexture(1, 0));
}


InnerMight::~InnerMight()
{
}

void InnerMight::Trigger(Unit* _Equipped, Unit* _Enemy, BattleOutcome* _Battle)
{
	_Equipped->setATK(_Equipped->getATK() + 2);
}