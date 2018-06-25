#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>
#include "Unit.h"
#include "BattleOutcome.h"
#include "TextureManager.h"

class UnitSkill
{
public:

	enum ACTIVATIONTIME
	{
		ONGAIN,
		PREBATTLE,
		ONATTACK,
		ONDEFEND,
		ONTAKINGDAMAGE,
		AFTERBATTLE
	};

	std::string GetDescription();
	std::string GetName();

	virtual void Trigger(Unit* _Equipped, Unit* _Enemy, BattleOutcome* _Battle) = 0;

	UnitSkill();
	virtual ~UnitSkill();
	
	std::shared_ptr<sf::Sprite> SkillSprite;

	ACTIVATIONTIME TriggerType;
protected:
	std::string SkillName;
	std::string SkillDescription;
};

