#include "UnitClass.h"
#include "UnitManager.h"

UnitClass::UnitClass(int _ATKGrowth, int _DEFGRowth, int _SPDGrowth, int _LCKGrowth, int _HPGrowth, std::string _NAME, std::string _ID, sf::Rect<int> _SPRITE)
{
	ATKGrowth = _ATKGrowth;
	DEFGrowth = _DEFGRowth;
	SPDGrowth = _SPDGrowth;
	LCKGrowth = _LCKGrowth;
	HPGrowth = _HPGrowth;
	NAME = _NAME;
	ID = _ID;

	TextureCords = _SPRITE;
}


UnitClass::~UnitClass()
{
}
