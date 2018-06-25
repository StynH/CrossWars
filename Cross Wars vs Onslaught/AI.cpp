#include "AI.h"
#include "Weapon.h"
#include "GameTime.h"
#include <iostream>
#include "FloodFill.h"
#include "MapHelper.h"
#include "BattleOutcome.h"
#include "ScreenManager.h"
#include "AStar.h"
#include "MathHelper.h"

AI::AI(Unit::ENTITY_ALLIANCE _Alliance, Cursor* _pCursor)
{
	AIAlliance = _Alliance;
	pAICursor = _pCursor;
	AITimer = 0;
	AIDone = false;
	CurrentState = NOUNIT;
}

AI::~AI()
{
}

void AI::Think()
{
	//The AI will move every 0.6 seconds, gives a more natural look.
	//Looks like the guy is thinking.
	//But change 0.6 to 0.01 and see how fast he really is. Kinda boring.
	if (AITimer >= 0.6F && CurrentState != DONE)
	{
		if (CurrentState == NOUNIT)
		{
			Unit* FreeUnit = GetFreeUnit();

			if (FreeUnit)
			{
				pAICursor->setPosition(FreeUnit->getPosition(), FreeUnit->getDataPosition());
				pAICursor->SelectUnit();

				CurrentState = PICKPOSITION;
			}
			else
			{
				AIDone = true;
			}
		}
		else if (CurrentState == PICKPOSITION)
		{
			Unit* CurrentUnit = pAICursor->GetSelectedUnit();

			bool UnitIsLow = UnitIsLowHP(CurrentUnit);
			Useable* pHealingItem = GetHealingItem(CurrentUnit);

			if (UnitIsLowHP(CurrentUnit) && pHealingItem)
			{
				pHealingItem->Use(CurrentUnit);
				CurrentUnit->RemoveItemFromInventory(pHealingItem, true);

				CurrentUnit->DeactivateUnit();
				pAICursor->DeselectUnit();

				CurrentState = NOUNIT;
			}
			else if(!UnitIsLow || !pHealingItem)
			{
				AvailableNodes = std::priority_queue<AINode>();

				TargetNode = std::make_unique<AINode>();
				TargetNode->InRangeOfAttack = false;

				Weapon* EquippedWeapon = static_cast<Weapon*>(CurrentUnit->getItemFromInventory(0));
				if (CurrentUnit->getAIType() != Unit::ENTITY_AI_TYPE::IDLE)
				{
					std::vector<MovementTile*> PossibleUnitMovement = MapHelper::GetTilesToDistance(pCurrentMap, CurrentUnit->getDataPosition(), CurrentUnit->getMOV(), MapHelper::GetEnemyPositions(AIAlliance, pCurrentMap));

					for (auto& pTile : PossibleUnitMovement)
					{
						if (!MapHelper::FindUnitAtPos(pTile->DataPosition, pCurrentMap) || MapHelper::FindUnitAtPos(pTile->DataPosition, pCurrentMap)->getGUID() == CurrentUnit->getGUID())
						{
							int Score = 0;
							AINode NewNode;
							NewNode.InRangeOfAttack = false;

							Unit* SupportUnit = MapHelper::GetAISupportUnit(pTile->DataPosition, CurrentUnit->getALLIANCE(), CurrentUnit->getGUID(), pCurrentMap);

							if (SupportUnit)
							{
								Score += 5;
								NewNode.HasSupport = true;
								NewNode.SupportInRange = SupportUnit;
							}
							else
							{
								NewNode.HasSupport = false;
								SupportUnit = nullptr;
							}

							std::vector<MovementTile*> AvailableAttackableTiles = MapHelper::GetTilesToDistance(pCurrentMap, pTile->DataPosition, EquippedWeapon->getRNG(), std::vector<sf::Vector2f>());
							std::vector<Unit*> EnemiesInRange = GetEnemiesInRange(AvailableAttackableTiles);

							if (EnemiesInRange.size() > 0)
							{
								if (EnemiesInRange.size() > 1)
								{
									int BestBattleScore = -9999;
									Unit* BestTarget = nullptr;

									for (auto& pEnemyUnit : EnemiesInRange)
									{
										int ThisBattleScore = 0;
										Unit* EnemySupport = MapHelper::GetAISupportUnit(pEnemyUnit->getDataPosition(), pEnemyUnit->getALLIANCE(), pEnemyUnit->getGUID(), pCurrentMap);

										BattleOutcome FutureBattle(CurrentUnit, pEnemyUnit, SupportUnit, EnemySupport);

										//I didn't die?
										if (FutureBattle.AttackerFutureHP > 0)
										{
											ThisBattleScore += FutureBattle.AttackerFutureHP;
										}
										else
										{
											ThisBattleScore -= 50;
										}

										if (FutureBattle.DefenderFutureHP > 0)
										{
											ThisBattleScore -= FutureBattle.AttackerFutureHP;
										}
										else
										{
											ThisBattleScore += 50;
										}

										if (ThisBattleScore > BestBattleScore)
										{
											BestBattleScore = ThisBattleScore;
											BestTarget = pEnemyUnit;
										}
									}

									Score += 15;

									if (BestBattleScore > 0) Score += BestBattleScore;

									NewNode.InRangeOfAttack = true;
									NewNode.UnitInRange = BestTarget;
									Unit* EnemySupport = MapHelper::GetAISupportUnit(NewNode.UnitInRange->getDataPosition(), NewNode.UnitInRange->getALLIANCE(), NewNode.UnitInRange->getGUID(), pCurrentMap);
									NewNode.Battle = std::make_shared<BattleOutcome>(CurrentUnit, NewNode.UnitInRange, SupportUnit, EnemySupport);
								}
								else
								{
									Score += 15;
									NewNode.InRangeOfAttack = true;
									NewNode.UnitInRange = EnemiesInRange.at(0);
									Unit* EnemySupport = MapHelper::GetAISupportUnit(NewNode.UnitInRange->getDataPosition(), NewNode.UnitInRange->getALLIANCE(), NewNode.UnitInRange->getGUID(), pCurrentMap);
									NewNode.Battle = std::make_shared<BattleOutcome>(CurrentUnit, NewNode.UnitInRange, SupportUnit, EnemySupport);
								}
							}

							if (pTile->DataPosition == CurrentUnit->getDataPosition()) Score += 1;

							NewNode.Tile = pTile;

							NewNode.Score = Score;
							AvailableNodes.push(NewNode);
						}
					}

					if (AvailableNodes.top().Score == 1)
					{
						if (CurrentUnit->getAIType() == Unit::ENTITY_AI_TYPE::TARGET)
						{
							std::vector<MovementTile> Path = AStar::GeneratePath(CurrentUnit->getDataPosition(), CurrentUnit->AIPositionTarget);
							TargetNode->Tile = GetFarestMovableTileFromPath(Path, CurrentUnit->getMOV());

							if (TargetNode->Tile)
							{
								pAICursor->setPosition(sf::Vector2f(TargetNode->Tile->x * 32, TargetNode->Tile->y * 32), sf::Vector2f(TargetNode->Tile->x, TargetNode->Tile->y));
								CurrentState = SETUNIT;
							}
							else
							{
								CurrentUnit->setAIType(Unit::SENTRY);
								CurrentUnit->DeactivateUnit();
								pAICursor->DeselectUnit();
								CurrentState = NOUNIT;
							}
						}
						else if (CurrentUnit->getAIType() == Unit::ENTITY_AI_TYPE::CHASER)
						{
							if(!CurrentUnit->AIUnitTarget || CurrentUnit->AIUnitTarget->IsDead)
							{
								CurrentUnit->AIUnitTarget = FindEnemyToAttack();
							}

							std::vector<MovementTile> Path = AStar::GeneratePath(CurrentUnit->getDataPosition(), static_cast<sf::Vector2i>(CurrentUnit->AIUnitTarget->getDataPosition()));
							TargetNode->Tile = GetFarestMovableTileFromPath(Path, CurrentUnit->getMOV());

							if (TargetNode->Tile)
							{
								pAICursor->setPosition(sf::Vector2f(TargetNode->Tile->x * 32, TargetNode->Tile->y * 32), sf::Vector2f(TargetNode->Tile->x, TargetNode->Tile->y));
								CurrentState = SETUNIT;
							}
							else
							{
								CurrentUnit->DeactivateUnit();
								pAICursor->DeselectUnit();
								CurrentState = NOUNIT;
							}
						}
						else
						{
							CurrentUnit->DeactivateUnit();
							pAICursor->DeselectUnit();
							CurrentState = NOUNIT;
						}
					}
					else
					{
						TargetNode = std::make_unique<AINode>(AvailableNodes.top());

						if (TargetNode->Score < 5 && CurrentUnit->getAIType() == Unit::ENTITY_AI_TYPE::GROUPUP)
						{
							CurrentUnit->DeactivateUnit();
							pAICursor->DeselectUnit();
							CurrentState = NOUNIT;
						}

						else if (TargetNode->Score < 15 && CurrentUnit->getAIType() == Unit::ENTITY_AI_TYPE::SENTRY)
						{
							CurrentUnit->DeactivateUnit();
							pAICursor->DeselectUnit();
							CurrentState = NOUNIT;
						}
						else if (TargetNode->Score < 15 && CurrentUnit->getAIType() == Unit::ENTITY_AI_TYPE::TARGET)
						{
							std::vector<MovementTile> Path = AStar::GeneratePath(CurrentUnit->getDataPosition(), CurrentUnit->AIPositionTarget);
							TargetNode->Tile = GetFarestMovableTileFromPath(Path, CurrentUnit->getMOV());
							pAICursor->setPosition(sf::Vector2f(TargetNode->Tile->x * 32, TargetNode->Tile->y * 32), sf::Vector2f(TargetNode->Tile->x, TargetNode->Tile->y));
							CurrentState = SETUNIT;
						}
						else if (TargetNode->Score < 15 && CurrentUnit->getAIType() == Unit::ENTITY_AI_TYPE::CHASER)
						{
							if (!CurrentUnit->AIUnitTarget || CurrentUnit->AIUnitTarget->IsDead)
							{
								CurrentUnit->AIUnitTarget = FindEnemyToAttack();
							}

							std::vector<MovementTile> Path = AStar::GeneratePath(CurrentUnit->getDataPosition(), static_cast<sf::Vector2i>(CurrentUnit->AIUnitTarget->getDataPosition()));
							TargetNode->Tile = GetFarestMovableTileFromPath(Path, CurrentUnit->getMOV());
							pAICursor->setPosition(sf::Vector2f(TargetNode->Tile->x * 32, TargetNode->Tile->y * 32), sf::Vector2f(TargetNode->Tile->x, TargetNode->Tile->y));
							CurrentState = SETUNIT;
						}
						else if (TargetNode->Tile->DataPosition != CurrentUnit->getDataPosition())
						{
							pAICursor->setPosition(sf::Vector2f(TargetNode->Tile->x * 32, TargetNode->Tile->y * 32), sf::Vector2f(TargetNode->Tile->x, TargetNode->Tile->y));
							CurrentState = SETUNIT;
						}
						else
						{
							if (TargetNode->InRangeOfAttack)
							{
								CurrentState = PREPARINGATTACK;
							}
							else
							{
								CurrentUnit->DeactivateUnit();
								pAICursor->DeselectUnit();
								CurrentState = NOUNIT;
							}
						}
					}
				}
				else
				{
					CurrentUnit->DeactivateUnit();
					CurrentState = NOUNIT;
				}
			}	
		}
		else if (CurrentState == SETUNIT)
		{
			pAICursor->GetSelectedUnit()->Move(pAICursor->getPosition(), pAICursor->getDataPosition());

			if (TargetNode->InRangeOfAttack)
			{
				CurrentState = PREPARINGATTACK;
			}
			else
			{
				pAICursor->GetSelectedUnit()->DeactivateUnit();
				pAICursor->DeselectUnit();
				CurrentState = NOUNIT;
			}
		}
		else if (CurrentState == PREPARINGATTACK)
		{
			pAICursor->setPosition(TargetNode->UnitInRange->getPosition(), TargetNode->UnitInRange->getDataPosition());
			CurrentState = ATTACKING;
		}
		else if (CurrentState == ATTACKING)
		{
			ScreenManager::GetInstance()->GetLevelScreen()->Attack(TargetNode->Battle.get());
			CurrentState = NOUNIT;
		}
		AITimer = 0;
	}

	AITimer += GameTime::DeltaTime;
}

