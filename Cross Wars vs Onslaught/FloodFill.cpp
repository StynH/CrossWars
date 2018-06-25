#include "FloodFill.h"
#include "MapHelper.h"

std::vector<std::vector<MovementTile*>> FloodFill::TileList;
std::vector<sf::Vector2f> FloodFill::EnemyList;
int FloodFill::MoveDistance = 0;

std::vector<MovementTile*> FloodFill::FloodFillTiles(const std::vector<std::vector<Tile*>> &_Tiles, sf::Vector2f _Origin, int _MoveDistance, const std::vector<sf::Vector2f> &_EnemyPos)
{
	TileList.clear();
	EnemyList = _EnemyPos;

	MovementTile* Origin = nullptr;

	MoveDistance = _MoveDistance;

	int y = 0;
	int x = 0;

	int OriginX = 0;
	int OriginY = 0;

	for (std::vector<Tile*> TileRow : _Tiles)
	{
		if (TileRow.empty()) continue;

		TileList.push_back(std::vector<MovementTile*>());
		for (Tile* OldTile : TileRow)
		{
			MovementTile* NewTile = new MovementTile(OldTile->y, OldTile->x, x, y, OldTile->IsPassable());
			TileList.at(y).push_back(NewTile);

			if (NewTile->x == _Origin.x && NewTile->y == _Origin.y)
			{
				Origin = NewTile;
				OriginX = x;
				OriginY = y;
			}
			++x;
		}
		++y;
		x = 0;
	}

	if (Origin != nullptr)
	{
		//Fill(Origin, OriginX, OriginY);
		QueueFill(Origin);
	}

	std::vector<MovementTile*> MoveableTiles;

	for (std::vector<MovementTile*> TileRow : TileList)
	{
		if (TileRow.size() == 0) continue;

		for (MovementTile* AvailableTile : TileRow)
		{
			if (AvailableTile->CanMoveTo)
			{
				MoveableTiles.push_back(AvailableTile);
			}
			else
			{
				delete AvailableTile;
			}
		}
	}

	return MoveableTiles;
}

void FloodFill::Fill(MovementTile* _tile, int _x, int _y)
{
	if (!_tile->Visited)
	{
		if (!_tile->IsPassable)
		{
			_tile->Visited = true;
		}
		else
		{
			_tile->Visited = true;
			_tile->CanMoveTo = true;

			if (_x - 1 > 0)
			{
				Fill(TileList[_y][_x - 1], _x - 1, _y);
			}

			if (_y - 1 > 0)
			{
				Fill(TileList[_y - 1][_x], _x, _y - 1);
			}

			if (_y + 1 < TileList[_y].size())
			{
				Fill(TileList[_y + 1][_x], _x, _y + 1);
			}

			if (_x + 1 < TileList[_y].size())
			{
				Fill(TileList[_y][_x + 1], _x + 1, _y);
			}
		}
	}
}

void FloodFill::QueueFill(MovementTile* _tile)
{
	{
		if (!_tile->IsPassable) return;

		std::queue<MovementTile*> Nodequeue;
		Nodequeue.push(_tile);

		while (!Nodequeue.empty())
		{
			MovementTile* Node = Nodequeue.front();
			Nodequeue.pop();

			int Distance = Node->Distance;

			if (!Node->IsPassable || Distance > MoveDistance)
			{
				Node->Visited = true;
			}
			else if(MapHelper::EnemyAtPosition(Node->DataPosition, EnemyList))
			{
				Node->Visited = true;
				Node->CanMoveTo = true;
				Node->HasEnemy = true;
			}
			else
			{
				Node->Visited = true;
				Node->CanMoveTo = true;

				int x = Node->DataX;
				int y = Node->DataY;

				if (x - 1 > 0)
				{
					if (!TileList[y][x - 1]->Visited)
					{
						TileList[y][x - 1]->Distance = Distance + 1;
						Nodequeue.push(TileList[y][x - 1]);
					}
				}

				if (y - 1 > 0)
				{
					if (!TileList[y - 1][x]->Visited)
					{
						TileList[y - 1][x]->Distance = Distance + 1;
						Nodequeue.push(TileList[y - 1][x]);
					}
				}

				if (y + 1 < TileList.size())
				{
					if (!TileList[y + 1][x]->Visited)
					{
						TileList[y + 1][x]->Distance = Distance + 1;
						Nodequeue.push(TileList[y + 1][x]);
					}
				}

				if (x + 1 < TileList[y].size() - 1)
				{
					if (!TileList[y][x + 1]->Visited)
					{
						TileList[y][x + 1]->Distance = Distance + 1;
						Nodequeue.push(TileList[y][x + 1]);
					}
				}
			}
		}
	}
}
