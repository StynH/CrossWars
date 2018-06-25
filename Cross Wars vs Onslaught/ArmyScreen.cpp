#include "UnitSelectScreen.h"
#include "ArmyScreen.h"
#include "Folders.h"
#include "TextureManager.h"
#include "ScreenManager.h"
#include "PlayerManager.h"
#include "MusicPlayer.h"

ArmyScreen::ArmyScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	Background = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("army.png", Folders::MENU));
	pInactiveContainerSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcontainer.png", Folders::MENU));
	pActiveContainerSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcontaineractive.png", Folders::MENU));
	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcursor.png", Folders::MENU));

	pVignetteSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("vignette.png", Folders::MENU));

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 1));

	pUnitLimitText = new sf::Text();
	pUnitLimitText->setFont(*ResourceManager::GetInstance()->GetAceFont());
	pUnitLimitText->setPosition(16, 64);
	pUnitLimitText->setCharacterSize(16);

	sf::Texture* ScrollTexture = TextureManager::GetInstance()->LoadTexture("scrollbuttons.png", Folders::FolderType::MENU);
	ScrollUpEnabled = new sf::Sprite(*ScrollTexture, sf::IntRect(16, 0, 16, 16));
	ScrollUpEnabled->setPosition(32, 90);
	ScrollUpDisabled = new sf::Sprite(*ScrollTexture, sf::IntRect(16, 16, 16, 16));
	ScrollUpDisabled->setPosition(32, 90);
	ScrollDownEnabled = new sf::Sprite(*ScrollTexture, sf::IntRect(0, 0, 16, 16));
	ScrollDownEnabled->setPosition(32, 479);
	ScrollDownDisabled = new sf::Sprite(*ScrollTexture, sf::IntRect(0, 16, 16, 16));
	ScrollDownDisabled->setPosition(32, 479);

	pActiveContainer = nullptr;
	UnitCounter = 0;

	pUnitSelectScreen = new UnitSelectScreen(_pRenderWindow);
	pUnitSelectScreen->SetCallback<ArmyScreen>(&ArmyScreen::TradeBetweenUnits, this);
	pUnitSelectScreen->Hide();

	pItemTradeScreen = new ItemTradeOverlay(_pRenderWindow);

	pActiveOverlay = nullptr;

	LimitUnits(9999);
}

ArmyScreen::~ArmyScreen()
{

}

void ArmyScreen::Start()
{
	UnitCounter = 0;
	Offset = 0;

	CanMoveDown = false;
	CanMoveUp = false;

	ShowContainers(Offset);

	pActiveContainer = &Containers.at(0);
	UpdateCursorPosition();
}

void ArmyScreen::Update()
{
	if (pActiveOverlay)
	{
		pActiveOverlay->Update();
		if (pActiveOverlay->CloseScreen)
		{
			pActiveOverlay = nullptr;
		}
		else
		{
			if (pActiveOverlay->CloseGame)
			{
				CloseGame = true;
			}
		}
	}
}

void ArmyScreen::Draw()
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

	for (UnitContainer &Container : Containers)
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

	pWindow->draw(*pUnitLimitText);

	if (pActiveOverlay)
	{
		pWindow->draw(*pVignetteSprite);
		pActiveOverlay->Draw();
	}
}

