#include "ArenaUnitSelectScreen.h"
#include "PlayerManager.h"
#include "ResourceManager.h"
#include "MusicPlayer.h"
#include "ScreenManager.h"

ArenaUnitSelectScreen::ArenaUnitSelectScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	Background = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("army.png", Folders::MENU));
	pInactiveContainerSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcontainer.png", Folders::MENU));
	pActiveContainerSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcontaineractive.png", Folders::MENU));
	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcursor.png", Folders::MENU));

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 1));

	sf::Texture* ScrollTexture = TextureManager::GetInstance()->LoadTexture("scrollbuttons.png", Folders::FolderType::MENU);
	ScrollUpEnabled = new sf::Sprite(*ScrollTexture, sf::IntRect(16, 0, 16, 16));
	ScrollUpEnabled->setPosition(32, 90);
	ScrollUpDisabled = new sf::Sprite(*ScrollTexture, sf::IntRect(16, 16, 16, 16));
	ScrollUpDisabled->setPosition(32, 90);
	ScrollDownEnabled = new sf::Sprite(*ScrollTexture, sf::IntRect(0, 0, 16, 16));
	ScrollDownEnabled->setPosition(32, 479);
	ScrollDownDisabled = new sf::Sprite(*ScrollTexture, sf::IntRect(0, 16, 16, 16));
	ScrollDownDisabled->setPosition(32, 479);
}

ArenaUnitSelectScreen::~ArenaUnitSelectScreen()
{
}

void ArenaUnitSelectScreen::Start()
{
	UnitCounter = 0;
	Offset = 0;

	CanMoveDown = false;
	CanMoveUp = false;

	InitContainers(Offset);

	pActiveContainer = &Containers.at(0);
	UpdateCursorPosition();
}

void ArenaUnitSelectScreen::Draw()
{
	pCamera->DrawUpdate();

	pWindow->draw(*Background);

	if (CanMoveUp)
	{
		pWindow->draw(*ScrollUpEnabled);
	}
	else
	{
		pWindow->draw(*ScrollUpDisabled);
	}

	if (CanMoveDown)
	{
		pWindow->draw(*ScrollDownEnabled);
	}
	else
	{
		pWindow->draw(*ScrollDownDisabled);
	}

	for (ArenaUnitContainer &Container : Containers)
	{
		if (Container.IsActive)
		{
			pWindow->draw(*Container.ActiveContainerSprite);
			pWindow->draw(*Container.UnitSprite);
			pWindow->draw(*Container.UnitName);
		}
		else
		{
			pWindow->draw(*Container.InactiveContainerSprite);
			pWindow->draw(*Container.UnitSprite);
			pWindow->draw(*Container.UnitName);
		}
		pWindow->draw(*pCursorSprite);
	}
}

void ArenaUnitSelectScreen::Unload()
{
}

