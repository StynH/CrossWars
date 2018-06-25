#include "Include.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

sf::RenderWindow* Window;

/*void RenderThread(sf::RenderWindow* Window)
{
	while (Window->isOpen())
	{
		Window->clear(sf::Color::Black);
		ScreenManager::GetInstance()->DrawScreen();
		Window->display();
	}
}*/

void CrashHandler()
{
	EventLogger::GetInstance()->Dump();
	sf::Sprite CrashScreen(*TextureManager::GetInstance()->LoadTexture("crash.png", Folders::MENU));

	while (Window->isOpen())
	{
		Window->draw(CrashScreen);
		Window->display();
	}

	abort();
}

//Main entry point of the game
int main() {
	PROCESS_MEMORY_COUNTERS pmc;
	set_terminate(CrashHandler);

	EventLogger EventLogger;
	EventLogger.Init();

	//Window Initialization
	Window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cross Wars: VS Onslaught", sf::Style::Titlebar | sf::Style::Close);
	Window->setActive(false);
	srand(time(NULL));

	//LUA Manager loading;
	LuaManager LuaManager;
	LuaManager.Init();

	//Component Loading
	MapGenerator MapGenerator;

	//Manager Loading
	ResourceManager ResourceManager(Window);
	ResourceManager.LoadResources();

	ItemManager::GetInstance()->LoadItems();

	UnitManager::GetInstance()->LoadCharacters();
	UnitManager::GetInstance()->LoadClasses();

	//Load the progress tracker
	PlayerManager::GetInstance()->Load();

	//Preload all screens
	ScreenManager::GetInstance()->SetMenuScreen(new MenuScreen(Window));
	ScreenManager::GetInstance()->SetGameoverScreen(new GameoverScreen(Window));
	ScreenManager::GetInstance()->SetBattleScreen(new BattleScreen(Window));
	ScreenManager::GetInstance()->SetIntermissionScreen(new IntermissionScreen(Window));
	ScreenManager::GetInstance()->SetDialogueScreen(new DialogueScreen(Window));
	ScreenManager::GetInstance()->SetLevelScreen(new LevelScreen(Window));
	ScreenManager::GetInstance()->SetUnitOverviewScreen(new UnitOverviewScreen(Window));
	ScreenManager::GetInstance()->SetBarracksScreen(new BarracksScreen(Window));
	ScreenManager::GetInstance()->SetArmyScreen(new ArmyScreen(Window));
	ScreenManager::GetInstance()->SetMissionScreen(new MissionScreen(Window));
	ScreenManager::GetInstance()->SetItemCreationScreen(new ItemCreationScreen(Window));
	ScreenManager::GetInstance()->SetChapterScreen(new ChapterScreen(Window));
	ScreenManager::GetInstance()->SetRecieveScreen(new RecieveScreen(Window));
	ScreenManager::GetInstance()->SetStorageScreen(new StorageScreen(Window));
	ScreenManager::GetInstance()->SetArenaScreen(new ArenaScreen(Window));
	ScreenManager::GetInstance()->SetArenaUnitSelectScreen(new ArenaUnitSelectScreen(Window));

	//ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetMenuScreen());
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetMenuScreen());

	std::cout << "All screens loaded!" << std::endl;

	//RenderThread disabled.
	//I decided to not do rendering in another thread.
	//I found that the update thread was interfering with the draw thread way too much,
	//causing random errors inconsistantly. I don't think mulithreading a 2D game like this
	//will make much of a difference.
	//sf::Thread thread(&RenderThread, Window);
	//thread.launch();

	//------------------------------ Game Loop ------------------------------//
	float mainThreadTime = 0;
	float passedTime = 0;
	float avgfps = 0;
	float fps = 0;

	GameTime::Init();

	while (Window->isOpen())
	{
		mainThreadTime += GameTime::DeltaTime;

		if (mainThreadTime > 5.0f)
		{
			#ifdef _DEBUG
				GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
				std::cout << "Energy Points: " << PlayerManager::GetInstance()->GetEnergyUnits() << std::endl;
				std::cout << "Runtime(S): " << GameTime::Time << std::endl;
				std::cout << "MainThread(FPS): " << (fps / 5) << std::endl;
				std::cout << "Memory Usage(KB): " << static_cast<float>((pmc.PagefileUsage) / 1000) << std::endl;
				std::cout << "Average FPS: " << (avgfps / passedTime) << "\n\n";
			#endif

			avgfps += fps;
			passedTime += mainThreadTime;

			mainThreadTime = 0;
			fps = 0;
		}

		//Update Logic
		ScreenManager::GetInstance()->UpdateScreen();

		//Update Sound
		SoundPlayer::GetInstance()->Update();

		//Draw Image
		Window->clear(sf::Color::Black);
		ScreenManager::GetInstance()->DrawScreen();
		Window->display();

		//Update GameTime
		GameTime::Update();

		++fps;

		if(ScreenManager::GetInstance()->CloseGame)
		{
			Window->close();
		}
	}

	ScreenManager::DeleteInstance();

	//thread.wait();
	EventLogger::GetInstance()->Dump();
	Window->close();
}
