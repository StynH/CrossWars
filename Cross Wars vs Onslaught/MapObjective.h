#pragma once

class Map;

class MapObjective
{
public:
	MapObjective(Map* _pMap)
	{
		ObjectiveAchieved = false;
		pMap = _pMap;
	};

	virtual ~MapObjective()
	{}

	virtual void CheckObjective(){};
	bool IsObjectiveAchieved() { return ObjectiveAchieved; }
protected:
	Map* pMap;
	bool ObjectiveAchieved;
};
