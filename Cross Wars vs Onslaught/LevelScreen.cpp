#include "LevelScreen.h"
#include "Camera.h"
#include "ScreenManager.h"
#include "MathHelper.h"
#include <mutex>
#include "MapManager.h"
#include "PlayerManager.h"
#include "FloodFill.h"
#include "MapHelper.h"
#include "AStar.h"
#include "MusicPlayer.h"
#include "StateMachine.h"

LevelScreen::LevelScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pUnitOverviewScreen = new BattleCharScreen(_pRenderWindow);
	
	pCursor = new Cursor(new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons\\selector.png", Folders::FolderType::BATTLE), sf::IntRect(0, 0, 32, 32)), Cursor::OWNER::PLAYER);
	pEnemyCursor = new Cursor(new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons\\selector.png", Folders::FolderType::BATTLE), sf::IntRect(32, 0, 32, 32)), Cursor::OWNER::AI);
	pPartnerCursor = new Cursor(new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons\\selector.png", Folders::FolderType::BATTLE), sf::IntRect(64, 0, 32, 32)), Cursor::OWNER::AI);

	pUnitMenuScreen = new UnitMenuOverlay(_pRenderWindow, pCursor, this);
	pItemTradeScreen = new ItemTradeOverlay(_pRenderWindow);
	pBattleOutcomeScreen = new BattleOutcomeScreen(_pRenderWindow, this);
	pDialogueScreen = new DialogueOverlay(_pRenderWindow);

	ScreenOffset = sf::Vector2i(0, 0);

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 256));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 0.5f));
	pNeutralCamera = new Camera(sf::FloatRect(0, 0, 512, 256));
	pNeutralCamera->setViewport(sf::FloatRect(0, 0, 1, 0.5f));

	pRenderTexture = new sf::RenderTexture();
	pRenderTexture->create(512, 512);

	sf::Texture* VisualiserSprite = TextureManager::GetInstance()->LoadTexture("visualiser.png", Folders::FolderType::LEVELS);

	pMoveVisualiser = new sf::Sprite(*VisualiserSprite, sf::IntRect(0, 0, Tile::TILESIZE, Tile::TILESIZE));
	pAttackVisualiser = new sf::Sprite(*VisualiserSprite, sf::IntRect(Tile::TILESIZE, 0, Tile::TILESIZE, Tile::TILESIZE));

	pVictoryIndicator = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("victory.png", Folders::FolderType::LEVELS));
	VictoryCounter = 0.0f;
	pBlackScreen = new sf::Sprite(*ResourceManager::GetInstance()->GetBlackBackground());

	pOverlayScreen = nullptr;
	pCurrentMap = nullptr;

	EnemyAI = std::make_shared<AI>(Unit::ENTITY_ALLIANCE::ENEMY, pEnemyCursor);
	PartnerAI = std::make_shared<AI>(Unit::ENTITY_ALLIANCE::PARTNER, pPartnerCursor);

	CurrentTurn = PLAYER;
	CurrentState = PLAYING;
	CurrentPlayerAction = IDLE;

	pBattleTransitionShader = new BattleTransition(false, sf::Vector2f(512, 256));
	pBattleTransitionShader->SetResolution(sf::Vector2f(512, 512));
	TransitioningToBattle = false;
	TransitionTimer = 0.0;

	sf::Texture* TurnTexture = TextureManager::GetInstance()->LoadTexture("turns.png", Folders::FolderType::HUD);
	pPlayerTurn = new TurnIndicator(_pRenderWindow, new sf::Sprite(*TurnTexture, sf::IntRect(0, 0, 463, 165)), 0);
	pEnemyTurn = new TurnIndicator(_pRenderWindow, new sf::Sprite(*TurnTexture, sf::IntRect(0, 165, 463, 165)), 0);
	pPartnerTurn = new TurnIndicator(_pRenderWindow, new sf::Sprite(*TurnTexture, sf::IntRect(0, 330, 463, 165)), 0);

	BossSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons\\boss.png", Folders::FolderType::BATTLE));

	InIntro = false;
	VictoryAchieved = false;
	GameOver = false;
	ArenaMode = false;
	DelayUnitMenu = false;

	MustUnload = true;
}

LevelScreen::~LevelScreen()
{
	SafeDelete(pCamera);
	SafeDelete(pRenderTexture);
	SafeDelete(pCursor);
}

void LevelScreen::Start()
{
	CurrentTurn = PLAYER;
	CurrentState = INTRO;
	CurrentPlayerAction = IDLE;

	InIntro = true;
	GameOver = false;
	VictoryWait = true;

	VictoryCounter = 0.0f;
	TransitionTimer = 0.0;
	BlackAlpha = 255.0f;

	pOverlayScreen = nullptr;

	pBlackScreen->setColor(sf::Color(255, 255, 255, 255));

	SoundPlayer::GetInstance()->PlaySpecificMusic(SoundPlayer::GetInstance()->LevelMusic.get());
}

