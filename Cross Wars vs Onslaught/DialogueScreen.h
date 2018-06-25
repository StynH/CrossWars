#pragma once
#include "GameScreen.h"
#include "GameTime.h"
#include "Character.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "UnitManager.h"
#include "TextContainer.h"
#include <queue>
#include <SFML/Audio/Music.hpp>
#include "Camera.h"

struct MiniActor {
	std::string Name;
	std::shared_ptr<sf::Sprite> Sprite;
};

struct Scene {
	std::string Text;
	std::string Name;
	std::shared_ptr<MiniActor> Actor;
	std::shared_ptr<MiniActor> CounterActor;
	std::shared_ptr<sf::Music> MusicChange;
	bool StopMusic;
};

class DialogueScreen : public GameScreen
{
public:
	const static int TEXTSPEED = 50;

	DialogueScreen(sf::RenderWindow* _pRenderWindow);
	~DialogueScreen();

	void Draw();
	void Update();
	void Unload() {};
	void PollInput();
	void CheckScene();
	void NextScene();
	void Popscene();
	void LoadDialogue(std::string _SceneName);

	sf::Sprite* DialogueBox;
	sf::Sprite* Overlay;
	sf::Sprite* Gradient;
	sf::Sprite* Background;
	sf::Sprite* LowerFiller;

	sf::Font* DialogueFont;
	sf::Text* TextRenderer;
	sf::Text* NameRenderer;

	TextContainer* UpperScreen;
	TextContainer* LowerScreen;

	Scene* GetCurrentScene() { return Scenes.front(); }
private:
	std::queue<Scene*> Scenes;

	Camera* pCamera;

	std::string CurrentText;

	bool SceneEnded;

	double SceneTimer = 0;
	int PreviousTextLength = 0;
};
