#pragma once
#include <string>

class GameFlag
{
public:
	GameFlag(std::string _Adress)
	{
		Adress = _Adress;
		Unset();
	}

	std::string Adress;

	bool IsFlagSet() const { return IsSet; }
	void Set() { IsSet = true; }
	void Unset() { IsSet = false; }

private:
	bool IsSet;
};
