#include "MenuScreen.h"
#include "TextureManager.h"
#include "MusicPlayer.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "CharScreen.h"
#include "ScreenManager.h"
#include <boost/filesystem/operations.hpp>

MenuScreen::MenuScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	sf::Texture* pButtonTexture = TextureManager::GetInstance()->LoadTexture("mainmenu.png", Folders::MENU);
	pNewGameButton = new UIButton(new sf::Sprite(*pButtonTexture, sf::IntRect(0, 0, 171, 47)), _pRenderWindow, sf::Vector2f(171, 256));
	pNewGameButton->Fire += std::bind(&MenuScreen::NewGame, this);

	pLoadGameButton = new UIButton(new sf::Sprite(*pButtonTexture, sf::IntRect(0, 47, 171, 47)), _pRenderWindow, sf::Vector2f(171, 335));
	pLoadGameButton->Fire += std::bind(&MenuScreen::LoadGame, this);

	ButtonList.Insert(pNewGameButton);
	ButtonList.Insert(pLoadGameButton);

	pButtonGlow = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("buttonglow.png", Folders::MENU));

	pCursorIcon = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("mainmenupointer.png", Folders::MENU));

	pBackground = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("menubg.png", Folders::FolderType::MENU));
	pGlow = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("menuglow.png", Folders::FolderType::MENU));
	pGlow->setColor(sf::Color(255, 255, 255, 0));
	pGlow->setPosition(6, 15);

	pBlack = std::make_shared<sf::Sprite>(*ResourceManager::GetInstance()->GetBlackBackground());

	IntroQueue.push(std::make_pair(2.6f, new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("menu//first.png", Folders::FolderType::MENU))));
	IntroQueue.push(std::make_pair(6.0f, new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("menu//second.png", Folders::FolderType::MENU))));
	IntroQueue.push(std::make_pair(9.2f, new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("menu//third.png", Folders::FolderType::MENU))));

	MustUnload = false;
	InIntro = false;

	UpdateCursor();
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::Start()
{
	SoundPlayer::GetInstance()->PlayMusic("CanYouFeelIt.wav", true);

	Timer = 0.0f;
	Alpha = 255.0f;

	FadeOut = false;
	FadeIn = false;
	FinalWait = false;
	ButtonClicked = false;

	InIntro = true;

	#ifdef _DEBUG
		InIntro = false;
	#endif
}

void MenuScreen::Update()
{
	if(InIntro)
	{
		if (Timer >= IntroQueue.front().first && !FinalWait)
		{
			if (pBlack->getColor().a > 0 && FadeOut == false)
			{
				FadeIn = true;
			}

			if (FadeIn)
			{
				Alpha -= GameTime::DeltaTime * 500;
				pBlack->setColor(sf::Color(255, 255, 255, Alpha));

				if (Alpha <= 0)
				{
					Alpha = 0;
					FadeIn = false;
					FadeOut = true;

					pBlack->setColor(sf::Color(255, 255, 255, Alpha));
				}
			}
			else if (FadeOut)
			{
				Alpha += GameTime::DeltaTime * 300;
				pBlack->setColor(sf::Color(255, 255, 255, Alpha));

				if (Alpha >= 255)
				{
					Alpha = 255;
					FadeOut = false;

					if (IntroQueue.size() - 1 == 0)
					{
						FinalWait = true;
						Timer = 0;
					}
					else
					{
						IntroQueue.pop();
					}

					pBlack->setColor(sf::Color(255, 255, 255, Alpha));
				}
			}
		}
		else if(FinalWait)
		{
			if(Timer >= 2.5f)
			{
				InIntro = false;
				Alpha = 0;
				pBlack->setColor(sf::Color(255, 255, 255, Alpha));
			}
		}

		Timer += GameTime::DeltaTime;
	}
	else if(!ButtonClicked)
	{
		if (pGlow->getColor().a == 0 && FadeOut == false)
		{
			FadeIn = true;
		}

		if (FadeIn)
		{
			Alpha += GameTime::DeltaTime * 150;
			pGlow->setColor(sf::Color(255, 255, 255, Alpha));
			pButtonGlow->setColor(sf::Color(255, 255, 255, Alpha));

			if (Alpha >= 255)
			{
				FadeIn = false;
				FadeOut = true;
				Alpha = 255;
			}
		}
		else if(FadeOut)
		{
			Alpha -= GameTime::DeltaTime * 150;
			pGlow->setColor(sf::Color(255, 255, 255, Alpha));
			pButtonGlow->setColor(sf::Color(255, 255, 255, Alpha));

			if (Alpha <= 0)
			{
				FadeOut = false;
				FadeIn = true;
				Alpha = 0;
			}
		}
	}
	else
	{
		if (pBlack->getColor().a < 255)
		{
			Alpha += GameTime::DeltaTime * 150;
			pBlack->setColor(sf::Color(255, 255, 255, Alpha));
		}
		else if(Timer > 2.5F)
		{
			ButtonList.Front()->Fire(nullptr);
		}
		Timer += GameTime::DeltaTime;
	}
}

void MenuScreen::Draw()
{
	if(!InIntro)
	{
		pWindow->draw(*pBackground);
		pWindow->draw(*pGlow);

		pNewGameButton->Draw();
		pLoadGameButton->Draw();

		pButtonGlow->setPosition(sf::Vector2f(167, 252));
		pWindow->draw(*pButtonGlow);
		pButtonGlow->setPosition(sf::Vector2f(167, 331));
		pWindow->draw(*pButtonGlow);

		pWindow->draw(*pCursorIcon);
	}
	else if(InIntro)
	{
		pWindow->draw(*IntroQueue.front().second);
	}

	if(InIntro || ButtonClicked)
	{
		pWindow->draw(*pBlack);
	}
}

void MenuScreen::UpdateCursor()
{
	int x = ButtonList.Front()->GetPosition().y + 6;
	pCursorIcon->setPosition(64, x);
}

void MenuScreen::Unload()
{
}

void MenuScreen::PollInput()
{
	sf::Event event;

	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			CloseGame = true;
		}
		else if (event.type == sf::Event::KeyPressed && !InIntro && !ButtonClicked)
		{
			if (event.key.code == sf::Keyboard::Up)
			{
				ButtonList.BackToFront();
				UpdateCursor();
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				ButtonList.FrontToBack();
				UpdateCursor();
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				SoundPlayer::GetInstance()->PlaySound("NewGame.wav", false);
				ButtonClicked = true;
				Alpha = 0;
				Timer = 0;
			}
		}
	}
}

void MenuScreen::NewGame()
{
	ScreenManager::GetInstance()->LoadScreen(new CharScreen(ResourceManager::GetInstance()->GetWindow()));
}

void MenuScreen::LoadGame()
{
	if(boost::filesystem::exists("save.txt"))
	{
		PlayerManager::GetInstance()->LoadSaveFile();
		ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetBarracksScreen());
	}
	else
	{
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->ErrorSound, false);
		ButtonClicked = false;
	}
}
