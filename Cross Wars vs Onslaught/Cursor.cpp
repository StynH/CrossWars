#include "Cursor.h"
#include <iostream>
#include "Unit.h"
#include "ScreenManager.h"
#include "MapHelper.h"
#include "MusicPlayer.h"

Cursor::Cursor(sf::Sprite* _pSprite, OWNER _Owner)
{
	pCursorSprite = _pSprite;
	setPosition(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
	IsOnUnit = false;
	HasUnitSelected = false;
	Owner = _Owner;
}


Cursor::~Cursor()
{
	delete pCursorSprite;
}

void Cursor::SelectUnit()
{
	pSelectedUnit = UnitUnderCursor();
	HasUnitSelected = true;

	if (Owner == PLAYER)
	{
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
	}
}

void Cursor::DeselectUnit()
{
	pSelectedUnit = nullptr;
	HasUnitSelected = false;
}

void Cursor::Update()
{
	getSprite()->setPosition(getPosition());
}

void Cursor::Move(int _x, int _y)
{
	int x = getPosition().x + (_x * 32);
	int y = getPosition().y + (_y * 32);

	int DataPosX = getDataPosition().x + _x;
	int DataPosY = getDataPosition().y + _y;

	setPosition(sf::Vector2<float>(x,y), sf::Vector2<float>(DataPosX, DataPosY));
	CheckUnderCursor();

	if(Owner == PLAYER)
	{
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
	}
}

Unit* Cursor::UnitUnderCursor()
{
	return MapHelper::FindUnitAtPos(getDataPosition(), ScreenManager::GetInstance()->GetLevelScreen()->GetCurrentMap());
}

void Cursor::CheckUnderCursor()
{
	Unit* FoundUnit = UnitUnderCursor();
	if(FoundUnit != nullptr)
	{
		IsOnUnit = true;
		pUnitBeneath = FoundUnit;
		ScreenManager::GetInstance()->GetLevelScreen()->SetUnitOverviewScreen(FoundUnit);
	}
	else if(pUnitBeneath)
	{
		IsOnUnit = false;
		pUnitBeneath = nullptr;
		ScreenManager::GetInstance()->GetLevelScreen()->ClearUnitOverviewScreen();
	}
}