void LevelScreen::SetArenaMode()
{
	ArenaMode = true;
}

void LevelScreen::SetNewMap(Map* _pNewMap)
{
	pCurrentMap = _pNewMap;

	if(!ArenaMode)
	{
		pCurrentMap->PlayerUnits = PlayerManager::GetInstance()->GetReadyUnits();
		for (Unit* PlayersUnit : pCurrentMap->PlayerUnits)
		{
			sf::Vector2f DataSpawnPos = pCurrentMap->GetFreeSpawnzone()->Position;
			sf::Vector2f SpawnPos = sf::Vector2f(DataSpawnPos.x * Tile::TILESIZE, DataSpawnPos.y * Tile::TILESIZE);
			PlayersUnit->InitPosition(SpawnPos, DataSpawnPos);
			PlayersUnit->ReactivateUnit();
		}
	}
	else
	{
		pCurrentMap->PlayerUnits = PlayerManager::GetInstance()->GetArenaUnits();
		for (Unit* PlayersUnit : pCurrentMap->PlayerUnits)
		{
			sf::Vector2f DataSpawnPos = pCurrentMap->GetFreeSpawnzone()->Position;
			sf::Vector2f SpawnPos = sf::Vector2f(DataSpawnPos.x * Tile::TILESIZE, DataSpawnPos.y * Tile::TILESIZE);
			PlayersUnit->InitPosition(SpawnPos, DataSpawnPos);
			PlayersUnit->ReactivateUnit();
		}
	}
	
	CenterCameraAndCursorOnUnit(pCurrentMap->PlayerUnits.at(0));
	pCursor->CheckUnderCursor();

	EnemyAI->SetMap(_pNewMap);
	PartnerAI->SetMap(_pNewMap);

	AStar::InsertMap(_pNewMap);

	SoundPlayer::GetInstance()->SetLevelMusic(_pNewMap->LevelMusic, true);
	SoundPlayer::GetInstance()->SetBattleMusic(_pNewMap->BattleMusic, true);
	SoundPlayer::GetInstance()->SetBossMusic(_pNewMap->BossMusic, true);
}

void LevelScreen::Unload()
{
	//Delete the map.
	SafeDelete(pCurrentMap);

	//Reset the victory bool.
	VictoryAchieved = false;

	//If we're in the arena, remove any unit still in the arena squad.
	if (ArenaMode)
	{
		//Unset every unit.
		for(auto* pUnit : PlayerManager::GetInstance()->GetArenaUnits())
		{
			PlayerManager::GetInstance()->SwitchUnitFromArena(pUnit);
		}

		//and turn off the arena mode.
		ArenaMode = false;
	}
}

void LevelScreen::CenterCameraAndCursorOnUnit(Unit* _pUnit)
{
	pCursor->setPosition(_pUnit->getPosition(), _pUnit->getDataPosition());
	CenterCameraOnCursor(pCursor);
}

void LevelScreen::CenterCameraOnCursor(Cursor* _pCursor)
{
	int PosX = _pCursor->getPosition().x - (Camera::SCREEN_WIDTH / 2);
	int PosY = _pCursor->getPosition().y - (Camera::SCREEN_HEIGHT / 4);

	if (PosX + (Camera::SCREEN_WIDTH / 2) < _pCursor->getPosition().x)
	{
		PosX += _pCursor->getPosition().x - PosX;
	}

	//If the camera's view to the left is less then the map, readjust.
	if (PosX < 0)
	{
		PosX = 0;
	}

	//If the camera's  view to the top position is less then the map, readjust.
	if (PosY < 0)
	{
		PosY = 0;
	}

	//If the camera's view to the right position is less then the map, readjust.
	if (PosY + (Camera::SCREEN_HEIGHT / 2) > pCurrentMap->GetDimensions().y)
	{
		PosY -= (PosY + (Camera::SCREEN_HEIGHT / 2)) - pCurrentMap->GetDimensions().y;
	}

	//If the camera's view to the bottom position is less then the map, readjust.
	if (PosX + (Camera::SCREEN_WIDTH) > pCurrentMap->GetDimensions().x)
	{
		PosX -= (PosX + (Camera::SCREEN_WIDTH)) - pCurrentMap->GetDimensions().x;
	}

	pCamera->setCenter(PosX + (Camera::SCREEN_WIDTH) / 2, PosY + (Camera::SCREEN_HEIGHT) / 4);
	ScreenOffset.x = PosX;
	ScreenOffset.y = PosY;
}

