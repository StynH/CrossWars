#pragma once
#include "GameScreenOverlay.h"
#include "TextContainer.h"
#include "DialogueScreen.h"

class DialogueOverlay : public GameScreenOverlay
{
public:
	DialogueOverlay(sf::RenderWindow* _pRenderWindow);
	~DialogueOverlay();

	void Draw();
	void Reset();
	void Update();
	void Load() override;
	void Unload();
	void Hide();
	void PollInput();
	void NextScene();
	void Popscene();
	void CheckScene();

	void LoadDialogue(std::string _SceneName);
private:
	Scene* GetCurrentScene() { return Scenes.front(); }

	sf::Sprite* DialogueBox;
	sf::Sprite* Overlay;
	sf::Sprite* Gradient;
	sf::Sprite* LowerFiller;

	sf::Font* DialogueFont;
	sf::Text* TextRenderer;
	sf::Text* NameRenderer;

	TextContainer* UpperScreen;

	std::queue<Scene*> Scenes;

	std::string CurrentText;

	bool SceneEnded;

	double SceneTimer = 0;
	int PreviousTextLength = 0;
};

