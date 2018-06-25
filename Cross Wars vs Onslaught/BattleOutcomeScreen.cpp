#include "BattleOutcomeScreen.h"
#include "TextureManager.h"
#include "GameScreenOverlay.h"
#include "TextSanitizer.h"
#include "ResourceManager.h"
#include "ScreenManager.h"

BattleOutcomeScreen::BattleOutcomeScreen(sf::RenderWindow* _pWindow, LevelScreen* _pLevelScreen) : GameScreenOverlay(_pWindow)
{
	pOverlaySprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("battlepredictionoverlay.png", Folders::FolderType::BATTLE));

	AttackerWeapon = new sf::Text();
	AttackerWeapon->setFont(*ResourceManager::GetInstance()->GetCodersCruxFont());
	AttackerWeapon->setColor(sf::Color::White);
	AttackerWeapon->setCharacterSize(24);

	DefenderWeapon = new sf::Text();
	DefenderWeapon->setFont(*ResourceManager::GetInstance()->GetCodersCruxFont());
	DefenderWeapon->setColor(sf::Color::White);
	DefenderWeapon->setCharacterSize(24);

	AttackerHitChanceText = new sf::Text();
	AttackerHitChanceText->setFont(*ResourceManager::GetInstance()->GetAceFont());
	AttackerHitChanceText->setColor(sf::Color::White);
	AttackerHitChanceText->setCharacterSize(48);

	DefenderHitChanceText = new sf::Text();
	DefenderHitChanceText->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DefenderHitChanceText->setColor(sf::Color::White);
	DefenderHitChanceText->setCharacterSize(48);

	AttackerCritChance = new sf::Text();
	AttackerCritChance->setFont(*ResourceManager::GetInstance()->GetAceFont());
	AttackerCritChance->setColor(sf::Color::White);
	AttackerCritChance->setCharacterSize(24);

	DefenderCritChance = new sf::Text();
	DefenderCritChance->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DefenderCritChance->setColor(sf::Color::White);
	DefenderCritChance->setCharacterSize(24);

	AttackerHPPrediction = new sf::Text();
	AttackerHPPrediction->setFont(*ResourceManager::GetInstance()->GetAceFont());
	AttackerHPPrediction->setColor(sf::Color::White);
	AttackerHPPrediction->setCharacterSize(36);

	AttackerCurrentHP = new sf::Text();
	AttackerCurrentHP->setFont(*ResourceManager::GetInstance()->GetAceFont());
	AttackerCurrentHP->setColor(sf::Color::White);
	AttackerCurrentHP->setCharacterSize(36);

	DefenderCurrentHP = new sf::Text();
	DefenderCurrentHP->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DefenderCurrentHP->setColor(sf::Color::White);
	DefenderCurrentHP->setCharacterSize(36);

	DefenderHPPrediction = new sf::Text();
	DefenderHPPrediction->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DefenderHPPrediction->setColor(sf::Color::White);
	DefenderHPPrediction->setCharacterSize(36);

	DeselectUnitAfterClose = true;

	OnEngage += std::bind(&LevelScreen::Attack, _pLevelScreen, std::placeholders::_1);
}


BattleOutcomeScreen::~BattleOutcomeScreen()
{
}

void BattleOutcomeScreen::Draw()
{
	pWindow->draw(*pBattleOutcome->Attacker->getCHAR()->DialougeSprite->Normal);
	pWindow->draw(*pBattleOutcome->Defender->getCHAR()->DialougeSprite->Reversed);
	pWindow->draw(*pOverlaySprite);

	if(pDefenderWeaponSprite)
	{
		pWindow->draw(*pDefenderWeaponSprite);
		pWindow->draw(*AttackerWeapon);
	}
	
	if(pAttackerWeaponSprite)
	{
		pWindow->draw(*pAttackerWeaponSprite);
		pWindow->draw(*DefenderWeapon);
	}

	pWindow->draw(*AttackerHitChanceText);
	pWindow->draw(*AttackerCurrentHP);
	pWindow->draw(*AttackerCritChance);

	pWindow->draw(*AttackerHPPrediction);

	pWindow->draw(*DefenderHitChanceText);
	pWindow->draw(*DefenderCurrentHP);
	pWindow->draw(*DefenderCritChance);

	pWindow->draw(*DefenderHPPrediction);
}

