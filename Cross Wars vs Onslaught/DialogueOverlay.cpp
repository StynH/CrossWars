#include "DialogueOverlay.h"
#include "ResourceManager.h"
#include "TextSanitizer.h"
#include "MusicPlayer.h"
#include "EventLogger.h"

DialogueOverlay::DialogueOverlay(sf::RenderWindow* _pRenderWindow): GameScreenOverlay(_pRenderWindow)
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
	SceneEnded = true;

	UpperScreen = new TextContainer(_pRenderWindow, "", DialogueFont, sf::Vector2f(56, 182), 420);
}

void DialogueOverlay::Draw()
{
	Scene* CurrentScene = GetCurrentScene();

	pWindow->draw(*Gradient);

	if (CurrentScene->CounterActor->Sprite != nullptr)
	{
		pWindow->draw(*CurrentScene->CounterActor->Sprite);
	}

	if (CurrentScene->Actor->Sprite != nullptr)
	{
		pWindow->draw(*CurrentScene->Actor->Sprite);
	}

	pWindow->draw(*DialogueBox);
	pWindow->draw(*LowerFiller);

	UpperScreen->SetText(CurrentText, false);
	UpperScreen->Draw();

	NameRenderer->setString(CurrentScene->Name);

	TextSanitizer::CenterOrigin(NameRenderer);
	NameRenderer->setPosition(136, 163);
	pWindow->draw(*NameRenderer);
}

void DialogueOverlay::Reset()
{
}

void DialogueOverlay::Update()
{
	SceneTimer += (DialogueScreen::TEXTSPEED * GameTime::DeltaTime);
	CurrentText = GetCurrentScene()->Text.substr(0, SceneTimer);

	if (CurrentText.length() > PreviousTextLength + 2 && SceneEnded == false)
	{
		PreviousTextLength = CurrentText.length();
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->TalkSound, false);
	}

	CheckScene();
}

void DialogueOverlay::Load()
{
}

void DialogueOverlay::Unload()
{
}

void DialogueOverlay::Hide()
{

}

void DialogueOverlay::PollInput()
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

void DialogueOverlay::NextScene()
{
	if (Scenes.size() - 1 != 0) {
		Popscene();
		SceneTimer = 0;
		PreviousTextLength = 0;
		SceneEnded = false;
	}
	else
	{
		CloseScreen = true;
		MustUnload = true;
	}
}

void DialogueOverlay::Popscene()
{
	Scenes.pop();
}

void DialogueOverlay::CheckScene()
{
	if (CurrentText.size() == GetCurrentScene()->Text.size()) {
		SceneEnded = true;
	}
}

void DialogueOverlay::LoadDialogue(std::string _SceneName)
{
	SceneEnded = false;
	PreviousTextLength = 0;
	SceneTimer = 0;

	std::ifstream file(Folders::ROOT_FOLDER + Folders::LEVEL_DIALOGUEFOLDER + _SceneName + ".json");
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
	}
}

DialogueOverlay::~DialogueOverlay()
{
	SafeDelete(DialogueFont);
	SafeDelete(DialogueBox);
	SafeDelete(Overlay);
	SafeDelete(Gradient);
	SafeDelete(LowerFiller);
	SafeDelete(TextRenderer);
	SafeDelete(NameRenderer);
}