#include "UnitSelectScreen.h"
#include "Unit.h"
#include "PlayerManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "TextSanitizer.h"
#include "MusicPlayer.h"

UnitSelectScreen::UnitSelectScreen(sf::RenderWindow* _pWindow) : GameScreenOverlay(_pWindow)
{
	pContainer = std::make_unique<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("chardisplay\\unitselect.png", Folders::FolderType::HUD));
	pContainer->setPosition(32, 32);

	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("chardisplay\\unitselectcursor.png", Folders::FolderType::HUD));

	pUnitslotTexture = TextureManager::GetInstance()->LoadTexture("chardisplay\\unit_slot.png", Folders::FolderType::HUD);
}


UnitSelectScreen::~UnitSelectScreen()
{
}

void UnitSelectScreen::Draw()
{
	pWindow->draw(*pContainer);

	int Start = 5 * Offset;
	for (int i = Start; i < Start + 5; ++i)
	{
		if (UnitList.size() < i + 1)
		{
			break;
		}
		
		pWindow->draw(*UnitList.at(i).Sprite);
		pWindow->draw(*UnitList.at(i).Name);
	}

	pWindow->draw(*pCursorSprite);
}

void UnitSelectScreen::Unload()
{
	UnitList.clear();
	Callback.Clear();
}

void UnitSelectScreen::Load()
{
	Offset = 0;
	UnitCounter = 0;
	CloseScreen = false;

	CanMoveUp = false;

	sf::Font* pFont = ResourceManager::GetInstance()->GetAceFont();

	int Counter = 1;

	for(Unit* pUnit : PlayerManager::GetInstance()->GetReadyUnits())
	{
		UnitName NewUnit;
		NewUnit.Name = std::make_shared<sf::Text>();
		NewUnit.Name->setString(pUnit->getCHAR()->GetNAME());
		NewUnit.Name->setColor(sf::Color::White);
		NewUnit.Name->setFont(*pFont);
		NewUnit.Name->setCharacterSize(16);
		TextSanitizer::CenterOriginHorizontallyLeft(NewUnit.Name.get());
		NewUnit.Name->setPosition(59, 47 + (20 * Counter + 20 * (Counter - 1)));

		NewUnit.GUID = pUnit->getGUID();

		NewUnit.Sprite = std::make_shared<sf::Sprite>(*pUnitslotTexture);
		NewUnit.Sprite->setPosition(49, 47 + (8 * Counter + 32 * (Counter - 1)));

		UnitList.push_back(NewUnit);

		++Counter;
		if(Counter == 6)
		{
			Counter = 1;
		}
	}

	for (Unit* pUnit : PlayerManager::GetInstance()->GetInactiveUnits())
	{
		UnitName NewUnit;
		NewUnit.Name = std::make_shared<sf::Text>();
		NewUnit.Name->setString(pUnit->getCHAR()->GetNAME());
		NewUnit.Name->setColor(sf::Color::White);
		NewUnit.Name->setFont(*pFont);
		NewUnit.Name->setCharacterSize(16);
		TextSanitizer::CenterOriginHorizontallyLeft(NewUnit.Name.get());
		NewUnit.Name->setPosition(59, 47 + (20 * Counter + 20 * (Counter - 1)));

		NewUnit.GUID = pUnit->getGUID();

		NewUnit.Sprite = std::make_shared<sf::Sprite>(*pUnitslotTexture);
		NewUnit.Sprite->setPosition(49, 47 + (8 * Counter + 32 * (Counter - 1)));

		std::cout << pUnit->getCHAR()->GetNAME() << std::endl;

		UnitList.push_back(NewUnit);

		++Counter;
		if (Counter == 6)
		{
			Counter = 1;
		}
	}

	UpdateCursorPosition();
}

void UnitSelectScreen::Hide()
{
	UnitList.clear();
	CloseScreen = true;
}

void UnitSelectScreen::UpdateCursorPosition()
{
	pCursorSprite->setPosition(49, 41 + (8 * (UnitCounter - (Offset * 5) + 1) + 32 * (UnitCounter - (Offset * 5))));
}

void UnitSelectScreen::PollInput()
{
	sf::Event event;

	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			CloseGame = true;
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
				Hide();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				if(UnitCounter - (5 * Offset) + 1 < 5)
				{
					if(UnitCounter + 1 < UnitList.size())
					{
						++UnitCounter;
						UpdateCursorPosition();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					}
				}
				else
				{
					++Offset;
					++UnitCounter;
					UpdateCursorPosition();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);

					CanMoveUp = true;
				}
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				if (UnitCounter - (5 * Offset) > 0)
				{
					--UnitCounter;
					UpdateCursorPosition();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
				else if (CanMoveUp)
				{
					--Offset;
					--UnitCounter;
					UpdateCursorPosition();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);

					if(Offset == 0)
					{
						CanMoveUp = false;
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				Unit* pUnit = PlayerManager::GetInstance()->FindUnit(UnitList.at(UnitCounter).GUID);
				Callback(pUnit);
			}
		}
	}
}
