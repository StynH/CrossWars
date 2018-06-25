#pragma once
#include "Unit.h"
#include "FloodFill.h"
#include "Map.h"

class MapHelper
{
public:
	static std::vector<MovementTile*> MapHelper::GetTilesToDistance(Map* _pMap, sf::Vector2f _DataPosition, int _Distance, std::vector<sf::Vector2f> _EnemyPos)
	{
		int PosX = _DataPosition.x;
		int PosY = _DataPosition.y;

		std::vector<std::vector<Tile*>> FarestTiles;

		int y = 0;

		//Cut out a part of the map with the size of wich the unit can maximally move to.
		//You only need to add 1 to the rectangle but I like to add a gutter, just in case.
		for (auto i = -_Distance - 2; i < _Distance + 2; ++i)
		{
			FarestTiles.push_back(std::vector<Tile*>());
			for (auto j = -_Distance - 2; j < _Distance + 2; ++j)
			{
				if (PosX + j < 0 || PosX + j > (_pMap->GetDimensions().x / Tile::TILESIZE) - 1 || PosY + i < 0 || PosY + i > (_pMap->GetDimensions().y / Tile::TILESIZE) - 1) continue;

				FarestTiles.at(y).push_back(_pMap->GetTileAt(PosY + i, PosX + j));
			}
			++y;
		}

		FarestTiles.erase(std::remove_if(FarestTiles.begin(), FarestTiles.end(), [](std::vector<Tile*> _Vector) { return _Vector.empty(); }), FarestTiles.end());

		return FloodFill::FloodFillTiles(FarestTiles, _DataPosition, _Distance, _EnemyPos);
	}

	static bool MapHelper::EnemyAtPosition(sf::Vector2f _Pos, const std::vector<sf::Vector2f> &_Positions)
	{
		for (sf::Vector2f Position : _Positions)
		{
			if (_Pos.x == Position.x && _Pos.y == Position.y)
			{
				return true;
			}
		}
		return false;
	}

	//Support priority goes like this.
	//Right > Left > Down > Top
	static Unit* MapHelper::GetSupportUnit(sf::Vector2f _DataPosition, Unit::ENTITY_ALLIANCE _Alliance,  Map* _pMap)
	{
		Unit* UnitRight = nullptr;
		Unit* UnitLeft = nullptr;
		Unit* UnitDown = nullptr;
		Unit* UnitTop = nullptr;

		switch (_Alliance)
		{
		case Unit::ENTITY_ALLIANCE::PARTNER:
		case Unit::ENTITY_ALLIANCE::PLAYER:
			for (auto& CurrentUnit : _pMap->PlayerUnits)
			{
				sf::Vector2f UnitDataPosition = CurrentUnit->getDataPosition();
				if (UnitDataPosition.x == _DataPosition.x + 1 && UnitDataPosition.y == _DataPosition.y)
				{
					UnitRight = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x - 1 && UnitDataPosition.y == _DataPosition.y)
				{
					UnitLeft = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y + 1)
				{
					UnitDown = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y - 1)
				{
					UnitTop = CurrentUnit;
				}
			}
			for (auto& CurrentUnit : _pMap->PartnerUnits)
			{
				sf::Vector2f UnitDataPosition = CurrentUnit->getDataPosition();
				if (UnitDataPosition.x == _DataPosition.x + 1 && UnitDataPosition.y == _DataPosition.y)
				{
					UnitRight = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x - 1 && UnitDataPosition.y == _DataPosition.y)
				{
					UnitLeft = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y + 1)
				{
					UnitDown = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y - 1)
				{
					UnitTop = CurrentUnit;
				}
			}
			break;
		case Unit::ENTITY_ALLIANCE::ENEMY:
			for (auto& CurrentUnit : _pMap->EnemyUnits)
			{
				sf::Vector2f UnitDataPosition = CurrentUnit->getDataPosition();
				if (UnitDataPosition.x == _DataPosition.x + 1 && UnitDataPosition.y == _DataPosition.y)
				{
					UnitRight = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x - 1 && UnitDataPosition.y == _DataPosition.y)
				{
					UnitLeft = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y + 1)
				{
					UnitDown = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y - 1)
				{
					UnitTop = CurrentUnit;
				}
			}
			break;
		}

		if (UnitRight)
		{
			return UnitRight;
		}
		else if (UnitLeft)
		{
			return UnitLeft;
		}
		else if (UnitDown)
		{
			return UnitDown;
		}
		else if (UnitTop)
		{
			return UnitTop;
		}

		return nullptr;
	}