void LevelScreen::Update()
{
	//First we check if we didnt lose all of our units.
	if (GameOver)
	{
		ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetGameoverScreen());
	}

	//Dont update anything when transitioning.
	if (TransitioningToBattle)
	{
		pBattleTransitionShader->Update();
		TransitionTimer += GameTime::DeltaTime;
		if(TransitionTimer >= 1.75)
		{
			ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetBattleScreen());
		}
	}
	//Alright, no transition.
	else
	{
		//We check if the objective is achieved yet.
		pCurrentMap->Objective->CheckObjective();

		//Did we achieve the objective?
		if (pCurrentMap->Objective->IsObjectiveAchieved() && !VictoryAchieved)
		{
			CurrentState = VICTORY;
			VictoryAchieved = true;
		}
		//If we achieved victory, we wait for a few seconds and then fade out.
		else if (VictoryAchieved)
		{
			if(VictoryWait)
			{
				if(VictoryCounter >= 3.0)
				{
					VictoryWait = false;
					SoundPlayer::GetInstance()->FadeOutMusic(0.4);
					VictoryCounter = 0.0f;
				}
				VictoryCounter += GameTime::DeltaTime;
			}
			else
			{
				if(BlackAlpha >= 255)
				{
					pBlackScreen->setColor(sf::Color(255, 255, 255, 255));
					if(VictoryCounter >= 1.5)
					{
						pNeutralCamera->DrawUpdate();
						PlayerManager::GetInstance()->HealPlayerUnits();
						ScreenManager::GetInstance()->CloseScreen();
						StateMachine::GetInstance()->NextState();
					}
					VictoryCounter += GameTime::DeltaTime;
				}
				else
				{
					BlackAlpha += GameTime::DeltaTime * 100;
					pBlackScreen->setColor(sf::Color(255, 255, 255, BlackAlpha));
				}		
			}
		}

		//Firstly we update the lower screen.
		pUnitOverviewScreen->Update();

		//Check the gamestate
		if (CurrentState == PLAYING)
		{
			//Update all units.
			UpdateUnits();

			//If there is an overlay, update that instead of the normal loop.
			if (pOverlayScreen && CurrentPlayerAction == INOVERLAY && !pOverlayScreen->IsHiding)
			{
				UpdateOverlay();
			}
			//Otherwise we'll update the current player.
			//That means either the player, or one of the two AI's.
			else
			{
				switch (CurrentTurn)
				{
				case PLAYER:
					pCursor->Update();
					//Oh no I'm all out of active units, that means the end of me.
					if (!HasUnitsLeft())
					{
						EndTurn();
					}
					break;
				case ENEMY:
					pEnemyCursor->Update();
					CenterCameraOnCursor(pEnemyCursor);
					EnemyAI->Think();
					if (EnemyAI->IsDone())
					{
						EndTurn();
					}
					break;
				case PARTNER:
					pPartnerCursor->Update();
					CenterCameraOnCursor(pPartnerCursor);
					PartnerAI->Think();
					if (PartnerAI->IsDone())
					{
						EndTurn();
					}
					break;
				}
			}
		}
		//If we're switching, or done switching turns.
		else if (CurrentState == SWITCHTURNS)
		{
			if (pPlayerTurn->IsRunning)
			{
				pPlayerTurn->Update();
			}
			else if (pEnemyTurn->IsRunning)
			{
				pEnemyTurn->Update();
			}
			else if (pPartnerTurn->IsRunning)
			{
				pPartnerTurn->Update();
			}
			else
			{
				CurrentState = PLAYING;
				if(CurrentTurn == PLAYER)
				{
					if (HasUnitsLeft())
					{
						CenterCameraAndCursorOnUnit(pCurrentMap->PlayerUnits.at(0));
					}
				}
			}
		}
		//Oh we're in the intro, do that.
		else if (CurrentState == INTRO)
		{
			if(BlackAlpha > 0)
			{
				BlackAlpha -= GameTime::DeltaTime * 100;
				pBlackScreen->setColor(sf::Color(255, 255, 255, BlackAlpha));
				if(BlackAlpha <= 0)
				{
					BlackAlpha = 0;
					pBlackScreen->setColor(sf::Color(255, 255, 255, BlackAlpha));
					CurrentState = PLAYING;
				}
			}
		}
	}
}

