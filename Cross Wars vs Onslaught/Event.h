#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>

template<class EventArgs>
class Event
{
public:
	struct Subscriber {
		Subscriber(std::function<void(EventArgs)> _function, long _id)
		{
			Function = _function;
			ID = _id;
		}

		std::function<void(EventArgs)> Function;
		long ID;
	};
	~Event()
	{
		Clear();
	}
	void operator+=(std::function<void(EventArgs)> func)
	{
		Subscribers.push_back(new Subscriber(func, (long)(func.target_type().hash_code())));
	}
	void operator-=(std::function<void(EventArgs)> func)
	{
		long subID = (long)(func.target_type().hash_code());
		for (Subscriber* sub : Subscribers)
		{
			if (sub->ID == subID)
			{
				Subscribers.erase(std::remove(Subscribers.begin(), Subscribers.end(), sub), Subscribers.end());
				break;
			}
		}
	}
	void operator()(EventArgs e)
	{
		for (Subscriber* sub : Subscribers)
		{
			sub->Function(e);
		}
	}
	void Clear()
	{
		for (Subscriber* sub : Subscribers)
		{
			delete sub;
			sub = nullptr;
		}
	}
private:
	std::vector<Subscriber*> Subscribers;
};
