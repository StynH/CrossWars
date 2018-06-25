#include "BattleDialogueOverlay.h"
#include <SFML/Window/Keyboard.hpp>
#include "GameTime.h"
#include "TextSanitizer.h"
#include "Folders.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "UnitManager.h"
#include "MusicPlayer.h"


BattleDialogueOverlay::BattleDialogueOverlay(sf::RenderWindow* _pRenderWindow) : GameScreenOverlay(_pRenderWindow)
{
	DialogueBox = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("battledisplay\\dialogue.png", Folders::FolderType::HUD));

	sf::Font* AceFont = ResourceManager::GetInstance()->GetAceFont();

	TextRenderer = new sf::Text();
	TextRenderer->setFont(*AceFont);
	TextRenderer->setCharacterSize(18);
	TextRenderer->setPosition(56, 182);
	TextRenderer->setStyle(sf::Text::Bold);
	TextRenderer->setColor(sf::Color::White);
	TextRenderer->setPosition(10, 6);

	NameRenderer = new sf::Text();
	NameRenderer->setFont(*AceFont);
	NameRenderer->setCharacterSize(18);
	NameRenderer->setStyle(sf::Text::Bold);
	NameRenderer->setColor(sf::Color::White);

	HasScene = false;
	SceneEnded = false;
}


BattleDialogueOverlay::~BattleDialogueOverlay()
{
}

void BattleDialogueOverlay::Load()
{

}

void BattleDialogueOverlay::Update()
{
	SceneTimer += (TEXTSPEED * GameTime::DeltaTime);
	CurrentText = GetCurrentScene()->Text.substr(0, SceneTimer);

	if (CurrentText.length() > PreviousTextLength + 2 && SceneEnded == false)
	{
		PreviousTextLength = CurrentText.length();
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->TalkSound, false);
	}
	CheckScene();
}

void BattleDialogueOverlay::Draw()
{
	BattleScene* CurrentScene = GetCurrentScene();

	pWindow->draw(*DialogueBox);
	pWindow->draw(*CurrentScene->ActorSprite);

	TextRenderer->setString(CurrentText);
	pWindow->draw(*TextRenderer);

	NameRenderer->setString(GetCurrentScene()->Name);

	TextSanitizer::CenterOrigin(NameRenderer);
	NameRenderer->setPosition(252, 70);
	pWindow->draw(*NameRenderer);	
}

void BattleDialogueOverlay::Unload()
{

}

void BattleDialogueOverlay::Hide()
{

}

void BattleDialogueOverlay::PollInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && SceneEnded)
	{
		NextScene();
	}
}

void BattleDialogueOverlay::CheckScene()
{
	if (CurrentText.size() == GetCurrentScene()->Text.size()) {
		SceneEnded = true;
	}
}

void BattleDialogueOverlay::NextScene()
{
	if (Scenes.size() - 1 != 0) {
		Popscene();
		SceneTimer = 0;
		PreviousTextLength = 0;
		SceneEnded = false;
	}
	else
	{
		Popscene();
		HasScene = false;
	}
}

void BattleDialogueOverlay::Popscene()
{
	Scenes.pop();
}

void BattleDialogueOverlay::LoadDialogue(std::string _SceneName)
{
	SceneEnded = false;

	std::string File = Folders::ROOT_FOLDER + Folders::DIALOGUE_FOLDER + Folders::MAIN_BATTLESCENEFOLDER + _SceneName + ".json";
	std::ifstream file(File);
	nlohmann::json j = nlohmann::json::parse(file);

	for (auto scene : j["parts"])
	{
		BattleScene* NewScene = new BattleScene();

		Character* SpeakerCopy = UnitManager::GetInstance()->GetCharacter(scene["speaker"].get<std::string>());

		NewScene->Name = SpeakerCopy->GetNAME();
		NewScene->ActorSprite = std::make_shared<sf::Sprite>(sf::Sprite(*SpeakerCopy->SupportSprite->Normal));
		NewScene->ActorSprite->setPosition(5, 54);
		NewScene->Text = TextSanitizer::SanitizeString(scene["text"].get<std::string>());

		Scenes.push(NewScene);
	}

	SceneTimer = 0;
	HasScene = true;
	PreviousTextLength = 0;
}

