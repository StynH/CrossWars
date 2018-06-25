#include "GameoverScreen.h"
#include "TextureManager.h"
#include "MusicPlayer.h"
#include "ScreenManager.h"

GameoverScreen::GameoverScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pBackground = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("gameover.png", Folders::FolderType::MENU));

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 1));

	MustUnload = false;
}

GameoverScreen::~GameoverScreen()
{
}

void GameoverScreen::Start()
{
	SoundPlayer::GetInstance()->StopMusic();
	SoundPlayer::GetInstance()->PlayMusic("FallenHero.wav", true);
}

void GameoverScreen::Draw()
{
	pCamera->DrawUpdate();
	pWindow->draw(*pBackground);
}

void GameoverScreen::Unload()
{
}

void GameoverScreen::PollInput()
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
			if (event.key.code == sf::Keyboard::Z)
			{
				ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetBarracksScreen());
			}
		}
	}
}
