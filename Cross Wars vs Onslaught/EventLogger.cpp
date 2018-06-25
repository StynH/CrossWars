#include "EventLogger.h"
#include <fstream>


EventLogger::EventLogger()
{
}


EventLogger::~EventLogger()
{
}

void EventLogger::Dump()
{
	std::ofstream Log;
	Log.open("Log.txt");

	for (auto i = 0; i < _EventContextDataStack->size(); ++i) {
		Log << _EventContextNameStack->at(i).c_str() << _EventContextDataStack->at(i).c_str() << std::endl;
	}
	
	Log.close();
}

void EventLogger::Init()
{
	GetInstance()->_EventContextNameStack = new std::vector<std::string>;
	GetInstance()->_EventContextDataStack = new std::vector<std::string>;
}
