#pragma once
#include <vector>

template <typename T>
class IHeapItem
{
public:
	virtual ~IHeapItem()
	{
	}

	IHeapItem() : HeapIndex(0) {}
	int HeapIndex;

	virtual int CompareTo(T _itemToCompare) = 0;
};

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
class Heap
{
public:
	Heap(int _maxHeapSize);
	~Heap();

	void Add(T _item);
	T RemoveFirst();
	void UpdateItem(T _item);
	bool Contains(T _item);
	void SortDown(T _item);
	void SortUp(T _item);
	void Swap(T _itemA, T _itemB);
	int Size() { return currentItemCount; }

private:
	std::vector<T> Items;
	int currentItemCount;
};

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
Heap<T>::Heap(int _maxHeapSize) :
	Items(_maxHeapSize),
	currentItemCount(0)
{}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
Heap<T>::~Heap()
{

}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
void Heap<T>::Add(T _item)
{
	_item->HeapIndex = currentItemCount;
	Items[currentItemCount] = _item;
	SortUp(_item);
	currentItemCount++;
}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
T Heap<T>::RemoveFirst()
{
	T firstItem = Items[0];
	currentItemCount--;
	Items[0] = Items[currentItemCount];
	Items[0]->HeapIndex = 0;
	SortDown(Items[0]);
	return firstItem;
}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
void Heap<T>::UpdateItem(T _item)
{
	SortUp(_item);
}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
bool Heap<T>::Contains(T _item)
{
	return Items[_item->HeapIndex] == _item;
}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
void Heap<T>::SortDown(T _item)
{
	while (true)
	{
		int childIndexLeft = _item->HeapIndex * 2 + 1;
		int childIndexRight = _item->HeapIndex * 2 + 2;
		int swapIndex = 0;

		if (childIndexLeft < currentItemCount)
		{
			swapIndex = childIndexLeft;
			if (childIndexRight < currentItemCount)
				if (Items[childIndexLeft]->CompareTo(Items[childIndexRight]) < 0)
					swapIndex = childIndexRight;

			if (_item->CompareTo(Items[swapIndex]) < 0)
				Swap(_item, Items[swapIndex]);
			else return;
		}
		else return;
	}
}

template<class T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
void Heap<T>::SortUp(T _item)
{
	int parentIndex = (_item->HeapIndex - 1) / 2;

	while (true)
	{
		T parentItem = Items[parentIndex];
		if (_item->CompareTo(parentItem) > 0)
			Swap(_item, parentItem);
		else
			break;

		parentIndex = (_item->HeapIndex - 1) / 2;
	}
}

template<typename T = std::enable_if_t<std::is_base_of<IHeapItem, T>::value>>
void Heap<T>::Swap(T _itemA, T _itemB)
{
	Items[_itemA->HeapIndex] = _itemB;
	Items[_itemB->HeapIndex] = _itemA;
	int itemAIndex = _itemA->HeapIndex;
	_itemA->HeapIndex = _itemB->HeapIndex;
	_itemB->HeapIndex = itemAIndex;
}