void LevelScreen::UpdateOverlay()
{
	//We check if the game needs to be closed.
	if(pOverlayScreen->CloseGame)
	{
		CloseGame = true;
	}
	//Nope, does the overlay need to be closed?
	else if (pOverlayScreen->CloseScreen)
	{
		pOverlayScreen->Unload();

		if(pOverlayScreen->DeselectUnitAfterClose)
		{
			CursorDeselectUnit();
		}

		pOverlayScreen->IsHiding = false;
		pOverlayScreen->CloseScreen = false;

		pOverlayScreen = nullptr;
		if(DelayUnitMenu)
		{
			pUnitMenuScreen->Load();
			pOverlayScreen = pUnitMenuScreen;
			DelayUnitMenu = false;
		}
		else
		{
			CurrentPlayerAction = IDLE;
		}
	}
	//Alright, we'll update it,
	//If it's not hiding.
	else if(!pOverlayScreen->IsHiding)
	{
		pOverlayScreen->Update();
	}
}

void LevelScreen::Draw()
{
	//Clear our render texture.
	pRenderTexture->clear();

	//Set the top camera.
	pCamera->DrawUpdate();
	pRenderTexture->setView(pWindow->getView());

	//We don't render map parts that we don't see.
	CullMap();

	//Draw the map.
	pRenderTexture->draw(*pCurrentMap->GetSprite());

	//If the player is moving or attacking with a unit.
	if (CurrentPlayerAction == MOVINGUNIT)
	{
		DrawMovementTiles();
	}
	else if(CurrentPlayerAction == ATTACKING)
	{
		DrawAttackTiles();
	}

	//Draw all units.
	DrawUnits();
	
	//Draw the cursor.
	switch (CurrentTurn)
	{
	case PLAYER: 
		pRenderTexture->draw(*pCursor->getSprite());
		break;
	case ENEMY: 
		pRenderTexture->draw(*pEnemyCursor->getSprite());
		break;
	case PARTNER: 
		pRenderTexture->draw(*pPartnerCursor->getSprite());
		break;
	}

	//Draw the lower screen.
	pUnitOverviewScreen->Draw();

	//We want any overlay to display on the top screen.
	//So we switch to a camera that does not change position ever.
	pNeutralCamera->DrawUpdate();

	//Alright, we've drawn everything that matters onto the Render Texture.
	//Let's make a sprite out of it so we can draw it.
	pRenderTexture->display();
	sf::Sprite TotalSprite(pRenderTexture->getTexture());

	if(TransitioningToBattle)
	{
		pWindow->draw(TotalSprite, pBattleTransitionShader->GetShader());
	}
	else
	{
		pWindow->draw(TotalSprite);
	}

	//If we actually have an active overlay, draw it.
	if(pOverlayScreen != nullptr && !pOverlayScreen->IsHiding)
	{
		pOverlayScreen->Draw();
	}

	//If we're switching turns, draw those graphics.
	if (CurrentState == SWITCHTURNS)
	{
		switch(CurrentTurn)
		{
		case PLAYER: 
			pPlayerTurn->Draw();
			break;
		case ENEMY: 
			pEnemyTurn->Draw();
			break;
		case PARTNER: 
			pPartnerTurn->Draw();
			break;
		}
	}

	//And if we have completed our mission, draw the victory screen!
	if(VictoryAchieved)
	{
		pWindow->draw(*pVictoryIndicator);
	}

	//Both the intro and the victory use a black fade.
	if(InIntro || VictoryAchieved)
	{
		pWindow->draw(*pBlackScreen);
	}
}

sf::Texture* LevelScreen::GetSnapshot()
{
	pRenderTexture->clear();

	pCamera->DrawUpdate();
	pRenderTexture->setView(pWindow->getView());

	CullMap();

	pRenderTexture->draw(*pCurrentMap->GetSprite());

	DrawUnits();

	pRenderTexture->display();
	
	return new sf::Texture(pRenderTexture->getTexture());
}

void LevelScreen::EndTurn()
{
	if(CurrentState == PLAYING)
	{
		switch (CurrentTurn)
		{
		case PLAYER:
			CurrentTurn = ENEMY;
			EnemyAI->Acivate();
			pEnemyTurn->Start();
			break;
		case ENEMY:
			//If there's actually a partner, switch to him.
			if (pCurrentMap->PartnerUnits.size() > 0)
			{
				CurrentTurn = PARTNER;
				PartnerAI->Acivate();
				pPartnerTurn->Start();
			}
			//Otherwise we're going back to the player.
			else
			{
				CurrentTurn = PLAYER;
				pPlayerTurn->Start();
			}
			break;
		case PARTNER:
			CurrentTurn = PLAYER;
			pPlayerTurn->Start();
			break;
		default:
			//How did you get here...?
			break;
		}

		CurrentState = SWITCHTURNS;
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->TurnTransition, false);
		ReactivateUnits();
	}
}

