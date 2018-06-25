#include "AStar.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include "Heap.h"
#include <queue>
#include "Map.h"
#include "Node.h"
#include "FloodFill.h"

Map* AStar::pCurrentMap = nullptr;

std::vector<MovementTile> AStar::GeneratePath(sf::Vector2f startPos, sf::Vector2i endPos)
{
	std::shared_ptr<std::vector<std::vector<Node*>>> pNodeMap(CollisionLayerToNodes(pCurrentMap->GetCollisionLayer()));
	Node* start = GetNodeFromWorldPosition(startPos.y, startPos.x, *pNodeMap.get());
	Node* goal = GetNodeFromWorldPosition(endPos.y, endPos.x, *pNodeMap.get());

	Heap<Node*> openSet(pCurrentMap->GetDimensions().x * pCurrentMap->GetDimensions().y);
	std::unordered_set<Node*> closedSet;

	start->g = 0;

	openSet.Add(start);
	while (openSet.Size() > 0)
	{
		Node* currentNode = openSet.RemoveFirst();

		closedSet.insert(currentNode);
		currentNode->isClosed = true;

		if (currentNode->x == goal->x && currentNode->y == goal->y)
		{
			goal->parent = currentNode->parent;
			break;
		}
		for (Node* neighbour : GetNeighbours(currentNode, *pNodeMap))
		{
			if (neighbour != nullptr)
			{
				if (neighbour->isClosed || !neighbour->walkable) continue;
				int newMovementCostToNeighbour = currentNode->g + GetDistance(currentNode, neighbour);
				if (newMovementCostToNeighbour < neighbour->g || !openSet.Contains(neighbour))
				{
					neighbour->g = newMovementCostToNeighbour;
					neighbour->h = GetDistance(neighbour, goal);
					neighbour->parent = currentNode;

					if (!openSet.Contains(neighbour)) openSet.Add(neighbour);
					else openSet.UpdateItem(neighbour);
				}
			}
		}
	}

	for (Node* node : closedSet)
	{
		node->isClosed = false;
	}

	std::vector<Node*> nodes;
	Node* current = goal;
	while (current != start && current != nullptr)
	{
		nodes.push_back(current);
		current = current->parent;
	}
	if (nodes.size() <= 0)
	{
		return std::vector<MovementTile>();
	}
	reverse(nodes.begin(), nodes.end());

	return NodesToMovementTiles(nodes);
}

std::vector<MovementTile> AStar::NodesToMovementTiles(const std::vector<Node*> &Nodes)
{
	std::vector<MovementTile> Path;
	int Distance = 0;

	for (auto& pNode : Nodes)
	{
		++Distance;
		MovementTile pNewTile(pNode->y, pNode->x, 0, 0, true);
		pNewTile.Distance = Distance;
		Path.push_back(pNewTile);
	}

	for (auto& TBD : Nodes)
	{
		delete TBD;
	}

	return Path;
}


Node* AStar::GetNodeFromWorldPosition(int _x, int _y, const std::vector<std::vector<Node*>> &_pNodeMap)
{
	for (std::vector<Node*> nodeRow : _pNodeMap)
	{
		for (Node* node : nodeRow)
		{
			if (node->x == _x && node->y == _y)
			{
				return node;
			}
		}
	}

	return nullptr;
}

std::vector<std::vector<Node*>>* AStar::CollisionLayerToNodes(std::vector<std::vector<Tile*>>* _CollisionLayer)
{
	std::vector<std::vector<Node*>>* NewNodeMap = new std::vector<std::vector<Node*>>;

	for (int i = 0; i < _CollisionLayer->size(); ++i) {
		std::vector<Node*> row;
		for (int j = 0; j < _CollisionLayer->at(i).size(); ++j) {
			Tile* CurrentTile = static_cast<Tile*>(_CollisionLayer->at(i).at(j));
			Node* pNewNode = new Node(i, j, CurrentTile->IsPassable());
			row.push_back(pNewNode);
		}
		NewNodeMap->push_back(row);
	}

	return NewNodeMap;
}

int AStar::GetDistance(Node* nodeA, Node* nodeB)
{
	int distX = abs(nodeA->x - nodeB->x);
	int distY = abs(nodeA->y - nodeB->y);

	return 1 * sqrt(distX * distX + distY * distY);
}

std::vector<Node*> AStar::GetNeighbours(Node* _pNode, std::vector<std::vector<Node*>>& _pNodeMap)
{
	int floorBoundsX = pCurrentMap->GetDimensions().x;
	int floorBoundsY = pCurrentMap->GetDimensions().y;

	std::vector<Node*> neighbours;
	neighbours.reserve(8);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (x == 0 && y == 0 || x == -1 && y == -1 || x == 1 && y == 1 || x == -1 && y == 1 || x == 1 && y == -1) continue;

			int checkX = _pNode->x + x;
			int checkY = _pNode->y + y;

			if (checkX >= 0 && checkX < floorBoundsX && checkY >= 0 && checkY < floorBoundsY)
			{
				neighbours.push_back(_pNodeMap[checkX][checkY]);
			}
		}
	}
	return neighbours;
}

void AStar::Remove(std::vector<Node*>* _pVector, Node* _pNode)
{
	int pos = find(_pVector->begin(), _pVector->end(), _pNode) - _pVector->begin();
	auto it = _pVector->begin() + pos;
	rotate(it, it + 1, _pVector->end());
	_pVector->pop_back();
}

bool AStar::EnemyAtPosition(Node* _pNode, const std::vector<sf::Vector2f> &_Positions)
{
	for(sf::Vector2f Position : _Positions)
	{
		if(_pNode->x == Position.x && _pNode->y == Position.y)
		{
			return true;
		}
	}
	return false;
}