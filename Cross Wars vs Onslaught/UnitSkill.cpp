#include "UnitSkill.h"
#include "TextureManager.h"
#include "Unit.h"

UnitSkill::UnitSkill()
{

}

UnitSkill::~UnitSkill()
{
}

std::string UnitSkill::GetDescription()
{
	return SkillDescription;
}

std::string UnitSkill::GetName()
{
	return SkillName;
}