bool LevelScreen::HasUnitsLeft()
{
	switch(CurrentTurn)
	{
	case PLAYER:
		for (auto& CurrentUnit : pCurrentMap->PlayerUnits)
		{
			if (CurrentUnit->IsActive)
			{
				return true;
			}
		}
		break;
	case ENEMY: 
		for (auto& CurrentUnit : pCurrentMap->EnemyUnits)
		{
			if (CurrentUnit->IsActive)
			{
				return true;
			}
		}
		break;
	case PARTNER: 
		for (auto& CurrentUnit : pCurrentMap->PartnerUnits)
		{
			if(CurrentUnit->IsActive)
			{
				return true;
			}
		}
		break;
	}
	return false;
}

void LevelScreen::ReactivateUnits()
{
	for (auto& CurrentUnit : pCurrentMap->PlayerUnits)
	{
		CurrentUnit->ReactivateUnit();
	}

	for (auto& CurrentUnit : pCurrentMap->PartnerUnits)
	{
		CurrentUnit->ReactivateUnit();
	}

	for (auto& CurrentUnit : pCurrentMap->EnemyUnits)
	{
		CurrentUnit->ReactivateUnit();
	}
}

//Don't draw what you dont see aye?
//Only for the map that is...
void LevelScreen::CullMap()
{
	sf::Vector2f viewCenter = GetCamera()->getCenter();
	sf::Vector2f halfExtents = GetCamera()->getSize() / 2.0f;
	sf::Vector2f translation = viewCenter - halfExtents;

	int mX = static_cast<int>(translation.x);
	int mY = static_cast<int>(translation.y);

	pCurrentMap->GetSprite()->setTextureRect(sf::IntRect(mX, mY, 512, 256));
	pCurrentMap->GetSprite()->setPosition(mX, mY);
}

void LevelScreen::UpdateUnits()
{
	for (std::vector<Unit*>::size_type i = 0; i < pCurrentMap->PlayerUnits.size();)
	{
		Unit* pUnit = pCurrentMap->PlayerUnits.at(i);
		if (!pUnit->IsDead)
		{
			pUnit->Update();
			++i;
		}
		else
		{
			pCurrentMap->PlayerUnits.erase(pCurrentMap->PlayerUnits.begin() + i);
		}
	}

	for (std::vector<Unit*>::size_type i = 0; i < pCurrentMap->EnemyUnits.size();)
	{
		Unit* pUnit = pCurrentMap->EnemyUnits.at(i);
		if (!pUnit->IsDead)
		{
			pUnit->Update();
			++i;
		}
		else
		{
			pCurrentMap->EnemyUnits.erase(pCurrentMap->EnemyUnits.begin() + i);
		}
	}

	for (std::vector<Unit*>::size_type i = 0; i < pCurrentMap->PartnerUnits.size();)
	{
		Unit* pUnit = pCurrentMap->PartnerUnits.at(i);
		if (!pUnit->IsDead)
		{
			pUnit->Update();
			++i;
		}
		else
		{
			pCurrentMap->PartnerUnits.erase(pCurrentMap->PartnerUnits.begin() + i);
		}
	}
}

void LevelScreen::DrawUnits()
{
	sf::Vector2i MapSize = pCurrentMap->GetDimensions();
	sf::Vector2i MapLocation = pCurrentMap->GetSpritePosition();

	for (auto& CurrentUnit : pCurrentMap->PlayerUnits)
	{
		//Dont draw the sucker if he isn't in view.
		if(CurrentUnit->IsUnitInView(MapLocation, MapSize))
		{
			pRenderTexture->draw(*CurrentUnit->GetUnitSprite());
			pRenderTexture->draw(*CurrentUnit->GetHealthBarBackground());
			pRenderTexture->draw(*CurrentUnit->GetHealthBarValue());
		}

	}

	for (auto& CurrentUnit : pCurrentMap->PartnerUnits)
	{
		if (CurrentUnit->IsUnitInView(MapLocation, MapSize))
		{
			pRenderTexture->draw(*CurrentUnit->GetUnitSprite());
			pRenderTexture->draw(*CurrentUnit->GetHealthBarBackground());
			pRenderTexture->draw(*CurrentUnit->GetHealthBarValue());
		}
	}

	for (auto& CurrentUnit : pCurrentMap->EnemyUnits)
	{
		if (CurrentUnit->IsUnitInView(MapLocation, MapSize))
		{
			pRenderTexture->draw(*CurrentUnit->GetUnitSprite());

			if(CurrentUnit->IsBoss)
			{
				BossSprite->setPosition(CurrentUnit->getPosition());
				pRenderTexture->draw(*BossSprite);
			}

			pRenderTexture->draw(*CurrentUnit->GetHealthBarBackground());
			pRenderTexture->draw(*CurrentUnit->GetHealthBarValue());
		}
	
	}
}

