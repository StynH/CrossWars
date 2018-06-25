#include "BarracksScreen.h"
#include "TextureManager.h"
#include "MusicPlayer.h"
#include "ScreenManager.h"
#include "StateMachine.h"
#include "TextSanitizer.h"

BarracksScreen::BarracksScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	sf::Texture* MenuTexture = TextureManager::GetInstance()->LoadTexture("barracksmenu.png", Folders::FolderType::MENU);

	Background = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("barracks.png", Folders::MENU));
	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons//menuselector.png", Folders::FolderType::BATTLE));

	MissionIndicator = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("missionname.png", Folders::MENU));
	MissionIndicator->setPosition(222, 427);

	MissionName = std::make_shared<sf::Text>();
	MissionName->setColor(sf::Color::White);
	MissionName->setFont(*ResourceManager::GetInstance()->GetAceFont());
	MissionName->setCharacterSize(16);

	pArmyButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 0, 177, 34)), pWindow, sf::Vector2f(32, 112));
	pArmyButton->Fire += std::bind(&BarracksScreen::OpenArmyScreen, this);

	pMissionButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 34, 177, 34)), pWindow, sf::Vector2f(32, 150));
	pMissionButton->Fire += std::bind(&BarracksScreen::OpenMissionScreen, this);

	pItemCreationButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 102, 177, 34)), pWindow, sf::Vector2f(32, 188));
	pItemCreationButton->Fire += std::bind(&BarracksScreen::OpenItemCreationScreen, this);

	pItemStorageButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 136, 177, 34)), pWindow, sf::Vector2f(32, 226));
	pItemStorageButton->Fire += std::bind(&BarracksScreen::OpenItemStorageScreen, this);

	pArenaButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 204, 177, 34)), pWindow, sf::Vector2f(32, 264));
	pArenaButton->Fire += std::bind(&BarracksScreen::OpenArenaScreen, this);

	pStartButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 68, 177, 34)), pWindow, sf::Vector2f(32, 74));
	pStartButton->Fire += std::bind(&BarracksScreen::StartMission, this);
	pStartButton->Available = false;

	pSaveButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 170, 177, 34)), pWindow, sf::Vector2f(32, 302));
	pSaveButton->Fire += std::bind(&PlayerManager::CreateSaveFile, PlayerManager::GetInstance());
	pSaveButton->Fire += std::bind(&SoundPlayer::PlaySound, SoundPlayer::GetInstance(), "NewGame.wav", false);

	ButtonList.Insert(pStartButton);
	ButtonList.Insert(pArmyButton);
	ButtonList.Insert(pMissionButton);
	ButtonList.Insert(pItemCreationButton);
	ButtonList.Insert(pItemStorageButton);
	ButtonList.Insert(pArenaButton);
	ButtonList.Insert(pSaveButton);

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 1));
}

void BarracksScreen::Start()
{
	SoundPlayer::GetInstance()->StopMusic();
	SoundPlayer::GetInstance()->PlayMusic("Prepare for The Assault.wav", true);

	while (!ButtonList.Front()->Available)
	{
		ButtonList.FrontToBack();
	}
}

void BarracksScreen::Update()
{
	pCursorSprite->setPosition(ButtonList.Front()->GetPosition());
}

void BarracksScreen::PollInput()
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
				ButtonList.Front()->Fire(nullptr);
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				ButtonList.BackToFront();
				while(!ButtonList.Front()->Available)
				{
					ButtonList.BackToFront();
				}
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				ButtonList.FrontToBack();
				while (!ButtonList.Front()->Available)
				{
					ButtonList.FrontToBack();
				}
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
		}
	}
}

BarracksScreen::~BarracksScreen()
{
}

void BarracksScreen::Draw()
{
	pCamera->DrawUpdate();
	pWindow->draw(*Background);

	for (UIButton* pButton : ButtonList.GetInnerVector())
	{
		if (pButton->Available)
		{
			pButton->Draw();
		}
	}

	if(pStartButton->Available)
	{
		pWindow->draw(*MissionIndicator);
		pWindow->draw(*MissionName);
	}

	pWindow->draw(*pCursorSprite);
}

void BarracksScreen::Unload()
{

}

void BarracksScreen::OpenArmyScreen()
{
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetArmyScreen());
}

void BarracksScreen::OpenMissionScreen()
{
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetMissionScreen());
}

void BarracksScreen::OpenItemCreationScreen()
{
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetItemCreationScreen());
}

void BarracksScreen::OpenItemStorageScreen()
{
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetStorageScreen());
}

void BarracksScreen::OpenArenaScreen()
{
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetArenaScreen());
}

void BarracksScreen::SetChapter(std::string _ID, std::string _MissionName, int _UnitLimit)
{
	ChapterID = _ID;
	ScreenManager::GetInstance()->GetArmyScreen()->LimitUnits(_UnitLimit);

	pStartButton->Available = true;
	MissionName->setString(_MissionName);
	TextSanitizer::CenterOrigin(MissionName.get());
	MissionName->setPosition(388, 489);
}

void BarracksScreen::ResetBarracks()
{
	ChapterID = "";
	ScreenManager::GetInstance()->GetArmyScreen()->LimitUnits(9999);

	ButtonList.FrontToBack();
	pStartButton->Available = false;
}

void BarracksScreen::StartMission()
{
	StateMachine::GetInstance()->LoadChapter(LuaManager::GetInstance()->LoadChapter(ChapterID));
}