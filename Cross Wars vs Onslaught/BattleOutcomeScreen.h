#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "GameScreenOverlay.h"
#include "BattleOutcome.h"
#include "Event.h"

class LevelScreen;

class BattleOutcomeScreen : public GameScreenOverlay
{
public:
	BattleOutcomeScreen(sf::RenderWindow* _pWindow, LevelScreen* _pLevelScreen);
	~BattleOutcomeScreen();

	void Draw();
	void Update();
	void SetNewOutcome(BattleOutcome* pBattleOutcome);
	void Unload();
	void Hide();
	void PollInput();
	void Load();

private:
	BattleOutcome* pBattleOutcome;

	sf::Sprite* pOverlaySprite;

	sf::Text* AttackerWeapon;
	sf::Text* DefenderWeapon;

	sf::Text* AttackerHitChanceText;
	sf::Text* DefenderHitChanceText;

	sf::Text* AttackerCurrentHP;
	sf::Text* DefenderCurrentHP;

	sf::Text* AttackerCritChance;
	sf::Text* DefenderCritChance;

	sf::Text* AttackerHPPrediction;
	sf::Text* DefenderHPPrediction;

	std::shared_ptr<sf::Sprite> pAttackerWeaponSprite;
	std::shared_ptr<sf::Sprite> pDefenderWeaponSprite;

	Event<BattleOutcome*> OnEngage;
};