void LevelScreen::DrawVisualiser()
{
	//The max tiles the unit can move.
	int MoveDist = pCursor->GetSelectedUnit()->getMOV();

	AvailableMovementTiles.clear();
	std::vector<MovementTile*> AvailableTiles = MapHelper::GetTilesToDistance(pCurrentMap, pCursor->GetSelectedUnit()->getDataPosition(), MoveDist, MapHelper::GetEnemyPositions(Unit::PLAYER, pCurrentMap));

	for(MovementTile* Tile : AvailableTiles)
	{
		if(!Tile->HasEnemy)
		{
			AvailableMovementTiles.push_back(Tile);
		}
	}
}

void LevelScreen::PrepareAttack()
{
	//The furthest a unit can attack.
	int MaxDist = 0;

	if(pCursor->GetSelectedUnit()->getInventory().size() > 0)
	{
		for (auto item : pCursor->GetSelectedUnit()->getInventory())
		{
			if (item->getType() == Item::ITEMTYPE::WEAPON)
			{
				Weapon* weapon = static_cast<Weapon*>(item);
				if (weapon->getRNG() > MaxDist)
				{
					MaxDist = weapon->getRNG();
				}
			}
		}
	}

	AvailableAttackableTiles = MapHelper::GetTilesToDistance(pCurrentMap, pCursor->GetSelectedUnit()->getDataPosition(), MaxDist, std::vector<sf::Vector2f>());

	CurrentPlayerAction = ATTACKING;
}

void LevelScreen::DrawMovementTiles()
{
	for(MovementTile* AvailableTile : AvailableMovementTiles)
	{
		pMoveVisualiser->setPosition(AvailableTile->x * Tile::TILESIZE, AvailableTile->y * Tile::TILESIZE);
		pRenderTexture->draw(*pMoveVisualiser);
	}
}

void LevelScreen::DrawAttackTiles()
{
	for (MovementTile* AvailableTile : AvailableAttackableTiles)
	{
		pAttackVisualiser->setPosition(AvailableTile->x * Tile::TILESIZE, AvailableTile->y * Tile::TILESIZE);
		pRenderTexture->draw(*pAttackVisualiser);
	}
}

void LevelScreen::Attack(BattleOutcome* _pBattle)
{
	//Time to go in for the kill.
	//We initialize the battle.
	ScreenManager::GetInstance()->GetBattleScreen()->InitBattle(_pBattle, pCurrentMap->Style, ArenaMode);

	//No overlays.
	pOverlayScreen = nullptr;

	//Start the transition!
	TransitioningToBattle = true;
	pBattleTransitionShader->SetSecondTexture(ScreenManager::GetInstance()->GetBattleScreen()->GetSnapshot());
	pBattleTransitionShader->Reset();

	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->BattleTransition, false);
	SoundPlayer::GetInstance()->Transition(SoundPlayer::GetInstance()->BattleMusic.get(), 2);
}

MovementTile* LevelScreen::CanMoveToTile(sf::Vector2f _Position)
{
	for (std::vector<MovementTile*>::iterator it = AvailableMovementTiles.begin(); it != AvailableMovementTiles.end(); ++it)
	{
		MovementTile* Tile = *it;
		if(Tile->x == _Position.x && Tile->y == _Position.y)
		{
			return Tile;
		}
	}
	return nullptr;
}

bool LevelScreen::UnitInRange(sf::Vector2f _Position)
{
	for(MovementTile* tile : AvailableAttackableTiles)
	{
		if(tile->x * Tile::TILESIZE == _Position.x && tile->y * Tile::TILESIZE == _Position.y)
		{
			return true;
		}
	}
	return false;
}

void LevelScreen::CursorDeselectUnit()
{
	pCursor->DeselectUnit();
	pCursor->Update();
	pCursor->CheckUnderCursor();
}

bool LevelScreen::CanIndent(int _x, int _y)
{
	if(_x != 0)
	{
		if (((pCursor->getPosition().x + _x) - ScreenOffset.x) < pCurrentMap->GetDimensions().x && pCursor->getPosition().x + _x != pCurrentMap->GetDimensions().x)
		{
			return true;
		}
		return false;
	}

	if(_y != 0)
	{
		if (((pCursor->getPosition().y + _y) - ScreenOffset.y) < pCurrentMap->GetDimensions().y && pCursor->getPosition().x + _x != pCurrentMap->GetDimensions().y)
		{
			return true;
		}
		return false;
	}
	
	return false;
}

bool LevelScreen::IsAtEdge(sf::Keyboard::Key _dir)
{

	switch(_dir)
	{
	case sf::Keyboard::Left: 
		if((pCursor->getPosition().x - ScreenOffset.x) == 0)
		{
			return true;
		}
		break;
	case sf::Keyboard::Right: 
		if((pCursor->getPosition().x - ScreenOffset.x) == (Camera::SCREEN_WIDTH - Tile::TILESIZE))
		{
			return true;
		}
		break;
	case sf::Keyboard::Up: 
		if((pCursor->getPosition().y - ScreenOffset.y) == 0)
		{
			return true;
		}
		break;
	case sf::Keyboard::Down: 
		if((pCursor->getPosition().y - ScreenOffset.y) == ((Camera::SCREEN_HEIGHT / 2) - Tile::TILESIZE))
		{
			return true;
		}
		break;
	}

	return false;
}