void AI::Acivate()
{
	CurrentState = NOUNIT;
	AIDone = false;
	AITimer = 0;
}

void AI::SetMap(Map* _pMap)
{
	pCurrentMap = _pMap;
}

Unit* AI::GetFreeUnit()
{
	switch (AIAlliance)
	{
	case Unit::ENTITY_ALLIANCE::ENEMY:
		for (auto pUnit : pCurrentMap->EnemyUnits)
		{
			if (pUnit->IsActive)
			{
				return pUnit;
			}
		}
		break;
	case Unit::ENTITY_ALLIANCE::PARTNER:
		for (auto pUnit : pCurrentMap->PartnerUnits)
		{
			if (pUnit->IsActive)
			{
				return pUnit;
			}
		}
		break;
	}

	return nullptr;
}

std::vector<Unit*> AI::GetEnemiesInRange(std::vector<MovementTile*>& _Tiles)
{
	std::vector<Unit*> AttackAbleUnits;

	switch (AIAlliance)
	{
	case Unit::ENTITY_ALLIANCE::PARTNER:
	case Unit::ENTITY_ALLIANCE::PLAYER:
		for (auto& pUnit : pCurrentMap->EnemyUnits)
		{
			for (auto& pTile : _Tiles)
			{
				if (pUnit->getDataPosition() == pTile->DataPosition)
				{
					AttackAbleUnits.push_back(pUnit);
				}
			}
		}
		break;
	case Unit::ENTITY_ALLIANCE::ENEMY:
		for (auto& pUnit : pCurrentMap->PlayerUnits)
		{
			for (auto& pTile : _Tiles)
			{
				if (pUnit->getDataPosition() == pTile->DataPosition)
				{
					AttackAbleUnits.push_back(pUnit);
				}
			}
		}
		for (auto& pUnit : pCurrentMap->PartnerUnits)
		{
			for (auto& pTile : _Tiles)
			{
				if (pUnit->getDataPosition() == pTile->DataPosition)
				{
					AttackAbleUnits.push_back(pUnit);
				}
			}
		}
		break;
	}

	return AttackAbleUnits;
}