void BattleOutcomeScreen::Update()
{

}

void BattleOutcomeScreen::SetNewOutcome(BattleOutcome* _pBattleOutcome)
{
	pBattleOutcome = _pBattleOutcome;
	
	AttackerHitChanceText->setString(std::to_string(pBattleOutcome->AttackerHitChance));
	TextSanitizer::CenterOrigin(AttackerHitChanceText);
	AttackerHitChanceText->setPosition(40, 216);

	DefenderHitChanceText->setString(std::to_string(pBattleOutcome->DefenderHitChance));
	TextSanitizer::CenterOrigin(DefenderHitChanceText);
	DefenderHitChanceText->setPosition(472, 216);

	AttackerCurrentHP->setString(std::to_string(pBattleOutcome->Attacker->getHP()));
	TextSanitizer::CenterOrigin(AttackerCurrentHP);
	AttackerCurrentHP->setPosition(117, 227);

	AttackerHPPrediction->setString(std::to_string(pBattleOutcome->AttackerFutureHP));
	TextSanitizer::CenterOrigin(AttackerHPPrediction);
	AttackerHPPrediction->setPosition(185, 227);

	AttackerCritChance->setString(std::to_string(pBattleOutcome->AttackerCritChance));
	TextSanitizer::CenterOrigin(AttackerCritChance);
	AttackerCritChance->setPosition(213, 172);

	DefenderCurrentHP->setString(std::to_string(pBattleOutcome->Defender->getHP()));
	TextSanitizer::CenterOrigin(DefenderCurrentHP);
	DefenderCurrentHP->setPosition(395, 227);

	DefenderHPPrediction->setString(std::to_string(pBattleOutcome->DefenderFutureHP));
	TextSanitizer::CenterOrigin(DefenderHPPrediction);
	DefenderHPPrediction->setPosition(327, 227);

	DefenderCritChance->setString(std::to_string(pBattleOutcome->DefenedrCritChance));
	TextSanitizer::CenterOrigin(DefenderCritChance);
	DefenderCritChance->setPosition(299, 172);

	Item* pAttackerWeapon = pBattleOutcome->Attacker->getItemFromInventory(0);
	Item* pDefenderWeapon = pBattleOutcome->Defender->getItemFromInventory(0);

	if(AttackerWeapon)
	{
		pAttackerWeaponSprite = std::make_shared<sf::Sprite>(*pAttackerWeapon->getTexture());
		pAttackerWeaponSprite->setPosition(6, 132);

		AttackerWeapon->setString(pAttackerWeapon->getName());
		TextSanitizer::CenterOriginHorizontally(AttackerWeapon);
		AttackerWeapon->setPosition(30, 140);
	}
	
	if (DefenderWeapon)
	{
		pDefenderWeaponSprite = std::make_shared<sf::Sprite>(*pDefenderWeapon->getTexture());
		pDefenderWeaponSprite->setPosition(490, 132);

		DefenderWeapon->setString(pDefenderWeapon->getName());
		TextSanitizer::CenterOriginHorizontallyRight(DefenderWeapon);
		DefenderWeapon->setPosition(483, 140);
	}
}

void BattleOutcomeScreen::Unload()
{
	SafeDelete(pBattleOutcome);
}

void BattleOutcomeScreen::Hide()
{

}

void BattleOutcomeScreen::PollInput()
{
	sf::Event event;

	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			CloseGame = true;
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				CloseScreen = true;
				MustUnload = true;
				pBattleOutcome->Attacker->RevertPosition();
			}
			else if(event.key.code == sf::Keyboard::Z) 
			{
				OnEngage(pBattleOutcome);
			}
		}
	}
}

void BattleOutcomeScreen::Load()
{

}
