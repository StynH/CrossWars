#pragma once
#include "Singleton.h"
#include <vector>

class EventLogger : public TSingleton<EventLogger> 
{
public:
	EventLogger();
	~EventLogger();

	void Dump();
	void Init();

	std::vector<std::string>* _EventContextNameStack;
	std::vector<std::string>* _EventContextDataStack;
};

class EventContext
{
public:
	EventContext(const char *_pName, const char *_pData) {
		EventLogger::GetInstance()->_EventContextNameStack->push_back(std::string(_pName));
		EventLogger::GetInstance()->_EventContextDataStack->push_back(std::string(_pData));
	}
	EventContext(const char *_pName, std::string *_pData) {
		EventLogger::GetInstance()->_EventContextNameStack->push_back(std::string(_pName));
		EventLogger::GetInstance()->_EventContextDataStack->push_back(*_pData);
	}
};