void ArmyScreen::ShowContainers(int _Offset)
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

	std::vector<Unit*> ReadyUnits = PlayerManager::GetInstance()->GetReadyUnits();
	std::vector<Unit*> InactiveUnits = PlayerManager::GetInstance()->GetInactiveUnits();

	for (int i = Total; i < SCREEN_LIMIT * (Offset + 1); ++i)
	{
		if(i >= ReadyUnits.size())
		{
			break;
		}

		Unit* pUnit = ReadyUnits.at(i);

		UnitContainer NewContainer;
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
		if (Total + 1 == (SCREEN_LIMIT + (_Offset * 14)))
		{
			ScreenLimitReached = true;
			break;
		}
	}

	Total = 0;

	if(!ScreenLimitReached)
	{
		for (int i = Total; i < SCREEN_LIMIT * (Offset + 1); ++i)
		{
			if (i >= InactiveUnits.size())
			{
				break;
			}
			UnitContainer NewContainer;

			Unit* pUnit = InactiveUnits.at(i);

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
			if (Total + 1 == (SCREEN_LIMIT + _Offset))
			{
				break;
			}
		}
	}

	if(SCREEN_LIMIT * (Offset + 1) < (InactiveUnits.size() + ReadyUnits.size()))
	{
		CanMoveDown = true;
	}
	else
	{
		CanMoveDown = false;
	}

	if(Offset > 0)
	{
		CanMoveUp = true;
	}
	else
	{
		CanMoveUp = false;
	}
}

void ArmyScreen::Unload()
{

}

void ArmyScreen::UpdateCursorPosition()
{
	pCursorSprite->setPosition(pActiveContainer->Position);
}

void ArmyScreen::LimitUnits(int _UnitLimit)
{
	UnitLimit = _UnitLimit;

	if(UnitLimit == 9999)
	{
		pUnitLimitText->setString("Allowed Units on next mission: not set");
		return;
	}

	if(PlayerManager::GetInstance()->GetReadyUnits().size() > UnitLimit)
	{
		int Counter = 1;
		for (int i = 0; i < PlayerManager::GetInstance()->GetReadyUnits().size(); ++i)
		{
			Unit* pUnit = PlayerManager::GetInstance()->GetReadyUnits().at(i);
			if(Counter > UnitLimit)
			{
				PlayerManager::GetInstance()->SwitchUnitFromTeams(pUnit);
			}
			++Counter;
		}
	}

	pUnitLimitText->setString("Allowed Units on next mission: " + std::to_string(UnitLimit));
}

void ArmyScreen::TradeBetweenUnits(Unit* _pUnit)
{
	if(_pUnit->getGUID() != pActiveContainer->pUnit->getGUID())
	{
		pItemTradeScreen->SetTraders(pActiveContainer->pUnit, _pUnit);
		pItemTradeScreen->Load();
		pActiveOverlay = pItemTradeScreen;
	}
	else
	{
		ScreenManager::GetInstance()->GetUnitOverviewScreen()->SetUnit(pActiveContainer->pUnit);
		ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetUnitOverviewScreen());
	}
	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
}

void ArmyScreen::PollInput()
{
	sf::Event event;

	if (!pActiveOverlay)
	{
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
					ScreenManager::GetInstance()->CloseScreen();
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					if (pActiveContainer->pUnit->InActiveSquad && PlayerManager::GetInstance()->GetReadyUnits().size() > 1 || !pActiveContainer->pUnit->InActiveSquad && PlayerManager::GetInstance()->GetReadyUnits().size() + 1 <= UnitLimit)
					{
						PlayerManager::GetInstance()->SwitchUnitFromTeams(pActiveContainer->pUnit);
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
						ShowContainers(Offset);
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
						ShowContainers(Offset);
						UpdateCursorPosition();
					}
				}
				else if (event.key.code == sf::Keyboard::Return)
				{
					ScreenManager::GetInstance()->GetUnitOverviewScreen()->SetUnit(pActiveContainer->pUnit);
					ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetUnitOverviewScreen());
				}
				else if (event.key.code == sf::Keyboard::T)
				{
					pUnitSelectScreen->Load();
					pActiveOverlay = pUnitSelectScreen;
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
				}
				else if (event.key.code == sf::Keyboard::I)
				{
					ScreenManager::GetInstance()->GetUnitOverviewScreen()->SetUnit(pActiveContainer->pUnit);
					ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetUnitOverviewScreen());
				}
			}
		}
	}
	else
	{
		pActiveOverlay->PollInput();
	}
}