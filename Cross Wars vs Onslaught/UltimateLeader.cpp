#include "UltimateLeader.h"

UltimateLeader::UltimateLeader()
{
	TriggerType = PREBATTLE;
	SkillName = "Ultimate Leader";
	SkillDescription = "Unit gains 50% more Support Points.";
	SkillSprite = std::shared_ptr<sf::Sprite>(TextureManager::GetInstance()->GetSkillTexture(4, 0));
}

UltimateLeader::~UltimateLeader()
{
}

void UltimateLeader::Trigger(Unit* _Equipped, Unit* _Enemy, BattleOutcome* _Battle)
{
	//Check to see if the equipped is an attacker or defender.
	if(_Battle->Attacker->getGUID() == _Equipped->getGUID())
	{
		_Battle->AttackerATKBonus *= 1.5;
		_Battle->AttackerDEFBonus *= 1.5;
		_Battle->AttackerSPDBonus *= 1.5;
	}
	else
	{
		_Battle->DefenderATKBonus *= 1.5;
		_Battle->DefenderDEFBonus *= 1.5;
		_Battle->DefenderSPDBonus *= 1.5;
	}
}
