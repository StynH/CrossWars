#include "ChapterScreen.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "TextSanitizer.h"
#include "MusicPlayer.h"
#include "StateMachine.h"


ChapterScreen::ChapterScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pText = new sf::Text();
	pText->setFont(*ResourceManager::GetInstance()->GetAceFont());
	pText->setCharacterSize(18);
	pText->setStyle(sf::Text::Style::Bold);

	pRenderTexture = new sf::RenderTexture();
	pRenderTexture->create(512, 512);

	pBackground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("chapter.png", Folders::MENU));

	pBurnAwayShader = new BurnAway();

	SoundPlayer::GetInstance();
}


ChapterScreen::~ChapterScreen()
{
}

void ChapterScreen::Update()
{
	if(Timer > 7)
	{
		pBurnAwayShader->Update();
	}
	
	
	if(Timer > 11)
	{
		StateMachine::GetInstance()->NextState();
	}
	else
	{
		Timer += GameTime::DeltaTime;
	}
}

void ChapterScreen::Start()
{
	SoundPlayer::GetInstance()->PlayMusic("LetTheAssaultBegin.wav", false);
	pBurnAwayShader->Reset();
	Timer = 0;
}

void ChapterScreen::Draw()
{
	pRenderTexture->clear(sf::Color::Black);

	pRenderTexture->draw(*pBackground);
	pRenderTexture->draw(*pText);

	pRenderTexture->display();

	sf::Sprite RenderTextureSprite(pRenderTexture->getTexture());

	if (Timer > 7)
	{
		pWindow->draw(RenderTextureSprite, pBurnAwayShader->GetShader());
	}
	else
	{
		pWindow->draw(RenderTextureSprite);
	}
}

void ChapterScreen::Unload()
{

}

void ChapterScreen::PollInput()
{

}

void ChapterScreen::SetChapterName(std::string _ChapterName)
{
	pText->setString(_ChapterName);
	TextSanitizer::CenterOrigin(pText);
	pText->setPosition(261, 261);
}
