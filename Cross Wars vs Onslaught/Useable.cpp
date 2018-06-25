#include "Useable.h"
#include <iostream>
#include "MusicPlayer.h"


Useable::Useable() : luaUse(nullptr)
{
	TYPE = ACTION;
}

Useable::Useable(const Useable& _Useable)
{
	GUID = GenerateRandomGUID();

	PRICE = _Useable.PRICE;
	ID = _Useable.ID;
	NAME = _Useable.NAME;
	DESCRIPTION = _Useable.DESCRIPTION;
	SPRITE = new sf::Sprite(*_Useable.SPRITE);
	TYPE = _Useable.TYPE;
	USETYPE = _Useable.USETYPE;

	luaUse = _Useable.luaUse;
}

Useable::~Useable()
{
}

void Useable::Use(Unit* _pUser)
{
	if (luaUse) {
		try {
			(*luaUse)(_pUser);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "LuaException: " << e.what() << std::endl;
		}
	}
	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->ItemUseSound, false);
}