void LevelScreen::ClearUnitOverviewScreen()
{
	pUnitOverviewScreen->CallForClearance();
}

void LevelScreen::SetUnitOverviewScreen(Unit* _pNewUnit)
{
	pUnitOverviewScreen->ChangeUnit(_pNewUnit);
}

void LevelScreen::ShowTradeScreen()
{
	pItemTradeScreen->SetTraders(pCursor->GetSelectedUnit(), nullptr);
	pOverlayScreen = pItemTradeScreen;
}

void LevelScreen::ShowInventoryScreen(Unit* _pUnit)
{
	ScreenManager::GetInstance()->GetUnitOverviewScreen()->AddEventOnItemUse<LevelScreen>(&LevelScreen::DisableUnit, this);
	ScreenManager::GetInstance()->GetUnitOverviewScreen()->AddEventOnItemUse<ScreenManager>(&ScreenManager::CloseScreen, ScreenManager::GetInstance());
	ScreenManager::GetInstance()->GetUnitOverviewScreen()->SetUnit(_pUnit);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetUnitOverviewScreen());
}

void LevelScreen::DisableUnit()
{
	//Disable overlay.
	pOverlayScreen = nullptr;

	//Deactivates the unit.
	pCursor->GetSelectedUnit()->DeactivateUnit();

	//Deselects it from the cursor.
	CursorDeselectUnit();

	//The player is going back to idle.
	CurrentPlayerAction = IDLE;
}

void LevelScreen::EndBattle(Unit* _pUnit)
{
	switch(CurrentTurn)
	{
	case PLAYER: 
		//The player is going back to idle.
		CurrentPlayerAction = IDLE;
		//Unit attacked, can't do anything next.
		DisableUnit();
		//Force the units to update, mostly to remove any that died.
		UpdateUnits();
		break;
	case ENEMY: 
		_pUnit->DeactivateUnit();
		pEnemyCursor->DeselectUnit();
		UpdateUnits();
		break;
	case PARTNER: 
		_pUnit->DeactivateUnit();
		pPartnerCursor->DeselectUnit();
		UpdateUnits();
		break;
	}

	//If there is any unit left, transition the music back.
	//Otherwise, game over.
	if(pCurrentMap->PlayerUnits.size() > 0)
	{
		SoundPlayer::GetInstance()->Transition(SoundPlayer::GetInstance()->LevelMusic.get(), 2);
	}
	else
	{
		GameOver = true;
	}
	
	pCursor->CheckUnderCursor();
	TransitioningToBattle = false;
	TransitionTimer = 0;
}

