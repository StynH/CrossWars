#include "Unit.h"
#include "LuaManager.h"
#include "UnitManager.h"
#include "Camera.h"
#include "UnitSkill.h"
#include <boost/lexical_cast.hpp>

Unit::Unit()
{
	IsActive = true;
	pHealthBarBackground = new sf::Sprite(*UnitManager::GetInstance()->GetHealthBarTexture(), sf::Rect<int>(0, 0, 32, 8));
	pHealthBarValue = new sf::Sprite(*UnitManager::GetInstance()->GetHealthBarTexture(), sf::Rect<int>(3, 8, 26, 2));
	IsDead = false;
	ArenaKills = 0;
	EXP = 0;
}

Unit::Unit(ENTITY_ALLIANCE _alliance)
{
	ALLIANCE = _alliance;
	IsActive = true;
	pHealthBarBackground = new sf::Sprite(*UnitManager::GetInstance()->GetHealthBarTexture(), sf::Rect<int>(0, 0, 32, 8));
	pHealthBarValue = new sf::Sprite(*UnitManager::GetInstance()->GetHealthBarTexture(), sf::Rect<int>(3, 8, 26, 2));
	IsDead = false;
	ArenaKills = 0;
	EXP = 0;
}

Unit::Unit(int _LVL, int _ATK, int _DEF, int _SPD, int _LCK, int _MOV, int _MAXHP, ENTITY_ALLIANCE _ALLIANCE)
{
	IsActive = true;
	LVL = _LVL;
	ATK = _ATK;
	DEF = _DEF;
	SPD = _SPD;
	LCK = _LCK;
	MOV = _MOV;
	HP = _MAXHP;
	MAXHP = _MAXHP;
	ALLIANCE = _ALLIANCE;
	EXP = 0;

	pHealthBarBackground = new sf::Sprite(*UnitManager::GetInstance()->GetHealthBarTexture(), sf::Rect<int>(0, 0, 32, 8));
	pHealthBarValue = new sf::Sprite(*UnitManager::GetInstance()->GetHealthBarTexture(), sf::Rect<int>(3, 8, 26, 2));
	IsDead = false;
}

Unit::~Unit()
{

}

void Unit::Update()
{
	pUnitSprite->setPosition(getPosition());
	pHealthBarBackground->setPosition(getPosition().x ,getPosition().y + 24);

	pHealthBarValue->setTextureRect(sf::IntRect(3, 8, int(static_cast<float>(HP) / static_cast<float>(MAXHP) * 26), 2));
	pHealthBarValue->setPosition(getPosition().x + 3, getPosition().y + 27);
}

void Unit::InitPosition(sf::Vector2f _Position, sf::Vector2f _DataPosition)
{
	SetPrevPosition(_Position, _DataPosition);
	setPosition(_Position, _DataPosition);
}

void Unit::SetPrevPosition(sf::Vector2f _Position, sf::Vector2f _DataPosition)
{
	PrevPosition = _Position;
	PrevDataPosition = _DataPosition;
}

void Unit::Move(sf::Vector2f _Position, sf::Vector2f _DataPosition)
{
	PrevPosition = getPosition();
	PrevDataPosition = getDataPosition();
	setPosition(_Position, _DataPosition);
}

void Unit::RevertPosition()
{
	setPosition(PrevPosition, PrevDataPosition);
}

void Unit::CreateUnitSprite(ENTITY_ALLIANCE _alliance)
{
	switch(_alliance)
	{
	case ENTITY_ALLIANCE::PLAYER: 
		pUnitSprite = new sf::Sprite(*UnitManager::GetInstance()->GetPlayerUnitTexture(), *getCLASS()->GetUnitTextureCoords());
		break;
	case ENTITY_ALLIANCE::ENEMY: 
		pUnitSprite = new sf::Sprite(*UnitManager::GetInstance()->GetEnemyUnitTexture(), *getCLASS()->GetUnitTextureCoords());
		break;
	case ENTITY_ALLIANCE::PARTNER: 
		pUnitSprite = new sf::Sprite(*UnitManager::GetInstance()->GetPartnerUnitTexture(), *getCLASS()->GetUnitTextureCoords());
		break;
	default: 
		pUnitSprite = new sf::Sprite(*UnitManager::GetInstance()->GetPlayerUnitTexture(), *getCLASS()->GetUnitTextureCoords());
		break;
	}
}

