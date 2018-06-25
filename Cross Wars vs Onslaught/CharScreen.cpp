#include "CharScreen.h"
#include <SFML/Window/Event.hpp>
#include "Folders.h"
#include <iostream>
#include "TextureManager.h"
#include "PlayerManager.h"
#include "ScreenManager.h"
#include "MapManager.h"
#include "StateMachine.h"
#include "MusicPlayer.h"

CharScreen::CharScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pBackground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("charscreen.png", Folders::FolderType::MENU));
	pBackground->setPosition(0, 0);

	Font = ResourceManager::GetInstance()->GetAceFont();

	NameRenderer = new sf::Text();

	NameRenderer->setFont(*Font);
	NameRenderer->setCharacterSize(40);
	NameRenderer->setColor(sf::Color::Black);
	NameRenderer->setPosition(207, 65);

	pRenderTexture = new sf::RenderTexture();
	pRenderTexture->create(512, 512);

	Name = "Zero";

	EndScene = false;
	pBattleTransitionShader = new CRTV();

	Timer = 0;
}


CharScreen::~CharScreen()
{
}

void CharScreen::Start()
{
	SoundPlayer::GetInstance()->PlayMusic("IsThisMusic.wav", true);
}

void CharScreen::Update()
{
	if(EndScene)
	{
		Timer += GameTime::DeltaTime;
		if (Timer > 3)
		{
			SoundPlayer::GetInstance()->StopMusic();
			PlayerManager::GetInstance()->NewGame();
		}
	}
	else
	{
		NameRenderer->setString(Name);
		pBattleTransitionShader->Update();
	}
}

void CharScreen::Draw()
{
	if(!EndScene)
	{
		pRenderTexture->draw(*pBackground);
		pRenderTexture->draw(*NameRenderer);
		pRenderTexture->display();

		pWindow->draw(sf::Sprite(pRenderTexture->getTexture()), pBattleTransitionShader->GetShader());
	}
}

void CharScreen::PollInput()
{	
	sf::Event event;
	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Return && !EndScene)
			{
				MustUnload = true;
				EndScene = true;
				PlayerManager::GetInstance()->SetPlayerName(Name);
			}
		}
		else if (event.type == sf::Event::TextEntered)
		{
			if(event.text.unicode == 8)
			{
				Name = Name.substr(0, Name.size() - 1);
			}
			else if (event.text.unicode >= 97 && event.text.unicode <= 122 && Name.size() < 7 || event.text.unicode >= 65 && event.text.unicode <= 90 && Name.size() < 7)
			{
				Name += static_cast<char>(event.text.unicode);			
			}
		}
	}

}

void CharScreen::Unload()
{
	SafeDelete(pBackground);
	SafeDelete(pBattleTransitionShader);
}