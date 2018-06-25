#include "BattleCharScreen.h"
#include "TextureManager.h"
#include "TextSanitizer.h"
#include "LuaManager.h"
#include <thread>
#include <mutex>
#include "ResourceManager.h"
#include "UnitSkill.h"

BattleCharScreen::BattleCharScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	sf::Texture* HudTexture = TextureManager::GetInstance()->LoadTexture("chardisplay\\hud.png", Folders::FolderType::HUD);

	InactiveHud = new sf::Sprite(*HudTexture);
	InactiveHud->setTextureRect(sf::IntRect(0, 0, 512, 256));
	InactiveHud->setPosition(0, 0);

	ActiveHud = new sf::Sprite(*HudTexture);
	ActiveHud->setTextureRect(sf::IntRect(512, 0, 512, 256));
	ActiveHud->setPosition(0, 0);

	SkillContainer = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("chardisplay\\skilldisplay.png", Folders::FolderType::HUD));
	SkillContainer->setPosition(89, 148);

	HUDFont = new sf::Font;
	if (!HUDFont->loadFromFile(Folders::ROOT_FOLDER + Folders::DIALOGUE_FOLDER + "Ace-Attorney.ttf"))
	{
		std::cout << "Failed to load HUD font!" << std::endl;
	}

	NameText = new sf::Text();
	NameText->setFont(*HUDFont);
	NameText->setColor(sf::Color::White);
	NameText->setCharacterSize(16);

	ClassText = new sf::Text();
	ClassText->setFont(*HUDFont);
	ClassText->setColor(sf::Color::White);
	ClassText->setCharacterSize(16);

	HPText = new sf::Text();
	HPText->setFont(*HUDFont);
	HPText->setColor(sf::Color::White);
	HPText->setCharacterSize(16);

	LVLText = new sf::Text();
	LVLText->setFont(*HUDFont);
	LVLText->setColor(sf::Color::White);
	LVLText->setCharacterSize(24);

	InvSlot = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("chardisplay\\inv_slot.png", Folders::FolderType::HUD));

	pSubCamera = new Camera(sf::FloatRect(0, 0, 512, 512));
	pSubCamera->setViewport(sf::FloatRect(0, 0.5f, 1, 1));

	IsActive = false;
	EmptyScreen = false;
}


BattleCharScreen::~BattleCharScreen()
{
	SafeDelete(pSubCamera);

	SafeDelete(HPText);
	SafeDelete(LVLText);

	SafeDelete(InactiveHud);
	SafeDelete(ActiveHud);
}

void BattleCharScreen::Draw()
{
	pSubCamera->DrawUpdate();

	if (IsActive)
	{
		pWindow->draw(*ActiveHud);
		pWindow->draw(*pCurrentUnit->getCHAR()->HUDSprite);
		pWindow->draw(*SkillContainer);

		DrawStats();
		DrawInventory();
		DrawSkills();

		if (EmptyScreen)
		{
			UnsetUnit();
		}
	}
	else
	{
		pWindow->draw(*InactiveHud);
	}

}

void BattleCharScreen::InitStats()
{
	LVLText->setString("LVL " + std::to_string(pCurrentUnit->getLVL()));
	LVLText->setCharacterSize(24);
	TextSanitizer::CenterOrigin(LVLText);
	LVLText->setPosition(108, 228);

	int PosX = 350;
	int PosY = 49;
	int c = 0, y = 0;

	for (Stat* CurrentStat : Stats)
	{
		delete CurrentStat;
	}

	Stats.clear();

	for (auto stat : UnitStats)
	{
		Stat* NewStat = new Stat;

		std::shared_ptr<sf::Text> Title = std::make_shared<sf::Text>();
		Title->setFont(*ResourceManager::GetInstance()->GetAceFont());
		Title->setColor(sf::Color::White);
		Title->setString(stat.first);
		Title->setPosition(PosX, PosY);
		Title->setCharacterSize(16);

		std::shared_ptr<sf::Text> Value = std::make_shared<sf::Text>();
		Value->setFont(*ResourceManager::GetInstance()->GetAceFont());
		Value->setColor(sf::Color::White);
		Value->setString(stat.second);
		Value->setPosition(PosX + 73, PosY);
		Value->setCharacterSize(16);
		
		NewStat->Name = Title;
		NewStat->Value = Value;

		Stats.push_back(NewStat);

		//Crappy solution to fix MOV being centered.
		//At the time of typing this, it's incredibly annoying and I dont want to spend
		//too much time doing tiny crap like this right now.
		//TODO: Fix this please.
		if (c > 1)
		{
			y = 1;
		}

		PosY += 26 + y;
		++c;
	}

	HPText->setString("HP " + std::to_string(pCurrentUnit->getHP()) + "/" + std::to_string(pCurrentUnit->getMAXHP()));
	HPText->setCharacterSize(26);
	TextSanitizer::CenterOrigin(HPText);
	HPText->setPosition(391, 213);

	NameText->setString(pCurrentUnit->getCHAR()->GetNAME());
	TextSanitizer::CenterOrigin(NameText);
	NameText->setPosition(108, 34);

	ClassText->setString(pCurrentUnit->getCLASS()->GetNAME());
	TextSanitizer::CenterOrigin(ClassText);
	ClassText->setPosition(242, 34);
}