void Unit::AddArenaKill()
{
	ArenaKills += 1;

	if(ArenaKills == 50 && CROWN == ZERO)
	{
		CROWN = BRONZE;
	}
	else if (ArenaKills == 150 && CROWN == BRONZE)
	{
		CROWN = SILVER;
	}
	else if (ArenaKills == 500 && CROWN == SILVER)
	{
		CROWN = GOLD;
	}
}

bool Unit::IsPlayerUnit()
{
	return ALLIANCE == ENTITY_ALLIANCE::PLAYER ? true : false;
}

bool Unit::IsEnemyUnit()
{
	return ALLIANCE == ENTITY_ALLIANCE::ENEMY ? true : false;
}

bool Unit::IsPartnerUnit()
{
	return ALLIANCE == ENTITY_ALLIANCE::PARTNER ? true : false;
}


bool Unit::IsUnitInView(sf::Vector2i _position, sf::Vector2i _rectangle)
{
	sf::Vector2f UnitPos = getPosition();

	if (
		(UnitPos.x + SPRITE_SIZE) <= (_position.x + Camera::SCREEN_WIDTH)
		&& (UnitPos.x) >= (_position.x)
		&& (UnitPos.y) >= (_position.y)
		&& (UnitPos.y + SPRITE_SIZE) <= (_position.y + (Camera::SCREEN_HEIGHT / 2))
		)
	{
		return true;
	}
	return false;
}

void Unit::DeactivateUnit()
{
	pUnitSprite->setColor(sf::Color(100, 100, 100, 255));
	IsActive = false;
}

void Unit::ReactivateUnit()
{
	pUnitSprite->setColor(sf::Color(255, 255, 255, 255));
	IsActive = true;
}

void Unit::InflictDamage(int _Damage)
{
	int HealthResult = HP - _Damage;
	if(HealthResult <= 0)
	{
		HP = 0;
		IsDead = true;
	}else
	{
		HP = HealthResult;
	}
}

void Unit::InflictHealing(int _Damage)
{

}

void Unit::setCostume(Costume* _Costume)
{
	pCostume = _Costume;
}

void Unit::Debug(const std::string& _Text)
{
	std::cout << _Text << std::endl;
}

void Unit::addHP(int _INC)
{
	if (HP + _INC < MAXHP) 
	{
		HP += _INC;
	}
	else 
	{
		HP = MAXHP;
	}
}

void Unit::giveEXP(int _Amount)
{
	EXP += _Amount;
	while(EXP >= 100)
	{
		EXP -= 100;
	}
}

void Unit::addItemToInventory(Item* _pItem)
{
	INV.push_back(_pItem);
}

Item* Unit::getItemFromInventory(int _Loc)
{
	if (INV.size() == 0) return nullptr;
	return INV.at(_Loc);
}

void Unit::RemoveItemFromInventory(Item* _pItem, bool _Delete)
{
	for (std::vector<Item*>::iterator iter = INV.begin(); iter != INV.end(); ++iter)
	{
		if ((*iter)->getGUID() == _pItem->getGUID())
		{
			if(_Delete)
			{
				delete *iter;
			}
			INV.erase(iter);
			break;
		}
	}
}

void Unit::addSkill(UnitSkill* _SKILL, bool _IsPassive)
{
	if(SKILLS.size() < 6)
	{
		if(_SKILL->TriggerType == UnitSkill::ACTIVATIONTIME::ONGAIN && !_IsPassive)
		{
			_SKILL->Trigger(this, nullptr, nullptr);
		}
		SKILLS.push_back(_SKILL);
	}
}

void Unit::OnEngaged(std::string _EngagerID)
{
	if (luaOnEngaged) {
		try {
			(*luaOnEngaged)(this, _EngagerID);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "LuaException: " << e.what() << std::endl;
		}
	}
}

void Unit::OnDefeated(std::string _EngagerID)
{
	if (luaOnDefeated) {
		try {
			(*luaOnDefeated)(this, _EngagerID);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "LuaException: " << e.what() << std::endl;
		}
	}
}

bool operator==(const Unit& Left, const Unit& Right)
{
	return Left.getGUID() == Right.getGUID();
}

bool operator!=(const Unit& Left, const Unit& Right)
{
	return Left.getGUID() != Right.getGUID();
}