MovementTile* AI::GetFarestMovableTileFromPath(std::vector<MovementTile> _pPath, int _Distance)
{
	if (_pPath.size() == 0)
	{
		return nullptr;
	}

	if (_pPath.size() >= _Distance)
	{
		for (auto& pTile : _pPath)
		{

			if (pTile.Distance == _Distance)
			{
				if (!MapHelper::FindUnitAtPos(pTile.DataPosition, pCurrentMap))
				{
					return new MovementTile(pTile);
				}
				return GetFarestMovableTileFromPath(_pPath, _Distance - 1);
			}
		}
	}
	else
	{
		Unit* pUnit = MapHelper::FindUnitAtPos(_pPath.back().DataPosition, pCurrentMap);
		if (!pUnit || pUnit->getGUID() == pAICursor->GetSelectedUnit()->getGUID())
		{
			return new MovementTile(_pPath.back());
		}
		return GetFarestMovableTileFromPath(_pPath, _Distance);
	}
}

Unit* AI::FindEnemyToAttack()
{
	std::vector<Unit*> Units;

	switch (AIAlliance)
	{
	case Unit::ENTITY_ALLIANCE::PARTNER:
	case Unit::ENTITY_ALLIANCE::PLAYER:
		for (auto& pUnit : pCurrentMap->EnemyUnits)
		{
			Units.push_back(pUnit);
		}
		break;
	case Unit::ENTITY_ALLIANCE::ENEMY:
		for (auto& pUnit : pCurrentMap->PlayerUnits)
		{
			Units.push_back(pUnit);
		}
		for (auto& pUnit : pCurrentMap->PartnerUnits)
		{
			Units.push_back(pUnit);
		}
		break;
	}	

	int MinDistance = INT_MAX;
	Unit* BestUnit = nullptr;
	for(Unit* pUnit : Units)
	{
		int Distance = MathHelper::DistanceBetweenTiles(pAICursor->GetSelectedUnit()->getPosition(), pUnit->getPosition());
		if(Distance < MinDistance)
		{
			MinDistance = Distance;
			BestUnit = pUnit;
		}
	}
	return BestUnit;
}

bool AI::UnitIsLowHP(Unit* _pUnit)
{
	float UnitHealthStatus = float(_pUnit->getHP() / _pUnit->getMAXHP());
	return UnitHealthStatus * 100.0f <= 40.0f;
}

Useable* AI::GetHealingItem(Unit* _pUnit)
{
	for(auto* pItem : _pUnit->getInventory())
	{
		if(pItem->getType() == Item::ACTION)
		{
			Useable* pUseable = static_cast<Useable*>(pItem);
			if(pUseable->GetUsetype() == Useable::HEAL)
			{
				return pUseable;
			}
		}
	}
	return nullptr;
}