void BattleCharScreen::InitInventory()
{
	int PosX = 204;
	int PosY = 79;

	for (InventoryItem* InventoryItem : Inventory)
	{
		delete InventoryItem;
	}

	Inventory.clear();

	for (auto i = 0; i < 5; ++i)
	{
		if (pCurrentUnit->getInventory().size() > i)
		{
			InventoryItem* InvItem = new InventoryItem;

			int ItemY = 84 + (28 * i);
			int SpriteY = 84 + (28 * i);
			Item* CurrentItem = pCurrentUnit->getItemFromInventory(i);

			std::shared_ptr<sf::Text> InventoryTitle = std::make_shared<sf::Text>();
			
			std::string ItemName = CurrentItem->getName();
			if(ItemName.size() > 11)
			{
				ItemName.resize(8);
				ItemName.resize(11, '.');
			}

			InventoryTitle->setFont(*ResourceManager::GetInstance()->GetCodersCruxFont());
			InventoryTitle->setColor(sf::Color::Black);
			InventoryTitle->setCharacterSize(14);
			InventoryTitle->setString(ItemName);
			InventoryTitle->setOrigin(InventoryTitle->getLocalBounds().left, static_cast<int>(InventoryTitle->getLocalBounds().height / 2.0f));
			InventoryTitle->setPosition(230, ItemY);

			InvItem->Sprite = std::make_shared<sf::Sprite>(*CurrentItem->getTexture());
			InvItem->Sprite->setPosition(209, SpriteY);
			InvItem->Title = InventoryTitle;

			Inventory.push_back(InvItem);
		}

		PosY += 28;
	}
}

void BattleCharScreen::InitSkills()
{
	int PosX = 95;
	int PosY = 156;

	int TempCount = 1;

	for (UnitSkill* InventoryItem : pCurrentUnit->getSkills())
	{
		InventoryItem->SkillSprite->setPosition(PosX, PosY);

		PosX += 24;

		if(TempCount == 3)
		{
			PosY += 24;
			PosX = 95;
		}

		++TempCount;
	}
}

void BattleCharScreen::DrawInventory()
{
	int PosX = 205;
	int PosY = 79;

	for (auto i = 0; i < 5; ++i)
	{
		InvSlot->setPosition(PosX, PosY);
		pWindow->draw(*InvSlot);
		PosY += 28;
	}

	for(InventoryItem* InvItem : Inventory)
	{
		pWindow->draw(*InvItem->Sprite);
		pWindow->draw(*InvItem->Title);
	}
}

void BattleCharScreen::DrawSkills()
{
	for(UnitSkill* Skill : pCurrentUnit->getSkills())
	{
		pWindow->draw(*Skill->SkillSprite);
	}
}

void BattleCharScreen::DrawStats()
{
	for(Stat* CurrentStat : Stats)
	{
		pWindow->draw(*CurrentStat->Name);
		pWindow->draw(*CurrentStat->Value);
	}

	pWindow->draw(*NameText);
	pWindow->draw(*ClassText);
	pWindow->draw(*LVLText);
	pWindow->draw(*HPText);
}

void BattleCharScreen::ChangeUnit(Unit* _pNewUnit)
{
	Stats.clear();
	Inventory.clear();

	pCurrentUnit = _pNewUnit;
	IsActive = true;

	UnitStats = std::map<std::string, std::string>();
	UnitStats.insert(make_pair("ATK", std::to_string(pCurrentUnit->getATK())));
	UnitStats.insert(make_pair("DEF", std::to_string(pCurrentUnit->getDEF())));
	UnitStats.insert(make_pair("SPD", std::to_string(pCurrentUnit->getSPD())));
	UnitStats.insert(make_pair("LCK", std::to_string(pCurrentUnit->getLCK())));
	UnitStats.insert(make_pair("MOV", std::to_string(pCurrentUnit->getMOV())));

	InitInventory();
	InitStats();
	InitSkills();
}

void BattleCharScreen::UnsetUnit()
{
	pCurrentUnit = nullptr; 
	EmptyScreen = false; 
	IsActive = false;
}

void BattleCharScreen::Update()
{

}

void BattleCharScreen::Unload()
{
	pCurrentUnit = nullptr;
}

void BattleCharScreen::PollInput()
{

}
