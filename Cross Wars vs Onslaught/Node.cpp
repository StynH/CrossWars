#include "Node.h"
#include "MathHelper.h"

Node::Node(int _x, int _y, int _e)
{
	x = _x;
	y = _y;
	walkable = _e;
	isClosed = false;
	penalty = g = h = 0;
	parent = nullptr;
}


Node::~Node()
{

}

int Node::CompareTo(Node * _nodeToCompare)
{
	int compare = MathHelper::CompareTo(GetFCost(), _nodeToCompare->GetFCost());
	if (compare == 0)
	{
		compare = MathHelper::CompareTo(h, _nodeToCompare->h);
	}
	return -compare;
}
