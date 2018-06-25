#pragma once
#include <vector>
#include "Heap.h"

class Node : public IHeapItem<Node*>
{
public:
	Node(int _x, int _y, int _e);
	~Node();

	std::vector<Node*> neighbors;
	Node* parent;
	bool isClosed;
	int g, h, x, y, penalty;
	bool walkable;

	int GetFCost() { return g + h; }
	int CompareTo(Node* _nodeToCompare) override;
};
