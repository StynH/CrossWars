#include "Trigger.h"

Trigger::Trigger(sf::Vector2f _Position)
{
	Position = _Position;
	Activate();
}

Trigger::Trigger(sf::Vector2f _Position, int _FamilyID, std::string _ScriptID)
{
	Position = _Position;
	FamilyID = _FamilyID;
	ScriptID = _ScriptID;
}

Trigger::~Trigger()
{
}

int Trigger::GetFamilyID() const
{
	return FamilyID;
}

std::string Trigger::GetScriptID() const
{
	return ScriptID;
}

sf::Vector2f Trigger::getPosition() const
{
	return Position;
}

bool Trigger::IsTriggerActive() const
{
	return IsActive;
}

void Trigger::Deactivate()
{
	IsActive = false;
}

void Trigger::Activate()
{
	IsActive = true;
}
