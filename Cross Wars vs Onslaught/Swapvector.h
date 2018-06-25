#pragma once
#include <vector>

template <typename T>
class Swapvector
{
public:
	int Size()
	{
		return InnerVector.size();
	}

	T Back()
	{
		return InnerVector.back();
	}

	T Front()
	{
		return InnerVector.front();
	}

	void Insert(T _t)
	{
		InnerVector.push_back(_t);
	}

	void BackToFront()
	{
		auto it = InnerVector.end();
		std::rotate(InnerVector.begin(), it - 1, it);
	}

	void FrontToBack()
	{
		auto it = InnerVector.begin();
		std::rotate(it, it + 1, InnerVector.end());
	}

	void Clear()
	{
		InnerVector.clear();
	}

	std::vector<T> GetInnerVector()
	{
		return InnerVector;
	}
private:
	std::vector<T> InnerVector;
};
