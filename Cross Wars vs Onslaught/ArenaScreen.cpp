#include "ArenaScreen.h"
#include "ResourceManager.h"
#include "TextSanitizer.h"
#include "MusicPlayer.h"
#include "ScreenManager.h"
#include "MapManager.h"

ArenaScreen::ArenaScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pBackground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("arena.png", Folders::MENU));

	pUnitCounter = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("unitcounter.png", Folders::ARENA));
	pUnitCounter->setPosition(50, 102);

	pUnitCounterText = new sf::Text();
	pUnitCounterText->setFont(*ResourceManager::GetInstance()->GetAceFont());
	pUnitCounterText->setCharacterSize(28);

	pDifficultySprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("difficulty.png", Folders::ARENA));
	pDifficultySprite->setPosition(50, 289);

	pDifficultySelector = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("difficultyglow.png", Folders::ARENA));

	pButtonCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("cursor.png", Folders::ARENA));

	pUnitSelectButton = new UIButton(new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("selectunits.png", Folders::ARENA), sf::IntRect(0, 0, 224, 64)), pWindow, sf::Vector2f(50, 182));
	pUnitSelectButton->Fire += std::bind(&ArenaScreen::ShowUnitSelectScreen, this);

	pStartButton = new UIButton(new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("arenabutton.png", Folders::ARENA), sf::IntRect(0, 0, 224, 64)), pWindow, sf::Vector2f(50, 426));
	pStartButton->Fire += std::bind(&ArenaScreen::StartArena, this);

	ButtonList.push_back(pUnitSelectButton);
	ButtonList.push_back(pStartButton);

	UpdateCounter();
}

ArenaScreen::~ArenaScreen()
{
}

void ArenaScreen::Start()
{
	SoundPlayer::GetInstance()->PlayMusic("Prepare for Virtual Hell.wav", true);

	Counter = 0;

	SelectingDifficulty = false;
	UpdateCursorPosition();

	CurrentDifficulty = EASY;
	UpdateDifficultyCursor();
}

void ArenaScreen::Draw()
{
	pWindow->draw(*pBackground);

	pWindow->draw(*pUnitCounter);
	pWindow->draw(*pUnitCounterText);

	pWindow->draw(*pDifficultySprite);
	pWindow->draw(*pDifficultySelector);

	for(UIButton* pButton : ButtonList)
	{
		pButton->Draw();
	}

	if(!SelectingDifficulty)
	{
		pWindow->draw(*pButtonCursorSprite);
	}
}

void ArenaScreen::Unload()
{
}

void ArenaScreen::UpdateCounter()
{
	pUnitCounterText->setString(std::to_string(PlayerManager::GetInstance()->GetArenaUnits().size()) + "/16");
	TextSanitizer::CenterOrigin(pUnitCounterText);
	pUnitCounterText->setPosition(90, 142);
}

void ArenaScreen::UpdateCursorPosition()
{
	pButtonCursorSprite->setPosition(ButtonList.at(Counter > 0 ? Counter - 1 : Counter)->GetPosition());
}

void ArenaScreen::UpdateDifficultyCursor()
{
	pDifficultySelector->setPosition(82 + (56 * static_cast<int>(CurrentDifficulty)), 340);
}

void ArenaScreen::StartArena()
{
	if(PlayerManager::GetInstance()->GetArenaUnits().size() == 0)
	{
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->ErrorSound, false);
		return;
	}

	int MedianLevel = 0;
	for(Unit* pUnit : PlayerManager::GetInstance()->GetArenaUnits())
	{
		MedianLevel += pUnit->getLVL();
	}

	int AmountOfPlayerUnits = PlayerManager::GetInstance()->GetArenaUnits().size();
	MedianLevel = MedianLevel / AmountOfPlayerUnits;

	switch(CurrentDifficulty)
	{
	case EASY: 
		AmountOfPlayerUnits -= 2;
		break;
	case HARD: 
		AmountOfPlayerUnits += 2;
		break;
	}

	if(AmountOfPlayerUnits < 1)
	{
		AmountOfPlayerUnits = 1;
	}
	else if(AmountOfPlayerUnits > 16)
	{
		AmountOfPlayerUnits = 16;
	}

	int SpawnX = 10;
	int SpawnY = 9;

	Map* ArenaMap = MapManager::GetInstance()->LoadMap("..\\" + Folders::ARENA_FOLDER + "map");
	for (int i = 0; i < AmountOfPlayerUnits; ++i)
	{
		Unit* pUnit = UnitManager::GetInstance()->GenerateUnit(CurrentDifficulty, MedianLevel);

		sf::Vector2f WorldPosition(SpawnX * Tile::TILESIZE, SpawnY * Tile::TILESIZE);
		pUnit->setPosition(WorldPosition, sf::Vector2f(SpawnX, SpawnY));

		pUnit->setAIType(Unit::CHASER);
		pUnit->AIUnitTarget = nullptr;
		
		ArenaMap->AddUnit(pUnit, Unit::ENEMY);

		if(SpawnY < 10)
		{
			++SpawnY;
		}
		else
		{
			--SpawnY;
		}
		++SpawnX;
	}
	
	ScreenManager::GetInstance()->GetLevelScreen()->SetArenaMode();
	ScreenManager::GetInstance()->GetLevelScreen()->SetNewMap(ArenaMap);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetLevelScreen());
}

void ArenaScreen::ShowUnitSelectScreen()
{
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetArenaUnitSelectScreen());
}

void ArenaScreen::PollInput()
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
				ButtonList.at(Counter > 0 ? Counter - 1 : Counter)->Fire(nullptr);
			}
			else if (event.key.code == sf::Keyboard::X)
			{
				ScreenManager::GetInstance()->CloseScreen();
				SoundPlayer::GetInstance()->PlayMusic("Prepare for The Assault.wav", true);
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				if (!SelectingDifficulty && Counter == ButtonList.size())
				{
					SelectingDifficulty = true;
					--Counter;
				}
				else if (SelectingDifficulty && Counter > 0)
				{
					SelectingDifficulty = false;
					--Counter;

					UpdateCursorPosition();
				}
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				if (!SelectingDifficulty && Counter == 0)
				{
					SelectingDifficulty = true;
					++Counter;
				}
				else if(SelectingDifficulty && Counter < ButtonList.size())
				{
					SelectingDifficulty = false;
					++Counter;

					UpdateCursorPosition();
				}
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
			}
			else if (event.key.code == sf::Keyboard::Right)
			{
				if(SelectingDifficulty)
				{
					if(CurrentDifficulty < max)
					{
						CurrentDifficulty = static_cast<DIFFICULTY>(CurrentDifficulty + 1);
						UpdateDifficultyCursor();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Left)
			{
				if (SelectingDifficulty)
				{
					if (CurrentDifficulty > min)
					{
						CurrentDifficulty = static_cast<DIFFICULTY>(CurrentDifficulty - 1);
						UpdateDifficultyCursor();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					}
				}
			}
		}
	}
}