void ArenaUnitSelectScreen::InitContainers(int _Offset)
{
	Containers.clear();

	int ContainerX = 63;
	int ContainerY = 102;
	int UnitX = 72;
	int UnitY = 105;
	int NameX = 112;
	int NameY = 110;
	int Counter = 1;
	int Total = _Offset * 14;

	bool ScreenLimitReached = false;

	std::vector<Unit*> ArenaUnits = PlayerManager::GetInstance()->GetArenaUnits();
	std::vector<Unit*> MissionActiveUnits = PlayerManager::GetInstance()->GetReadyUnits();
	std::vector<Unit*> InactiveUnits = PlayerManager::GetInstance()->GetInactiveUnits();

	for (int i = Total; i < SCREEN_LIMIT * (Offset + 1); ++i)
	{
		if (i >= ArenaUnits.size())
		{
			break;
		}

		Unit* pUnit = ArenaUnits.at(i);

		ArenaUnitContainer NewContainer;
		NewContainer.UnitSprite = std::make_shared<sf::Sprite>(*pUnit->GetUnitSprite());
		NewContainer.UnitSprite->setPosition(UnitX, UnitY);

		NewContainer.IsActive = true;

		NewContainer.InactiveContainerSprite = std::make_shared<sf::Sprite>(*pInactiveContainerSprite);
		NewContainer.InactiveContainerSprite->setPosition(ContainerX, ContainerY);
		NewContainer.ActiveContainerSprite = std::make_shared<sf::Sprite>(*pActiveContainerSprite);
		NewContainer.ActiveContainerSprite->setPosition(ContainerX, ContainerY);

		std::shared_ptr<sf::Text> Title = std::make_shared<sf::Text>();
		Title->setFont(*ResourceManager::GetInstance()->GetAceFont());
		Title->setColor(sf::Color::White);
		Title->setString(pUnit->getCHAR()->GetNAME());
		Title->setPosition(NameX, NameY);
		Title->setCharacterSize(16);
		NewContainer.UnitName = Title;
		NewContainer.pUnit = pUnit;

		NewContainer.Position = sf::Vector2f(ContainerX, ContainerY);

		Containers.push_back(NewContainer);

		if (Counter == 2)
		{
			ContainerX = 63;
			UnitX = 72;
			NameX = 112;

			ContainerY += 54;
			UnitY += 54;
			NameY += 54;

			Counter = 1;
		}
		else
		{
			ContainerX += 200;
			UnitX += 200;
			NameX += 200;
			Counter += 1;
		}

		++Total;
		if (Total == (SCREEN_LIMIT + (_Offset * SCREEN_LIMIT)))
		{
			ScreenLimitReached = true;
			break;
		}
	}

	Total = 0;

	std::vector<Unit*> NonArenaUnits;
	NonArenaUnits.insert(end(NonArenaUnits), begin(MissionActiveUnits), end(MissionActiveUnits));
	NonArenaUnits.insert(end(NonArenaUnits), begin(InactiveUnits), end(InactiveUnits));

	if (!ScreenLimitReached)
	{
		for (int i = Total; i < SCREEN_LIMIT * (Offset + 1); ++i)
		{
			if (i >= NonArenaUnits.size())
			{
				break;
			}

			Unit* pUnit;
			pUnit = NonArenaUnits.at(i);

			if(pUnit->InArenaSquad)
			{
				continue;
			}

			ArenaUnitContainer NewContainer;

			NewContainer.UnitSprite = std::make_shared<sf::Sprite>(*pUnit->GetUnitSprite());
			NewContainer.UnitSprite->setPosition(UnitX, UnitY);

			NewContainer.IsActive = false;

			NewContainer.InactiveContainerSprite = std::make_shared<sf::Sprite>(*pInactiveContainerSprite);
			NewContainer.InactiveContainerSprite->setPosition(ContainerX, ContainerY);
			NewContainer.ActiveContainerSprite = std::make_shared<sf::Sprite>(*pActiveContainerSprite);
			NewContainer.ActiveContainerSprite->setPosition(ContainerX, ContainerY);

			std::shared_ptr<sf::Text> Title = std::make_shared<sf::Text>();
			Title->setFont(*ResourceManager::GetInstance()->GetAceFont());
			Title->setColor(sf::Color::White);
			Title->setString(pUnit->getCHAR()->GetNAME());
			Title->setPosition(NameX, NameY);
			Title->setCharacterSize(16);
			NewContainer.UnitName = Title;
			NewContainer.pUnit = pUnit;

			NewContainer.Position = sf::Vector2f(ContainerX, ContainerY);

			Containers.push_back(NewContainer);

			if (Counter == 2)
			{
				ContainerX = 63;
				UnitX = 72;
				NameX = 112;

				ContainerY += 54;
				UnitY += 54;
				NameY += 54;

				Counter = 1;
			}
			else
			{
				ContainerX += 200;
				UnitX += 200;
				NameX += 200;
				Counter += 1;
			}

			++Total;
			if (Total == (SCREEN_LIMIT + _Offset))
			{
				break;
			}
		}
	}

	if (SCREEN_LIMIT * (Offset + 1) < (InactiveUnits.size() + ArenaUnits.size()))
	{
		CanMoveDown = true;
	}
	else
	{
		CanMoveDown = false;
	}

	if (Offset > 0)
	{
		CanMoveUp = true;
	}
	else
	{
		CanMoveUp = false;
	}
}

void ArenaUnitSelectScreen::UpdateCursorPosition()
{
	pCursorSprite->setPosition(pActiveContainer->Position);
}

void ArenaUnitSelectScreen::PollInput()
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
				ScreenManager::GetInstance()->GetArenaScreen()->UpdateCounter();
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
				ScreenManager::GetInstance()->CloseScreen();
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				if (!pActiveContainer->pUnit->InArenaSquad && PlayerManager::GetInstance()->GetArenaUnits().size() + 1 <= 16 || pActiveContainer->pUnit->InArenaSquad)
				{
					PlayerManager::GetInstance()->SwitchUnitFromArena(pActiveContainer->pUnit);
					pActiveContainer->IsActive = pActiveContainer->IsActive ? false : true;
					if (pActiveContainer->IsActive)
					{
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
					}
					else
					{
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
					}
				}
				else
				{
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->ErrorSound, false);
				}
			}
			else if (event.key.code == sf::Keyboard::Right)
			{
				if (UnitCounter + 1 < Containers.size())
				{
					++UnitCounter;
					pActiveContainer = &Containers.at(UnitCounter);
					UpdateCursorPosition();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
				else if (CanMoveDown)
				{
					Offset += 1;
					UnitCounter = 0;
					pActiveContainer = &Containers.at(0);
					InitContainers(Offset);
					UpdateCursorPosition();
				}
			}
			else if (event.key.code == sf::Keyboard::Left)
			{
				if (UnitCounter > 0)
				{
					--UnitCounter;
					pActiveContainer = &Containers.at(UnitCounter);
					UpdateCursorPosition();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
				else if (CanMoveUp)
				{
					Offset -= 1;
					UnitCounter = 0;
					pActiveContainer = &Containers.at(0);
					InitContainers(Offset);
					UpdateCursorPosition();
				}
			}
			else if (event.key.code == sf::Keyboard::Return)
			{
				ScreenManager::GetInstance()->GetUnitOverviewScreen()->SetUnit(pActiveContainer->pUnit);
				ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetUnitOverviewScreen());
			}
			else if (event.key.code == sf::Keyboard::I)
			{
				ScreenManager::GetInstance()->GetUnitOverviewScreen()->SetUnit(pActiveContainer->pUnit);
				ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetUnitOverviewScreen());
			}
		}
	}
}