void LevelScreen::PollInput()
{
	sf::Event event;

	if (pOverlayScreen != nullptr && !pOverlayScreen->IsHiding)
	{
		pOverlayScreen->PollInput();
	}
	else
	{
		while (pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				CloseGame = true;
			}
			else if (CurrentTurn == PLAYER && CurrentState == PLAYING && !VictoryAchieved)
			{
				if (event.type == sf::Event::KeyPressed)
				{
					int x = 0;
					int y = 0;

					//DIRECTIONAL KEYS
					if (event.key.code == sf::Keyboard::Left)
					{
						if ((pCursor->getPosition().x - Tile::TILESIZE) >= 0)
						{
							x = -1;
							if (IsAtEdge(sf::Keyboard::Left) && CanIndent(-Tile::TILESIZE, 0))
							{
								ScreenOffset.x -= Tile::TILESIZE;
								GetCamera()->move(-Tile::TILESIZE, 0);
							}
						}
						pCursor->Move(x, y);
					}
					else if (event.key.code == sf::Keyboard::Right)
					{
						if ((pCursor->getPosition().x + Tile::TILESIZE) < pCurrentMap->GetDimensions().x)
						{
							x = 1;
							if (IsAtEdge(sf::Keyboard::Right) && CanIndent(Tile::TILESIZE, 0))
							{
								ScreenOffset.x += Tile::TILESIZE;
								GetCamera()->move(Tile::TILESIZE, 0);
							}
						}
						pCursor->Move(x, y);
					}
					else if (event.key.code == sf::Keyboard::Up)
					{
						if ((pCursor->getPosition().y - Tile::TILESIZE) >= 0)
						{
							y = -1;
							if (IsAtEdge(sf::Keyboard::Up) && CanIndent(0, -Tile::TILESIZE))
							{
								ScreenOffset.y -= Tile::TILESIZE;
								GetCamera()->move(0, -Tile::TILESIZE);
							}
						}
						pCursor->Move(x, y);
					}
					else if (event.key.code == sf::Keyboard::Down)
					{
						if ((pCursor->getPosition().y + Tile::TILESIZE) < pCurrentMap->GetDimensions().y)
						{
							y = 1;
							if (IsAtEdge(sf::Keyboard::Down) && CanIndent(0, Tile::TILESIZE))
							{
								ScreenOffset.y += Tile::TILESIZE;
								GetCamera()->move(0, Tile::TILESIZE);
							}
						}
						pCursor->Move(x, y);
					}
					//NON DIRECTION KEYS
					else if (event.key.code == sf::Keyboard::Z)
					{
						if (pCursor->HasUnitSelected)
						{
							if (CurrentPlayerAction == MOVINGUNIT)
							{
								if (CanMoveToTile(pCursor->getDataPosition()) && !MapHelper::FindUnitAtPos(pCursor->getDataPosition(), pCurrentMap) || pCursor->getDataPosition() == pCursor->GetSelectedUnit()->getDataPosition())
								{
									if(pCursor->getDataPosition() != pCursor->GetSelectedUnit()->getDataPosition())
									{
										pCursor->GetSelectedUnit()->Move(pCursor->getPosition(), pCursor->getDataPosition());
										Trigger* pTrigger = pCurrentMap->OnTrigger(pCursor->getDataPosition());
										if(pTrigger)
										{
											pCurrentMap->SetTriggerStates(pTrigger->GetFamilyID(), false);
											pDialogueScreen->LoadDialogue(pTrigger->GetScriptID());
											pOverlayScreen = pDialogueScreen;
											CurrentPlayerAction = INOVERLAY;
											DelayUnitMenu = true;
										}
									}
									else
									{
										pCursor->GetSelectedUnit()->SetPrevPosition(pCursor->getPosition(), pCursor->getDataPosition());
									}

									if(!DelayUnitMenu)
									{
										pUnitMenuScreen->Load();
										pOverlayScreen = pUnitMenuScreen;
										CurrentPlayerAction = INOVERLAY;
									}
								}
							}
							else if (CurrentPlayerAction == ATTACKING)
							{
								if (pCursor->IsOnUnit)
								{
									if (pCursor->GetUnitBeneathCursor()->IsEnemyUnit() && UnitInRange(pCursor->GetUnitBeneathCursor()->getPosition()))
									{
										pBattleOutcomeScreen->SetNewOutcome(new BattleOutcome(pCursor->GetSelectedUnit(), pCursor->GetUnitBeneathCursor(), MapHelper::GetSupportUnit(pCursor->GetSelectedUnit()->getDataPosition(), pCursor->GetSelectedUnit()->getALLIANCE(), pCurrentMap), MapHelper::GetSupportUnit(pCursor->GetUnitBeneathCursor()->getDataPosition(), pCursor->GetUnitBeneathCursor()->getALLIANCE(), pCurrentMap)));
										pOverlayScreen = pBattleOutcomeScreen;
										CurrentPlayerAction = INOVERLAY;
										//Attack(pCursor->GetSelectedUnit(), pCursor->GetUnitBeneathCursor(), GetSupportUnit(pCursor->GetSelectedUnit()), GetSupportUnit(pCursor->GetUnitBeneathCursor()));
									}
								}
							}
						}
						else if (pCursor->IsOnUnit)
						{
							if (CurrentPlayerAction == IDLE)
							{
								if (pCursor->GetUnitBeneathCursor()->IsPlayerUnit() && pCursor->GetUnitBeneathCursor()->IsActive)
								{
									pCursor->SelectUnit();
									DrawVisualiser();
									CurrentPlayerAction = MOVINGUNIT;
								}
								else if (pCursor->GetUnitBeneathCursor()->IsPartnerUnit() || pCursor->GetUnitBeneathCursor()->IsEnemyUnit())
								{
									ShowInventoryScreen(pCursor->GetUnitBeneathCursor());
								}
							}
						}
					}
					else if (event.key.code == sf::Keyboard::X)
					{
						if (CurrentPlayerAction == MOVINGUNIT)
						{
							if (pCursor->HasUnitSelected)
							{
								CursorDeselectUnit();
								CurrentPlayerAction = IDLE;

								for (auto& pTile : AvailableMovementTiles)
								{
									delete pTile;
								}

								SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
							}
						}
						else if (CurrentPlayerAction == ATTACKING)
						{
							pOverlayScreen->IsHiding = false;
							CurrentPlayerAction = INOVERLAY;
						}
					}
				}
			}
		}
	}
}
