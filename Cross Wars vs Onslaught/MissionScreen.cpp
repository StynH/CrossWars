#include "MissionScreen.h"
#include "TextureManager.h"
#include "Camera.h"
#include "PlayerManager.h"
#include "ResourceManager.h"
#include "TextSanitizer.h"
#include "MusicPlayer.h"
#include "StateMachine.h"
#include "Chapter.h"

MissionScreen::MissionScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	Background = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("mission.png", Folders::MENU));
	sf::Texture* ContainerTexture = TextureManager::GetInstance()->LoadTexture("missionContainer.png", Folders::MENU);
	pInactiveContainerSprite = new sf::Sprite(*ContainerTexture, sf::IntRect(0, 0, 494, 72));
	pActiveContainerSprite = new sf::Sprite(*ContainerTexture, sf::IntRect(0, 72, 494, 72));

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 1));
}


MissionScreen::~MissionScreen()
{
}

void MissionScreen::Start()
{
	std::vector<PreloadChapter> Missions = PlayerManager::GetInstance()->GetAvailableMissions();

	int y = 90;
	int i = 0;

	for(PreloadChapter Mission : Missions)
	{
		MissionContainer* Container = new MissionContainer;

		Container->ActiveContainerSprite = std::make_shared<sf::Sprite>(*pActiveContainerSprite);
		Container->ActiveContainerSprite->setPosition(9, y);
		Container->InactiveContainerSprite = std::make_shared<sf::Sprite>(*pInactiveContainerSprite);
		Container->InactiveContainerSprite->setPosition(9, y);
		Container->MissionName = std::make_shared<sf::Text>();
		Container->MissionName->setFont(*ResourceManager::GetInstance()->GetAceFont());
		Container->MissionName->setCharacterSize(16);
		Container->MissionName->setString(Mission.Name);

		TextSanitizer::CenterOrigin(Container->MissionName.get());
		Container->MissionName->setPosition(256, y + 51);

		Container->ID = Mission.ID;
		Container->UnitLimit = Mission.UnitLimit;

		if(i == 0)
		{
			Container->Selected = true;
		}
		else
		{
			Container->Selected = false;
		}

		AvailableMissions.Insert(Container);

		y += 72;
		++i;
	}
}

void MissionScreen::Draw()
{
	pCamera->DrawUpdate();

	pWindow->draw(*Background);

	for(MissionContainer* Container : AvailableMissions.GetInnerVector())
	{
		if(Container->Selected)
		{
			pWindow->draw(*Container->ActiveContainerSprite);
		}
		else
		{
			pWindow->draw(*Container->InactiveContainerSprite);
		}
		pWindow->draw(*Container->MissionName);
	}
}

void MissionScreen::Unload()
{
}

void MissionScreen::PollInput()
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
			if (event.key.code == sf::Keyboard::Up)
			{
				AvailableMissions.Front()->Selected = false;
				AvailableMissions.BackToFront();
				AvailableMissions.Front()->Selected = true;
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				AvailableMissions.Front()->Selected = false;
				AvailableMissions.FrontToBack();
				AvailableMissions.Front()->Selected = true;
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
				//StateMachine::GetInstance()->LoadChapter(LuaManager::GetInstance()->LoadChapter(AvailableMissions.Front()->ID));
				ScreenManager::GetInstance()->GetBarracksScreen()->SetChapter(AvailableMissions.Front()->ID, AvailableMissions.Front()->MissionName->getString(), AvailableMissions.Front()->UnitLimit);
				AvailableMissions.Clear();
				ScreenManager::GetInstance()->CloseScreen();
			}
			else if (event.key.code == sf::Keyboard::X)
			{
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
				AvailableMissions.Clear();
				ScreenManager::GetInstance()->CloseScreen();
			}
		}
	}
}
