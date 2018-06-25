#pragma once
#include "Item.h"
#include <memory>
#include "LuaBridge.h"
#include "Unit.h"

class Useable : public Item
{
public:
	enum GOAL { BUFF, HEAL };

	Useable();
	Useable(const Useable& _Useable);
	~Useable();

	void SetUsetype(GOAL _GOAL) { USETYPE = _GOAL; }
	GOAL GetUsetype() { return USETYPE; };

	void Use(Unit* _pUser);
	std::shared_ptr<luabridge::LuaRef> luaUse;

private:
	GOAL USETYPE;
};

