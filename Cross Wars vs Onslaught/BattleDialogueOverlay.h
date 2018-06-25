#pragma once
#include "GameScreenOverlay.h"
#include <queue>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

struct BattleScene
{
	std::shared_ptr<sf::Sprite> ActorSprite;
	std::string Name;
	std::string Text;
};

class BattleDialogueOverlay : public GameScreenOverlay
{
public:
	const int TEXTSPEED = 50;

	bool SceneEnded;
	bool HasScene;

	double SceneTimer = 0;
	int PreviousTextLength = 0;

	BattleDialogueOverlay(sf::RenderWindow* _pRenderWindow);
	~BattleDialogueOverlay();

	void Draw();
	void Update();
	void Unload();
	void Hide();
	void PollInput();
	void Load();

	void CheckScene();
	void NextScene();
	void Popscene();
	void LoadDialogue(std::string _SceneName);

	std::string CurrentText;

	sf::Sprite* DialogueBox;

	sf::Text* TextRenderer;
	sf::Text* NameRenderer;

	BattleScene* GetCurrentScene() { return Scenes.front(); }
private:
	std::queue<BattleScene*> Scenes;
};

