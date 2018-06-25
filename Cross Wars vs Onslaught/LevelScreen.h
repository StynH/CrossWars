#pragma once 
#include "GameScreen.h"
#include "Cursor.h"
#include "TextureManager.h"
#include "Map.h"
#include "Camera.h"
#include "BattleCharScreen.h"
#include "FloodFill.h"
#include "UnitMenuOverlay.h"
#include "TurnIndicator.h"
#include "BattleOutcomeScreen.h"
#include "AI.h"
#include "BattleTransition.h"
#include "MindBend.h"
#include "ItemTradeOverlay.h"
#include "DialogueOverlay.h"

class LevelScreen : public GameScreen
{
public:
	enum STATE
	{
		INTRO,
		PLAYING,
		SWITCHTURNS,
		VICTORY
	};

	enum PLAYERACTION
	{
		IDLE,
		MOVINGUNIT,
		INOVERLAY,
		ATTACKING,
		TRADING
	};

	enum TURN
	{
		PLAYER,
		ENEMY,
		PARTNER
	};

	LevelScreen(sf::RenderWindow* _pRenderWindow);
	~LevelScreen();

	Cursor* pCursor;
	Cursor* pEnemyCursor;
	Cursor* pPartnerCursor;

	sf::Texture* GetSnapshot();

	float TransitionTimer;

	void Start() override;
	void SetArenaMode();
	void CullMap();
	void UpdateUnits();
	void DrawUnits();
	void DrawVisualiser();
	void DrawMovementTiles();
	void DrawAttackTiles();
	void Attack(BattleOutcome* _pBattle);
	void Draw();
	void UpdateOverlay();
	void Update();

	bool HasUnitsLeft();
	void ReactivateUnits();
	void PrepareAttack();

	void EndTurn();
	void EndBattle(Unit* _pUnit);

	void Unload();

	MovementTile* CanMoveToTile(sf::Vector2f _Position);
	bool UnitInRange(sf::Vector2f _Position);
	void CursorDeselectUnit();
	void PollInput();

	void SetPlayerAction(PLAYERACTION _Action) { CurrentPlayerAction = _Action; };

	Map* GetCurrentMap() { return pCurrentMap; };
	void SetNewMap(Map* _pNewMap);
	void CenterCameraAndCursorOnUnit(Unit* _pUnit);
	void CenterCameraOnCursor(Cursor* _pCursor);

	Camera* GetCamera() { return pCamera; };

	bool CanIndent(int _x, int _y);
	bool IsAtEdge(sf::Keyboard::Key _dir);

	sf::Vector2i ScreenOffset;

	void ClearUnitOverviewScreen();
	void SetUnitOverviewScreen(Unit* _pNewUnit);
	void ShowTradeScreen();
	void ShowInventoryScreen(Unit* _pUnit);

	TurnIndicator* pPlayerTurn;
	TurnIndicator* pEnemyTurn;
	TurnIndicator* pPartnerTurn;
private:
	void DisableUnit();

	Map* pCurrentMap;
	Camera* pCamera;
	Camera* pNeutralCamera;

	BattleCharScreen* pUnitOverviewScreen;
	UnitMenuOverlay* pUnitMenuScreen;
	ItemTradeOverlay* pItemTradeScreen;
	DialogueOverlay* pDialogueScreen;

	BattleOutcomeScreen* pBattleOutcomeScreen;

	GameScreenOverlay* pOverlayScreen;

	TURN CurrentTurn;
	STATE CurrentState;
	PLAYERACTION CurrentPlayerAction;

	sf::Sprite* pMoveVisualiser;
	sf::Sprite* pAttackVisualiser;

	sf::Sprite* BossSprite;

	sf::Sprite* pVictoryIndicator;
	float VictoryCounter;
	bool VictoryWait;
	bool DelayUnitMenu;
	float BlackAlpha;

	sf::Sprite* pBlackScreen;

	BattleTransition* pBattleTransitionShader;
	bool TransitioningToBattle;

	std::vector<MovementTile*> AvailableMovementTiles;
	std::vector<MovementTile*> AvailableAttackableTiles;
	std::vector<MovementTile*> AvailableTradeTiles;

	sf::RenderTexture* pRenderTexture;

	std::shared_ptr<AI> EnemyAI;
	std::shared_ptr<AI> PartnerAI;

	bool InIntro;
	bool VictoryAchieved;
	bool GameOver;
	bool ArenaMode;
};
