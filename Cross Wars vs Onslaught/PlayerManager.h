#pragma once
#include <string>
#include "Singleton.h"
#include <vector>
#include "Unit.h"
#include "GameFlag.h"

struct PreloadChapter
{
	std::string Name;
	std::string ID;
	int UnitLimit;
};

class PlayerManager : public TSingleton<PlayerManager>
{
public:
	PlayerManager();
	~PlayerManager();

	std::string GetPlayerName() { return PlayerName; };
	void SetPlayerName(std::string _Name) { PlayerName = _Name; };

	std::vector<Unit*>& GetReadyUnits() { return TeamUnits; }
	std::vector<Unit*>& GetInactiveUnits() { return InactiveUnits; }
	std::vector<Unit*>& GetArenaUnits() { return ArenaUnits; }

	void HealPlayerUnits();

	void AddReadyUnit(Unit* _pUnit);
	void AddInactiveUnit(Unit* _pUnit);
	void AddAvailableMission(PreloadChapter _Preload);
	void SwitchUnitFromTeams(Unit* _pUnit);
	void SwitchUnitFromArena(Unit* _pUnit);
	Unit* FindUnit(std::string _GUID);

	int GetEnergyUnits();
	void AddEnergyUnits(int _Amount);
	void DeductEnergyUnits(int _Amount);

	std::vector<PreloadChapter> GetAvailableMissions() { return AvailableMissions; }

	void Load();
	void NewGame();
	void LoadSaveFile();
	void Reset();
	void CreateSaveFile();

	static void AddCreateableItem(std::string _ItemID);
private:
	std::string PlayerName;

	std::vector<Unit*> TeamUnits;
	std::vector<Unit*> InactiveUnits;
	std::vector<Unit*> ArenaUnits;

	std::vector<PreloadChapter> AvailableMissions;

	int EnergyAmount;
};
