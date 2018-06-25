#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

class Trigger
{
public:
	Trigger(sf::Vector2f _Position);
	Trigger(sf::Vector2f _Position, int _FamilyID, std::string _ScriptID);
	~Trigger();

	int GetFamilyID() const;
	std::string GetScriptID() const;

	sf::Vector2f getPosition() const;

	bool IsTriggerActive() const;
	void Deactivate();
	void Activate();
private:
	int FamilyID;
	std::string ScriptID;
	bool IsActive;
	sf::Vector2f Position;
};

