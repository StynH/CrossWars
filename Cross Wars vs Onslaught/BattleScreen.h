#pragma once
#include "GameScreen.h"
#include "Character.h"
#include "Unit.h"
#include "TextureManager.h"
#include <iostream>
#include "ResourceManager.h"
#include "Camera.h"
#include "BattleOutcome.h"
#include "BattleHelper.h"
#include "BattleDialogueOverlay.h"
#include "ShockWave.h"
#include "MindBend.h"
#include "BattleTransition.h"

class BattleScreen: public GameScreen
{
public:
	const int SHAKEMAGNITUDE = 16;
	const int MOVEMENTSPEED = 1000;

	enum BATTLEPHASE
	{
		INIT,
		ATTACK,
		COUNTERATTACK,
		DEAD,
		POSTBATTLE,
		EXPSCREEN,
		LEVELUP,
		LEVELUPUNIT,
		END
	};

	BattleScreen(sf::RenderWindow* _pRenderWindow);
	~BattleScreen();

	void Draw();
	void Update();
	void Unload();
	void InitBattle(BattleOutcome* _pBattle, std::string _scene, bool _ArenaMode);
	void LevelUp(Unit* _pUnit);
	void ResetGains();
	void PollInput();

	sf::Texture* GetSnapshot();

	Stat UnitATK;
	Stat UnitDEF;
	Stat UnitSPD;
	Stat UnitLCK;
	Stat UnitHP;
	sf::Sprite* Vignette;
	std::shared_ptr<sf::Sprite> UnitLevelUpPortrait;

	int UnitLevelsGained;
	int SupportLevelsGained;

	int ATKGained;
	int DEFGained;
	int SPDGained;
	int LCKGained;
	int HPGained;

	double Timer;

	float MovementSpeed;
	float Shaker;
	float ColorTimer;
	float AlphaTimer;

	float UnitEXPOld;
	float UnitSupportEXPOld;
	float UnitEXPGain;
	float UnitSupportEXPGain;

	bool UnitGotEXP;
	bool UnitSupportGotEXP;

	Unit* UnitEXPRecieved;
	Unit* SupportEXPRecieved;

	bool ArenaMode;

	bool UnitExpGainDone;
	bool SupportExpGainDone;

	bool AttackerMiss;
	bool AttackerCrit;
	bool AttackerDied;

	bool DefenderMiss;
	bool DefenderCrit;
	bool DefenderDied;

	bool MustMindwave;
	bool TransitioningBack;

	sf::Sprite* HPOverlay;
	sf::Sprite* EXPGained;
	sf::Sprite* LevelUpOverlay;
	sf::Sprite* SkillIncrease;

	sf::Text* AttackerHP;
	sf::Text* DefenderHP;

	sf::Sprite* Snapshot;

	sf::Text* UnitExpCounter;
	sf::Text* UnitSupportExpCounter;
	sf::Text* UnitExpGainCounter;
	sf::Text* UnitSupporGainExpCounter;

	sf::Text* OldLevel;
	sf::Text* NewLevel;

	sf::Sprite* EXPScreenContainer;
	sf::Sprite* EXPScreenContainerTwo;
	sf::Sprite* UnitEXPPortrait;
	sf::Sprite* UnitSupportEXPPortrait;

	sf::Sprite* CritSprite;
	sf::Sprite* MissSprite;

	sf::Sprite* AttackerBattleSprite;
	sf::Sprite* DefenderBattleSprite;

	sf::Sprite* AttackerAttackSprite;
	sf::Sprite* DefenderAttackSprite;

	sf::Sprite* AttackerSupport;
	sf::Sprite* DefenderSupport;

	sf::Sprite* Scene;
	sf::Sprite* Background;
	sf::Sprite* Platform;
	sf::Sprite *SupportBlue, *SupportRed;

	sf::Color HitColor;

	sf::Vector2f DefenderOriginalPos;
	sf::Vector2f AttackerOriginalPos;

	BattleOutcome* pBattleOutcome;
	BattleDialogueOverlay* pDialogueOverlay;

	Camera* pCamera;
	BATTLEPHASE CurrentPhase;

	ShockWave* ShockWaveShaderDefender;
	ShockWave* ShockWaveShaderAttacker;
	MindBend* MindBendShader;
	BattleTransition* BattleTransitionShader;
};

