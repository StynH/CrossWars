#include "DialogueScreen.h"
#include "TextSanitizer.h"
#include "ScreenManager.h"
#include "StateMachine.h"
#include "MusicPlayer.h"
#include "EventLogger.h"

DialogueScreen::DialogueScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	DialogueFont = ResourceManager::GetInstance()->GetAceFont();

	DialogueBox = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("box.png", Folders::FolderType::DIALOGUE));
	Overlay = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overlay.png", Folders::FolderType::DIALOGUE));
	Gradient = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("gradient.png", Folders::FolderType::DIALOGUE));
	LowerFiller = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("lower.png", Folders::FolderType::DIALOGUE));
	LowerFiller->setPosition(0, 256);

	TextRenderer = new sf::Text();
	TextRenderer->setFont(*DialogueFont);
	TextRenderer->setCharacterSize(18);
	TextRenderer->setPosition(56, 182);
	TextRenderer->setStyle(sf::Text::Bold);
	TextRenderer->setColor(sf::Color::White);

	NameRenderer = new sf::Text();
	NameRenderer->setFont(*DialogueFont);
	NameRenderer->setCharacterSize(18);
	NameRenderer->setStyle(sf::Text::Bold);
	NameRenderer->setColor(sf::Color::White);

	SceneTimer = 0;

	UpperScreen = new TextContainer(_pRenderWindow, "", DialogueFont, sf::Vector2f(56, 182), 420);
	LowerScreen = new TextContainer(_pRenderWindow, "", DialogueFont, sf::Vector2f(40, 272), 432);

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 1));
}


void DialogueScreen::Draw()
{
	pCamera->DrawUpdate();

	Scene* CurrentScene = GetCurrentScene();

	pWindow->draw(*Background);
	//pWindow->draw(*Gradient);

	if(CurrentScene->CounterActor->Sprite != nullptr)
	{
		pWindow->draw(*CurrentScene->CounterActor->Sprite);
	}

	if (CurrentScene->Actor->Sprite != nullptr)
	{
		pWindow->draw(*CurrentScene->Actor->Sprite);
	}

	pWindow->draw(*DialogueBox);
	pWindow->draw(*LowerFiller);

	LowerScreen->Draw();

	UpperScreen->SetText(CurrentText, false);
	UpperScreen->Draw();

	NameRenderer->setString(CurrentScene->Name);

	TextSanitizer::CenterOrigin(NameRenderer);
	NameRenderer->setPosition(136, 163);
	pWindow->draw(*NameRenderer);	
}

void DialogueScreen::Update()
{
	SceneTimer += (TEXTSPEED * GameTime::DeltaTime);
	CurrentText = GetCurrentScene()->Text.substr(0, SceneTimer);

	if(CurrentText.length() > PreviousTextLength + 2 && SceneEnded == false)
	{
		PreviousTextLength = CurrentText.length();
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->TalkSound, false);
	}

	CheckScene();
}

void DialogueScreen::PollInput()
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
			if (event.key.code == sf::Keyboard::Z && SceneEnded)
			{
				NextScene();
			}
		}
	}
}

void DialogueScreen::CheckScene()
{
	if (CurrentText.size() == GetCurrentScene()->Text.size()) {
		SceneEnded = true;
	}
}

void DialogueScreen::NextScene()
{
	if (Scenes.size() - 1 != 0) {
		Popscene();
		SceneTimer = 0;
		PreviousTextLength = 0;
		SceneEnded = false;
		
		if(Scenes.front()->StopMusic)
		{
			SoundPlayer::GetInstance()->FadeOutMusic(0.2f);
		}
		else if(Scenes.front()->MusicChange)
		{
			SoundPlayer::GetInstance()->PlaySpecificMusic(Scenes.front()->MusicChange.get());
		}
	}
	else
	{
		Popscene();
		StateMachine::GetInstance()->NextState();
	}
}

void DialogueScreen::Popscene()
{
	Scenes.pop();
}

void DialogueScreen::LoadDialogue(std::string _scenename)
{
	SceneEnded = false;
	PreviousTextLength = 0;
	SceneTimer = 0;

	std::ifstream file(Folders::ROOT_FOLDER + Folders::DIALOGUE_FOLDER + Folders::MAIN_SCENEFOLDER + _scenename + ".json");
	nlohmann::json j = nlohmann::json::parse(file);

	for (auto scene : j["parts"])
	{
		Scene* NewScene = new Scene();

		MiniActor* SpeakerActor = new MiniActor();
		Character* SpeakerCopy = UnitManager::GetInstance()->GetCharacter(scene["speaker"].get<std::string>());

		SpeakerActor->Name = SpeakerCopy->GetNAME();
		SpeakerActor->Sprite = std::make_shared<sf::Sprite>(sf::Sprite(*SpeakerCopy->DialougeSprite->Normal));
		NewScene->Actor = std::shared_ptr<MiniActor>(SpeakerActor);

		MiniActor* ListenerActor = new MiniActor();
		if (scene["listener"].get<std::string>().compare("none") != 0)
		{
			Character* ListenerCopy = UnitManager::GetInstance()->GetCharacter(scene["listener"].get<std::string>());

			ListenerActor->Name = ListenerCopy->GetNAME();
			ListenerActor->Sprite = std::make_shared<sf::Sprite>(sf::Sprite(*ListenerCopy->DialougeSprite->Reversed));
		}
		else
		{
			ListenerActor->Name = "???";
		}

		if (scene["active"].get<std::string>() == "speaker")
		{
			if (scene["listener"].get<std::string>().compare("none") != 0)
			{
				ListenerActor->Sprite->setColor(sf::Color(125, 125, 125, 255));
			}
			NewScene->Name = SpeakerActor->Name;
		}
		else
		{
			SpeakerActor->Sprite->setColor(sf::Color(125, 125, 125, 255));
			NewScene->Name = ListenerActor->Name;
		}

		NewScene->CounterActor = std::shared_ptr<MiniActor>(ListenerActor);

		NewScene->Text = TextSanitizer::SanitizeString(scene["text"].get<std::string>());
		Scenes.push(NewScene);

		NewScene->MusicChange = nullptr;
		if (scene.find("changemusic") != scene.end()) {
			NewScene->MusicChange = std::make_shared<sf::Music>();
			if (!NewScene->MusicChange->openFromFile(Folders::ROOT_FOLDER + Folders::MUSIC_FOLDER + scene["changemusic"].get<std::string>()))
			{
				EventContext ec("Cannot find music file: ", scene["changemusic"].get<std::string>().c_str());
			}
			NewScene->MusicChange->setLoop(true);
		}

		NewScene->StopMusic = false;
		if (scene.find("stopmusic") != scene.end()) {
			NewScene->StopMusic = scene["stopmusic"].get<bool>();
		}
	}

	Background = ResourceManager::GetInstance()->GetBackground(j["background"].get<std::string>());
	LowerScreen->SetText(TextSanitizer::SanitizeString(j["description"].get<std::string>()), false);

	if (Scenes.front()->StopMusic)
	{
		SoundPlayer::GetInstance()->FadeOutMusic(1.0f);
	}
	else if (Scenes.front()->MusicChange)
	{
		SoundPlayer::GetInstance()->PlaySpecificMusic(Scenes.front()->MusicChange.get());
	}
}

DialogueScreen::~DialogueScreen()
{
	SafeDelete(DialogueFont);
	SafeDelete(DialogueBox);
	SafeDelete(Overlay);
	SafeDelete(Gradient);
	SafeDelete(LowerFiller);
	SafeDelete(TextRenderer);
	SafeDelete(NameRenderer);
}