	static Unit* MapHelper::GetAISupportUnit(sf::Vector2f _DataPosition, Unit::ENTITY_ALLIANCE _Alliance, std::string _GUID, Map* _pMap)
	{
		Unit* UnitRight = nullptr;
		Unit* UnitLeft = nullptr;
		Unit* UnitDown = nullptr;
		Unit* UnitTop = nullptr;

		switch (_Alliance)
		{
		case Unit::ENTITY_ALLIANCE::PARTNER:
		case Unit::ENTITY_ALLIANCE::PLAYER:
			for (auto& CurrentUnit : _pMap->PlayerUnits)
			{
				sf::Vector2f UnitDataPosition = CurrentUnit->getDataPosition();
				if (UnitDataPosition.x == _DataPosition.x + 1 && UnitDataPosition.y == _DataPosition.y && CurrentUnit->getGUID() != _GUID)
				{
					UnitRight = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x - 1 && UnitDataPosition.y == _DataPosition.y && CurrentUnit->getGUID() != _GUID)
				{
					UnitLeft = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y + 1 && CurrentUnit->getGUID() != _GUID)
				{
					UnitDown = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y - 1 && CurrentUnit->getGUID() != _GUID)
				{
					UnitTop = CurrentUnit;
				}
			}
			for (auto& CurrentUnit : _pMap->PartnerUnits)
			{
				sf::Vector2f UnitDataPosition = CurrentUnit->getDataPosition();
				if (UnitDataPosition.x == _DataPosition.x + 1 && UnitDataPosition.y == _DataPosition.y && CurrentUnit->getGUID() != _GUID)
				{
					UnitRight = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x - 1 && UnitDataPosition.y == _DataPosition.y && CurrentUnit->getGUID() != _GUID)
				{
					UnitLeft = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y + 1 && CurrentUnit->getGUID() != _GUID)
				{
					UnitDown = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y - 1 && CurrentUnit->getGUID() != _GUID)
				{
					UnitTop = CurrentUnit;
				}
			}
			break;
		case Unit::ENTITY_ALLIANCE::ENEMY:
			for (auto& CurrentUnit : _pMap->EnemyUnits)
			{
				sf::Vector2f UnitDataPosition = CurrentUnit->getDataPosition();
				if (UnitDataPosition.x == _DataPosition.x + 1 && UnitDataPosition.y == _DataPosition.y && CurrentUnit->getGUID() != _GUID)
				{
					UnitRight = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x - 1 && UnitDataPosition.y == _DataPosition.y && CurrentUnit->getGUID() != _GUID)
				{
					UnitLeft = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y + 1 && CurrentUnit->getGUID() != _GUID)
				{
					UnitDown = CurrentUnit;
				}
				else if (UnitDataPosition.x == _DataPosition.x && UnitDataPosition.y == _DataPosition.y - 1 && CurrentUnit->getGUID() != _GUID)
				{
					UnitTop = CurrentUnit;
				}
			}
			break;
		}

		if (UnitRight)
		{
			return UnitRight;
		}
		else if (UnitLeft)
		{
			return UnitLeft;
		} 
		else if (UnitDown)
		{
			return UnitDown;
		}
		else if (UnitTop)
		{
			return UnitTop;
		}

		return nullptr;
	}

	//Needs the Data Position
	static Unit* MapHelper::FindUnitAtPos(sf::Vector2f _DataPosition, Map* _pMap)
	{
		for (auto& PlayerUnit : _pMap->PlayerUnits)
		{
			if (PlayerUnit->getDataPosition().x == _DataPosition.x && PlayerUnit->getDataPosition().y == _DataPosition.y)
			{
				return PlayerUnit;
			}
		}

		for (auto& EnemyUnit : _pMap->EnemyUnits)
		{
			if (EnemyUnit->getDataPosition().x == _DataPosition.x && EnemyUnit->getDataPosition().y == _DataPosition.y)
			{
				return EnemyUnit;
			}
		}

		for (auto& PartnerUnit : _pMap->PartnerUnits)
		{
			if (PartnerUnit->getDataPosition().x == _DataPosition.x && PartnerUnit->getDataPosition().y == _DataPosition.y)
			{
				return PartnerUnit;
			}
		}
		return nullptr;
	}

	static std::vector<sf::Vector2f> GetEnemyPositions(Unit::ENTITY_ALLIANCE _Alliance, const Map* _pMap)
	{
		std::vector<sf::Vector2f> Positions;
		if(_Alliance == Unit::PLAYER || _Alliance == Unit::PARTNER)
		{
			for (auto& EnemyUnit : _pMap->EnemyUnits)
			{
				Positions.push_back(EnemyUnit->getDataPosition());
			}
		}
		else
		{
			for (auto& PlayerUnit : _pMap->PlayerUnits)
			{
				Positions.push_back(PlayerUnit->getDataPosition());
			}
			for (auto& PartnerUnit : _pMap->PartnerUnits)
			{
				Positions.push_back(PartnerUnit->getDataPosition());
			}
		}
		return Positions;
	}
};
