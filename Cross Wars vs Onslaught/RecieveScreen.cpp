#include "RecieveScreen.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "GameTime.h"
#include "MusicPlayer.h"
#include "TextSanitizer.h"
#include "StateMachine.h"

RecieveScreen::RecieveScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pBackground = new sf::Sprite(*ResourceManager::GetInstance()->GetBlackBackground());
	pBlackScreen = new sf::Sprite(*pBackground);

	pItemContainer = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("recieve\\itemcontainer.png", Folders::HUD));
	pUnitContainer = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("recieve\\unitcontainer.png", Folders::HUD));

	pText = new sf::Text();
	pText->setFont(*ResourceManager::GetInstance()->GetCodersCruxFont());
	pText->setColor(sf::Color::White);
	pText->setCharacterSize(24);

	pRenderTexture = new sf::RenderTexture();
	MustUnload = true;
}


RecieveScreen::~RecieveScreen()
{
}

void RecieveScreen::Start()
{
	InIntro = true;
	InOutro = false;
	Timer = 0.0f;
	Alpha = 0.0f;
	pBlackScreen->setColor(sf::Color(255, 255, 255, 0));
	SoundPlayer::GetInstance()->StopMusic();
	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->RecieveSound, false);
}

void RecieveScreen::Draw()
{
	pWindow->draw(*pBackground);
	pWindow->draw(*pCurrentContainer);
	pWindow->draw(*pBlackScreen);
}

void RecieveScreen::Update()
{
	if(InIntro)
	{
		pCurrentContainer->setOrigin(pCurrentContainer->getLocalBounds().width / 2, pCurrentContainer->getLocalBounds().height / 2);
		pCurrentContainer->setPosition(256, 256);

		if (Scale < 1.5f && !BounceBack)
		{
			Scale += GameTime::DeltaTime * ScaleSpeed;
			pCurrentContainer->setScale(Scale, Scale);
			ScaleSpeed += GameTime::DeltaTime * 25;
			if (Scale >= 1.5f)
			{
				BounceBack = true;
			}
		}
		else if (BounceBack)
		{
			if (Scale > 1.0f)
			{
				Scale -= GameTime::DeltaTime * ScaleSpeed;
				pCurrentContainer->setScale(Scale, Scale);
			}
			else
			{
				pCurrentContainer->setScale(1.0, 1.0);
				InIntro = false;
			}
		}
	}
	else if (InOutro)
	{
		if(Alpha < 255)
		{
			Alpha += GameTime::DeltaTime * 250;
			pBlackScreen->setColor(sf::Color(255, 255, 255, Alpha));

			if(Alpha >= 255)
			{
				Alpha = 255;
				pBlackScreen->setColor(sf::Color(255, 255, 255, Alpha));
				Timer = 0;
			}
		}
		else
		{
			if(Timer >= 1.5f)
			{
				StateMachine::GetInstance()->NextState();
			}
		}	
	}
	Timer += GameTime::DeltaTime;
}

void RecieveScreen::Unload()
{
	SafeDelete(pCurrentContainer);
}

void RecieveScreen::PollInput()
{
	sf::Event event;

	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Z && Timer >= 1.5f)
			{
				InOutro = true;
			}
		}
	}
}

void RecieveScreen::RecieveItem(Item* _pItem)
{
	Scale = 0.01f;
	ScaleSpeed = 0;
	BounceBack = false;
	InIntro = true;

	pText->setString("You recieved a " + _pItem->getName() + "!");
	TextSanitizer::CenterOrigin(pText);

	pRenderTexture->clear();
	pRenderTexture->create(pItemContainer->getLocalBounds().width, pItemContainer->getLocalBounds().height);

	pText->setPosition(186, 35);
	pRenderTexture->draw(*pItemContainer);
	pRenderTexture->draw(*pText);
	pRenderTexture->display();

	pCurrentContainer = new sf::Sprite(pRenderTexture->getTexture());
	pCurrentContainer->setScale(0.01, 0.01);
}

void RecieveScreen::RecieveUnit(Unit* _pUnit)
{
	Scale = 0.01f;
	ScaleSpeed = -0;
	BounceBack = false;
	InIntro = true;

	pText->setString(_pUnit->getCHAR()->GetNAME() + " has joined your team!");
	TextSanitizer::CenterOrigin(pText);

	pRenderTexture->clear();
	pRenderTexture->create(pUnitContainer->getLocalBounds().width, pUnitContainer->getLocalBounds().height);

	pText->setPosition(223, 87);
	pRenderTexture->draw(*pUnitContainer);
	pRenderTexture->draw(*pText);
	pRenderTexture->display();

	std::shared_ptr<sf::Sprite> pCharSprite = std::make_shared<sf::Sprite>(*_pUnit->getCHAR()->SupportSprite->Normal);
	pCharSprite->setPosition(13, 9);
	pRenderTexture->draw(*pCharSprite);

	pCurrentContainer = new sf::Sprite(pRenderTexture->getTexture());
	pCurrentContainer->setScale(0.01, 0.01);
}

void RecieveScreen::RecieveMission(const PreloadChapter& _pChapter)
{
	ScaleSpeed = 0;
	BounceBack = false;
	InIntro = true;

	pText->setString("Unlocked mission:\n" + _pChapter.Name + "!");
	TextSanitizer::CenterOrigin(pText);

	pRenderTexture->clear();
	pRenderTexture->create(pItemContainer->getLocalBounds().width, pItemContainer->getLocalBounds().height);

	pText->setPosition(186, 35);
	pRenderTexture->draw(*pItemContainer);
	pRenderTexture->draw(*pText);
	pRenderTexture->display();

	pCurrentContainer = new sf::Sprite(pRenderTexture->getTexture());
	pCurrentContainer->setScale(0.01, 0.01);
}