#include "IntermissionScreen.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "MusicPlayer.h"
#include "StateMachine.h"


IntermissionScreen::IntermissionScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	Timer = 0;
	pBattleTransitionShader = new CRTV();

	pRenderTexture = new sf::RenderTexture();
	pRenderTexture->create(512, 512);
}

IntermissionScreen::~IntermissionScreen()
{
}

void IntermissionScreen::Start()
{
	Timer = 0;
	FinalWait = false;
	SoundPlayer::GetInstance()->PlayMusic("IsThisMusic.wav", true);

	SlideQueue.empty();
}

void IntermissionScreen::Update()
{
	pBattleTransitionShader->Update();
	Timer += GameTime::DeltaTime;
	if (Timer > 3.5)
	{
		if(!FinalWait)
		{
			SlideQueue.pop();
			if (SlideQueue.size() == 0)
			{
				SoundPlayer::GetInstance()->StopMusic();
				SoundPlayer::GetInstance()->PlaySound("ScreenTurnOff.wav", false);
				FinalWait = true;
			}
			Timer = 0;
		}
		else
		{
			StateMachine::GetInstance()->NextState();
		}
	}
}

void IntermissionScreen::Draw()
{
	if(!FinalWait)
	{
		pRenderTexture->draw(*SlideQueue.front());
		pRenderTexture->display();

		pWindow->draw(sf::Sprite(pRenderTexture->getTexture()), pBattleTransitionShader->GetShader());
	}
}

void IntermissionScreen::LoadIntermission(std::string _IntermissionName)
{
	boost::filesystem::path Path(Folders::ROOT_FOLDER + Folders::INTERMISSION_FOLDER + _IntermissionName);
	boost::filesystem::directory_iterator EndIterator;
	for (boost::filesystem::directory_iterator itr(Path); itr != EndIterator; ++itr)
	{
		if (is_regular_file(itr->path())) {
			std::string Name = itr->path().stem().string();
			std::string File = _IntermissionName + "\\" + Name + ".png";
			sf::Sprite* Sprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture(File, Folders::FolderType::INTERMISSION));
			SlideQueue.push(Sprite);
		}
	}
}

void IntermissionScreen::Unload()
{
}