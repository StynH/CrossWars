#pragma once
#include "lua.hpp"
#include "LuaBridge.h"
#include <string>
#include "Character.h"
#include "Item.h"
#include <memory>
#include "UnitClass.h"
#include "Entity.h"
#include <json.hpp>

class Weapon;
class UnitSkill;

class Unit : public Entity
{
public:
	const int SPRITE_SIZE = 32;

	struct Costume
	{
		std::string Name;
		CharacterSprite* AttackSprite;
		CharacterSprite* BattleSprite;
		CharacterSprite* DialougeSprite;
		CharacterSprite* SupportSprite;
		sf::Sprite* HUDSprite;
	};

	enum ENTITY_AI_TYPE
	{
		NONE,
		IDLE,
		SENTRY,
		GROUPUP,
		TARGET,
		CHASER
	};

	enum ENTITY_ALLIANCE
	{
		PLAYER,
		ENEMY,
		PARTNER
	};

	enum ARENA_CROWN
	{
		ZERO,
		BRONZE,
		SILVER,
		GOLD
	};

	//LUA ONLY CONSTRUCTOR
	Unit();
	Unit(ENTITY_ALLIANCE _alliance);
	Unit(int _LEVEL, int _ATK, int _DEF, int _SPD, int _LCK, int _MOV, int _MAXHP, ENTITY_ALLIANCE _ALLIANCE);
	~Unit();

	void Update();
	void InitPosition(sf::Vector2f _Position, sf::Vector2f _DataPosition);
	void SetPrevPosition(sf::Vector2f _Position, sf::Vector2f _DataPosition);
	void Move(sf::Vector2f _Position, sf::Vector2f _DataPosition);
	void RevertPosition();

	bool IsPlayerUnit();
	bool IsEnemyUnit();
	bool IsPartnerUnit();
	bool IsUnitInView(sf::Vector2i _position, sf::Vector2i _rectangle);
	void DeactivateUnit();
	void ReactivateUnit();

	void InflictDamage(int _Damage);
	void InflictHealing(int _Damage);

	void setCostume(Costume* _Costume);

	void Debug(const std::string& _Text);

	ENTITY_ALLIANCE getALLIANCE() const { return ALLIANCE; }

	ENTITY_AI_TYPE getAIType() const { return AITYPE; }
	void setAIType(ENTITY_AI_TYPE _AIType) { AITYPE = _AIType; }

	Character* getCHAR() const { return CHARACTER; }
	void setCHAR(Character* _CHAR) { CHARACTER = _CHAR; }

	UnitClass* getCLASS() const { return UNITCLASS; }
	void setCLASS(UnitClass* _CLASS) { UNITCLASS = _CLASS; }

	std::string getID() const { return UNITID; }
	void setID(std::string _UNITID) { UNITID = _UNITID; }

	int getLVL() const { return LVL; }
	void setLVL(int _LVL) { LVL = _LVL; }
	void addLVL(int _INC) { LVL += _INC; }

	int getATK() const { return ATK; }
	void setATK(int _ATK) { ATK = _ATK; }
	void addATK(int _INC) { ATK += _INC; }

	int getDEF() const { return DEF; }
	void setDEF(int _DEF) { DEF = _DEF; }
	void addDEF(int _INC) { DEF += _INC; }

	int getSPD() const { return SPD; }
	void setSPD(int _SPD) { SPD = _SPD; }
	void addSPD(int _INC) { SPD += _INC; }

	int getLCK() const { return LCK; }
	void setLCK(int _LCK) { LCK = _LCK; }
	void addLCK(int _INC) { LCK += _INC; }

	int getMOV() const { return MOV; }
	void setMOV(int _MOV) { MOV = _MOV; }
	void addMOV(int _INC) { MOV += _INC; }

	int getHP() const { return HP; }
	void setHP(int _HP) { HP = _HP; }
	void addHP(int _INC);

	int getMAXHP() const { return MAXHP; }
	void setMAXHP(int _MAXHP) { MAXHP = _MAXHP; }
	void addMAXHP(int _INC) { MAXHP += _INC; }

	void setEXP(int _EXP) { EXP = _EXP; };
	int getEXP() const { return EXP; }
	void giveEXP(int _Amount);

	Costume* getCostume() const { return pCostume; }

	void setArenaKills(int _ArenaKills) { ArenaKills = _ArenaKills; }
	int getArenaKills() const { return ArenaKills; }

	std::vector<Item*> getInventory() { return INV; }

	std::vector<Item*>* getPointerToInventory() { return &INV; };
	void addItemToInventory(Item* _pItem);
	Item* getItemFromInventory(int _Loc);
	void RemoveItemFromInventory(Item* _pItem, bool _Delete);

	Weapon* getEQUIPPED() const { return EQUIPPED; }
	void setEQUIPPED(Weapon* _EQUIPPED) { EQUIPPED = _EQUIPPED; }

	std::vector<UnitSkill*> getSkills() { return SKILLS; };
	void addSkill(UnitSkill* _SKILL, bool _IsPassive);

	void OnEngaged(std::string _EngagerID);
	std::shared_ptr<luabridge::LuaRef> luaOnEngaged;

	void OnDefeated(std::string _EngagerID);
	std::shared_ptr<luabridge::LuaRef> luaOnDefeated;

	sf::Sprite* GetUnitSprite() { return pUnitSprite; }
	sf::Sprite* GetHealthBarBackground() { return pHealthBarBackground; }
	sf::Sprite* GetHealthBarValue() { return pHealthBarValue; }

	void CreateUnitSprite(ENTITY_ALLIANCE _alliance);
	void AddArenaKill();

	bool IsBoss, IsActive, InActiveSquad, InArenaSquad, IsDead;

	sf::Vector2i AIPositionTarget;
	Unit* AIUnitTarget;

	sf::Vector2f CrownPosition;
	sf::Sprite* pCrown;

	friend bool operator== (const Unit &Left, const Unit &Right);
	friend bool operator!= (const Unit &Left, const Unit &Right);
private:
	sf::Vector2f PrevPosition;
	sf::Vector2f PrevDataPosition;

	std::string UNITID;
	int LVL, EXP;
	int ATK, DEF, SPD, LCK, MOV, HP, MAXHP;
	int ArenaKills;

	Character* CHARACTER;
	std::vector<Item*> INV;
	std::vector<UnitSkill*> SKILLS;
	Weapon* EQUIPPED;

	sf::Sprite* pUnitSprite;
	sf::Sprite* pHealthBarBackground;
	sf::Sprite* pHealthBarValue;

	int amountOfCostumes;
	Costume* pCostume;

	UnitClass* UNITCLASS;
	ENTITY_ALLIANCE ALLIANCE;
	ENTITY_AI_TYPE AITYPE;
	ARENA_CROWN CROWN;
};

