#pragma once
#include "SFML/System/Vector2.hpp"
#include <vector>
#include "Heap.h"
#include "Map.h"
#include "Node.h"
#include "FloodFill.h"

class AStar
{
public:
	static std::vector<MovementTile> NodesToMovementTiles(const std::vector<Node*> &Nodes);
	static Node* GetNodeFromWorldPosition(int _x, int _y, const std::vector<std::vector<Node*>> &_pNodeMap);
	static std::vector<std::vector<Node*>>* CollisionLayerToNodes(std::vector<std::vector<Tile*>>* vector);
	static std::vector<MovementTile> GeneratePath(sf::Vector2f startPos, sf::Vector2i endPos);
	static std::vector<Node*> GetNeighbours(Node* _pNode, std::vector<std::vector<Node*>>& _pNodeMap);

	static void Remove(std::vector<Node*>* _pVector, Node* _pNode);
	static bool EnemyAtPosition(Node* _pNode, const std::vector<sf::Vector2f>& _Positions);
	static int GetDistance(Node* nodeA, Node* nodeB);

	static void InsertMap(Map* _pCurrentMap) { pCurrentMap = _pCurrentMap; };
private:
	static Map* pCurrentMap;
};

