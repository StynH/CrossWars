#include "BattleScreen.h"
#include "BattleHelper.h"
#include "BattleOutcome.h"
#include "GameTime.h"
#include "TextSanitizer.h"
#include "ScreenManager.h"
#include "MindBend.h"
#include "MusicPlayer.h"

BattleScreen::BattleScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pDialogueOverlay = new BattleDialogueOverlay(_pRenderWindow);

	Background = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("battledisplay\\bg.png", Folders::FolderType::HUD));
	Background->setPosition(0, 256);

	SupportBlue = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("battledisplay\\supportattack.png", Folders::FolderType::HUD));
	SupportBlue->setPosition(0, 400);

	SupportRed = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("battledisplay\\supportdefend.png", Folders::FolderType::HUD));
	SupportRed->setPosition(343, 400);

	UnitExpCounter = new sf::Text("0", *ResourceManager::GetInstance()->GetAceFont(), 36);
	UnitExpCounter->setColor(sf::Color::White);
	UnitExpCounter->setPosition(373, 50);
	UnitExpGainCounter = new sf::Text("0", *ResourceManager::GetInstance()->GetAceFont(), 36);
	UnitExpGainCounter->setColor(sf::Color::White);
	UnitExpGainCounter->setPosition(272, 50);

	UnitSupportExpCounter = new sf::Text("0", *ResourceManager::GetInstance()->GetAceFont(), 36);
	UnitSupportExpCounter->setColor(sf::Color::White);
	UnitSupportExpCounter->setPosition(373, 115);

	UnitSupporGainExpCounter = new sf::Text("0", *ResourceManager::GetInstance()->GetAceFont(), 36);
	UnitSupporGainExpCounter->setColor(sf::Color::White);
	UnitSupporGainExpCounter->setPosition(272, 115);

	sf::Texture* CritMissTexture = TextureManager::GetInstance()->LoadTexture("critmiss.png", Folders::FolderType::BATTLE);
	CritSprite = new sf::Sprite(*CritMissTexture, sf::IntRect(0, 0, 87, 31));
	MissSprite = new sf::Sprite(*CritMissTexture, sf::IntRect(87, 0, 55, 31));

	HPOverlay = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("hpshow.png", Folders::FolderType::BATTLE));
	EXPGained = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("expgain.png", Folders::FolderType::BATTLE));
	EXPGained->setPosition(215, 14);
	SkillIncrease = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("skillup.png", Folders::FolderType::BATTLE));
	LevelUpOverlay = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("levelup.png", Folders::FolderType::BATTLE));
	LevelUpOverlay->setPosition(278, 18);
	OldLevel = new sf::Text("0", *ResourceManager::GetInstance()->GetAceFont(), 36);
	OldLevel->setColor(sf::Color::White);
	OldLevel->setCharacterSize(36);
	NewLevel = new sf::Text("0", *ResourceManager::GetInstance()->GetAceFont(), 36);
	NewLevel->setColor(sf::Color::White);
	NewLevel->setCharacterSize(46);

	EXPScreenContainer = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("expscreen.png", Folders::FolderType::BATTLE));
	EXPScreenContainer->setPosition(84, 48);

	EXPScreenContainerTwo = new sf::Sprite(*EXPScreenContainer);
	EXPScreenContainerTwo->setPosition(84, 114);

	AttackerHP = new sf::Text();
	AttackerHP->setFont(*ResourceManager::GetInstance()->GetAceFont());
	AttackerHP->setColor(sf::Color::White);
	AttackerHP->setCharacterSize(36);

	DefenderHP = new sf::Text();
	DefenderHP->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DefenderHP->setColor(sf::Color::White);
	DefenderHP->setCharacterSize(36);

	Vignette = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("vignette.png", Folders::FolderType::BATTLE));

	sf::Text* ATKName = new sf::Text();
	ATKName->setCharacterSize(16);
	ATKName->setFont(*ResourceManager::GetInstance()->GetAceFont());
	ATKName->setString("ATK:");
	TextSanitizer::CenterOriginHorizontally(ATKName);
	ATKName->setPosition(302, 125);
	UnitATK.Name = std::shared_ptr<sf::Text>(ATKName);

	sf::Text* DEFName = new sf::Text();
	DEFName->setCharacterSize(16);
	DEFName->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DEFName->setString("DEF:");
	TextSanitizer::CenterOriginHorizontally(DEFName);
	DEFName->setPosition(302, 156);
	UnitDEF.Name = std::shared_ptr<sf::Text>(DEFName);

	sf::Text* LCKName = new sf::Text();
	LCKName->setCharacterSize(16);
	LCKName->setFont(*ResourceManager::GetInstance()->GetAceFont());
	LCKName->setString("LCK:");
	TextSanitizer::CenterOriginHorizontally(LCKName);
	LCKName->setPosition(384, 125);
	UnitLCK.Name = std::shared_ptr<sf::Text>(LCKName);

	sf::Text* SPDName = new sf::Text();
	SPDName->setCharacterSize(16);
	SPDName->setFont(*ResourceManager::GetInstance()->GetAceFont());
	SPDName->setString("SPD:");
	TextSanitizer::CenterOriginHorizontally(SPDName);
	SPDName->setPosition(384, 156);
	UnitSPD.Name = std::shared_ptr<sf::Text>(SPDName);

	sf::Text* HPName = new sf::Text();
	HPName->setCharacterSize(16);
	HPName->setFont(*ResourceManager::GetInstance()->GetAceFont());
	HPName->setString("HP:");
	TextSanitizer::CenterOriginHorizontally(HPName);
	HPName->setPosition(384, 188);
	UnitHP.Name = std::shared_ptr<sf::Text>(HPName);

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 512));

	Snapshot = nullptr;

	MustUnload = true;
	UnitExpGainDone = false;
	SupportExpGainDone = false;

	ShockWaveShaderAttacker = new ShockWave(sf::Vector2f(0, 0));
	ShockWaveShaderDefender = new ShockWave(sf::Vector2f(0, 0));
	MindBendShader = new MindBend();
	BattleTransitionShader = new BattleTransition(true, sf::Vector2f(512, 512));

	MustMindwave = false;
	TransitioningBack = false;
}


BattleScreen::~BattleScreen()
{
	SafeDelete(Platform);
	SafeDelete(Background);
}


void BattleScreen::Draw()
{
	pCamera->DrawUpdate();
	
	if(TransitioningBack)
	{
		pWindow->draw(*Snapshot, BattleTransitionShader->GetShader());
		pWindow->draw(*Background);
	}
	else
	{
		if (AttackerCrit)
		{
			pWindow->draw(*Scene, ShockWaveShaderDefender->GetShader());
		}
		else if (DefenderCrit)
		{
			pWindow->draw(*Scene, ShockWaveShaderAttacker->GetShader());
		}
		else if (MustMindwave)
		{
			pWindow->draw(*Scene, MindBendShader->GetShader());
		}
		else
		{
			pWindow->draw(*Scene);
		}

		pWindow->draw(*Background);

		Platform->setPosition(40, 195);

		pWindow->draw(*Platform);
		pWindow->draw(*AttackerBattleSprite);
		pWindow->draw(*AttackerAttackSprite);

		Platform->setPosition(318, 195);

		pWindow->draw(*Platform);
		pWindow->draw(*DefenderBattleSprite);
		pWindow->draw(*DefenderAttackSprite);

		if (AttackerSupport != nullptr)
		{
			pWindow->draw(*SupportBlue);
			pWindow->draw(*AttackerSupport);
		}
		if (DefenderSupport != nullptr)
		{
			pWindow->draw(*SupportRed);
			pWindow->draw(*DefenderSupport);
		}

		if (CurrentPhase == ATTACK || CurrentPhase == COUNTERATTACK)
		{
			if (AttackerMiss || DefenderMiss)
			{
				pWindow->draw(*MissSprite);
			}
			if (AttackerCrit || DefenderCrit)
			{
				pWindow->draw(*CritSprite);
			}
		}

		if (CurrentPhase == EXPSCREEN)
		{
			pWindow->draw(*EXPGained);

			pWindow->draw(*EXPScreenContainer);
			pWindow->draw(*UnitEXPPortrait);

			if (SupportEXPRecieved)
			{
				pWindow->draw(*EXPScreenContainerTwo);
				pWindow->draw(*UnitSupportEXPPortrait);

				pWindow->draw(*UnitSupportExpCounter);
				pWindow->draw(*UnitSupporGainExpCounter);
			}

			pWindow->draw(*UnitExpCounter);
			pWindow->draw(*UnitExpGainCounter);
		}

		if (CurrentPhase == LEVELUPUNIT)
		{
			pWindow->draw(*Vignette);

			pWindow->draw(*LevelUpOverlay);

			pWindow->draw(*UnitLevelUpPortrait);


			pWindow->draw(*UnitATK.Name);
			pWindow->draw(*UnitATK.Value);
			pWindow->draw(*UnitDEF.Name);
			pWindow->draw(*UnitDEF.Value);
			pWindow->draw(*UnitLCK.Name);
			pWindow->draw(*UnitLCK.Value);
			pWindow->draw(*UnitSPD.Name);
			pWindow->draw(*UnitSPD.Value);
			pWindow->draw(*UnitHP.Name);
			pWindow->draw(*UnitHP.Value);

			if (ATKGained > 0)
			{
				SkillIncrease->setPosition(348, 103);
				pWindow->draw(*SkillIncrease);
			}

			if (DEFGained > 0)
			{
				SkillIncrease->setPosition(348, 134);
				pWindow->draw(*SkillIncrease);
			}

			if (SPDGained > 0)
			{
				SkillIncrease->setPosition(430, 103);
				pWindow->draw(*SkillIncrease);
			}

			if (LCKGained > 0)
			{
				SkillIncrease->setPosition(430, 134);
				pWindow->draw(*SkillIncrease);
			}

			if (HPGained > 0)
			{
				SkillIncrease->setPosition(430, 166);
				pWindow->draw(*SkillIncrease);
			}

			pWindow->draw(*OldLevel);
			pWindow->draw(*NewLevel);
		}

		pWindow->draw(*HPOverlay);
		pWindow->draw(*AttackerHP);
		pWindow->draw(*DefenderHP);

		if (pDialogueOverlay->HasScene)
		{
			pDialogueOverlay->Draw();
		}
	}
}

void BattleScreen::Update()
{
	float DeltaTime = GameTime::DeltaTime;

	MindBendShader->Update();

	if(!pDialogueOverlay->HasScene)
	{
		if (CurrentPhase == INIT && Timer >= 1.5F)
		{
			CurrentPhase = ATTACK;
			HitColor.g = 0;
			HitColor.b = 0;
			Timer = 0;

			//Did the initiator hit or miss?
			AttackerMiss = false;
			if (!BattleHelper::WillHit(pBattleOutcome->AttackerHitChance))
			{
				//Nope, that's a miss.
				AttackerMiss = true;
				MissSprite->setColor(sf::Color(255, 255, 255, 255));
				MissSprite->setPosition(366, 40);
			}
			//He hit!
			//Inflict damage.
			else
			{
				AttackerCrit = false;
				if (BattleHelper::WillCrit(pBattleOutcome->AttackerCritChance))
				{
					AttackerCrit = true;
					pBattleOutcome->Defender->InflictDamage(pBattleOutcome->AttackerDamage * 2);
					CritSprite->setColor(sf::Color(255, 255, 255, 255));
					CritSprite->setPosition(347, 40);
					AlphaTimer = 255;
					Shaker = SHAKEMAGNITUDE + 16;

					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CritSound, false);
				}
				else
				{
					pBattleOutcome->Defender->InflictDamage(pBattleOutcome->AttackerDamage);
					Shaker = SHAKEMAGNITUDE;

					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->HitSound, false);
				}

				if (pBattleOutcome->Defender->IsDead)
				{
					DefenderDied = true;
				}

				DefenderHP->setString(std::to_string(pBattleOutcome->Defender->getHP()));
				TextSanitizer::CenterOrigin(DefenderHP);
				DefenderHP->setPosition(458, 232);
				DefenderBattleSprite->setColor(HitColor);
			}
		}
		else if (CurrentPhase == ATTACK)
		{
			if (AttackerMiss)
			{
				float PosY = MissSprite->getPosition().y - (20 * DeltaTime);
				MissSprite->setPosition(MissSprite->getPosition().x, PosY);

				AlphaTimer -= 125 * DeltaTime;
				if (AlphaTimer > 0)
				{
					float Alpha = MissSprite->getColor().a - AlphaTimer;
					sf::Color CurrentColor(MissSprite->getColor());
					CurrentColor.a = AlphaTimer;

					MissSprite->setColor(CurrentColor);
				}
			}
			else
			{
				ColorTimer += 100 * DeltaTime;
				if (ColorTimer < 255)
				{
					HitColor.g = ColorTimer;
					HitColor.b = ColorTimer;
					DefenderBattleSprite->setColor(HitColor);
					DefenderHP->setColor(HitColor);
				}

				DefenderBattleSprite->setPosition(DefenderOriginalPos.x + Shaker, DefenderOriginalPos.y);
				Shaker = -Shaker;

				if (Shaker > 0)
				{
					Shaker -= 25 * DeltaTime;
				}
				else
				{
					Shaker += 25 * DeltaTime;
				}

				if (AttackerCrit)
				{
					float PosY = CritSprite->getPosition().y - (20 * DeltaTime);
					CritSprite->setPosition(CritSprite->getPosition().x, PosY);

					AlphaTimer -= 125 * DeltaTime;
					if (AlphaTimer > 0)
					{
						float Alpha = CritSprite->getColor().a - AlphaTimer;
						sf::Color CurrentColor(CritSprite->getColor());
						CurrentColor.a = AlphaTimer;

						CritSprite->setColor(CurrentColor);
					}

					ShockWaveShaderDefender->Update();
				}
			}

			if (Timer >= 3.0)
			{
				if (!DefenderDied)
				{
					if (!pBattleOutcome->WillCounter)
					{
						CurrentPhase = POSTBATTLE;
						DefenderBattleSprite->setColor(sf::Color(255, 255, 255, 255));
						Timer = 0;
					}
					else
					{
						CurrentPhase = COUNTERATTACK;
						HitColor.g = 0;
						HitColor.b = 0;
						DefenderBattleSprite->setPosition(DefenderOriginalPos);
						DefenderBattleSprite->setColor(sf::Color(255, 255, 255, 255));
						ColorTimer = 0;
						Timer = 0;

						//Did the defender hit or miss?
						DefenderMiss = false;
						//Nope, that's a miss.
						if (!BattleHelper::WillHit(pBattleOutcome->DefenderHitChance))
						{
							DefenderMiss = true;
							MissSprite->setColor(sf::Color(255, 255, 255, 255));
							MissSprite->setPosition(94, 40);
							AlphaTimer = 255;
						}
						//He hit!
						//Inflict damage.
						else
						{
							DefenderCrit = false;
							if (BattleHelper::WillCrit(pBattleOutcome->DefenedrCritChance))
							{
								DefenderCrit = true;
								pBattleOutcome->Attacker->InflictDamage(pBattleOutcome->DefenderDamage * 2);
								CritSprite->setColor(sf::Color(255, 255, 255, 255));
								CritSprite->setPosition(77, 40);
								AlphaTimer = 255;
								Shaker = SHAKEMAGNITUDE + 16;

								SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CritSound, false);
							}
							else
							{
								pBattleOutcome->Attacker->InflictDamage(pBattleOutcome->DefenderDamage);
								Shaker = SHAKEMAGNITUDE;

								SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->HitSound, false);
							}

							if (pBattleOutcome->Attacker->IsDead)
							{
								AttackerDied = true;
							}

							AttackerHP->setString(std::to_string(pBattleOutcome->Attacker->getHP()));
							TextSanitizer::CenterOrigin(AttackerHP);
							AttackerHP->setPosition(55, 232);
							AttackerBattleSprite->setColor(HitColor);
						}
					}
				}
				else
				{
					CurrentPhase = DEAD;
					Timer = 0;
					AlphaTimer = 255;
					pBattleOutcome->Defender->OnDefeated(pBattleOutcome->Attacker->getID());
				}
			}
		}
		else if (CurrentPhase == COUNTERATTACK)
		{
			if (DefenderMiss)
			{
				float PosY = MissSprite->getPosition().y - (20 * DeltaTime);
				MissSprite->setPosition(MissSprite->getPosition().x, PosY);

				AlphaTimer -= 125 * DeltaTime;

				if (AlphaTimer > 0)
				{
					float Alpha = MissSprite->getColor().a - AlphaTimer;
					sf::Color CurrentColor(MissSprite->getColor());
					CurrentColor.a = AlphaTimer;

					MissSprite->setColor(CurrentColor);
				}
			}
			else
			{
				ColorTimer += 100 * DeltaTime;

				if (ColorTimer < 255)
				{
					HitColor.g = ColorTimer;
					HitColor.b = ColorTimer;
					AttackerBattleSprite->setColor(HitColor);
					AttackerHP->setColor(HitColor);
				}

				AttackerBattleSprite->setPosition(AttackerOriginalPos.x + Shaker, AttackerOriginalPos.y);
				Shaker = -Shaker;

				if (Shaker > 0)
				{
					Shaker -= 25 * DeltaTime;
				}
				else
				{
					Shaker += 25 * DeltaTime;
				}

				if (DefenderCrit)
				{
					float PosY = CritSprite->getPosition().y - (20 * DeltaTime);
					CritSprite->setPosition(CritSprite->getPosition().x, PosY);

					AlphaTimer -= 125 * DeltaTime;
					if (AlphaTimer > 0)
					{
						float Alpha = CritSprite->getColor().a - AlphaTimer;
						sf::Color CurrentColor(CritSprite->getColor());
						CurrentColor.a = AlphaTimer;

						CritSprite->setColor(CurrentColor);
					}

					ShockWaveShaderAttacker->Update();
				}
			}

			if (Timer >= 3.0)
			{
				if (AttackerDied)
				{
					CurrentPhase = DEAD;
					Timer = 0;
					AlphaTimer = 255;
					pBattleOutcome->Attacker->OnDefeated(pBattleOutcome->Defender->getID());
				}
				else
				{
					CurrentPhase = POSTBATTLE;
				}
				AttackerBattleSprite->setPosition(AttackerOriginalPos);
				AttackerBattleSprite->setColor(sf::Color(255, 255, 255, 255));
			}
		}
		else if (CurrentPhase == DEAD)
		{
			if (DefenderDied)
			{
				AlphaTimer -= 500 * DeltaTime;

				if (AlphaTimer > 0)
				{
					float Alpha = DefenderBattleSprite->getColor().a - AlphaTimer;
					sf::Color CurrentColor(DefenderBattleSprite->getColor());
					CurrentColor.a = AlphaTimer;

					DefenderBattleSprite->setColor(CurrentColor);
				}
			}
			else if (AttackerDied)
			{
				AlphaTimer -= 500 * DeltaTime;

				if (AlphaTimer > 0)
				{
					float Alpha = AttackerBattleSprite->getColor().a - AlphaTimer;
					sf::Color CurrentColor(AttackerBattleSprite->getColor());
					CurrentColor.a = AlphaTimer;

					AttackerBattleSprite->setColor(CurrentColor);
				}
			}
			if (Timer >= 3.0)
			{
				if (DefenderDied && pBattleOutcome->Defender->IsPlayerUnit() || AttackerDied && pBattleOutcome->Attacker->IsPlayerUnit())
				{
					CurrentPhase = END;
					Timer = 0;
				}
				else
				{
					CurrentPhase = POSTBATTLE;
					if (!AttackerDied)
					{
						AttackerBattleSprite->setPosition(AttackerOriginalPos);
						AttackerBattleSprite->setColor(sf::Color(255, 255, 255, 255));
					}

					if (!DefenderDied)
					{
						DefenderBattleSprite->setPosition(DefenderOriginalPos);
						DefenderBattleSprite->setColor(sf::Color(255, 255, 255, 255));
					}
				}
			}
		}
		else if (CurrentPhase == POSTBATTLE)
		{
			int EXP = 1;
			if (pBattleOutcome->Attacker->IsPlayerUnit())
			{
				if (!AttackerMiss)
				{
					int LevelDifference = pBattleOutcome->Defender->getLVL() - pBattleOutcome->Attacker->getLVL();

					int BOSSMODIFIER = 0;
					if (pBattleOutcome->Defender->IsBoss)
					{
						BOSSMODIFIER = 40;
					}

					int ATKEXP = 10 + (LevelDifference * 2) + BOSSMODIFIER / 4;
					EXP = ATKEXP;
					if (DefenderDied)
					{
						int KILLEXP = ATKEXP + (LevelDifference * 2) + 15 + BOSSMODIFIER;
						EXP = KILLEXP;

						if(ArenaMode)
						{
							pBattleOutcome->Attacker->AddArenaKill();
						}
					}

					if (pBattleOutcome->AttackerSupport != nullptr && pBattleOutcome->AttackerSupport->IsPlayerUnit())
					{
						UnitSupportEXPOld = pBattleOutcome->AttackerSupport->getEXP();
						UnitSupportEXPGain = EXP / 4;
						pBattleOutcome->AttackerSupport->giveEXP(UnitSupportEXPGain);
						UnitSupportGotEXP = true;

						SupportEXPRecieved = pBattleOutcome->AttackerSupport;

						UnitSupportEXPPortrait = new sf::Sprite(*pBattleOutcome->AttackerSupport->getCHAR()->SupportSprite->Normal);
						UnitSupportEXPPortrait->setPosition(88, 118);

						PlayerManager::GetInstance()->AddEnergyUnits((EXP + pBattleOutcome->AttackerSupport->getLCK() + BOSSMODIFIER) * 2);
					}

					PlayerManager::GetInstance()->AddEnergyUnits((EXP + pBattleOutcome->Attacker->getLCK() + BOSSMODIFIER) * 4);
				}
				UnitEXPOld = pBattleOutcome->Attacker->getEXP();
				UnitEXPGain = EXP;
				pBattleOutcome->Attacker->giveEXP(UnitEXPGain);
				UnitGotEXP = true;
				UnitEXPRecieved = pBattleOutcome->Attacker;

				CurrentPhase = EXPSCREEN;

				UnitEXPPortrait = new sf::Sprite(*pBattleOutcome->Attacker->getCHAR()->SupportSprite->Normal);
				UnitEXPPortrait->setPosition(88, 52);

				Timer = 0;
			}
			else if (pBattleOutcome->Defender->IsPlayerUnit())
			{
				if (!DefenderMiss)
				{
					int LevelDifference = pBattleOutcome->Attacker->getLVL() - pBattleOutcome->Defender->getLVL();

					int BOSSMODIFIER = 0;
					if (pBattleOutcome->Attacker->IsBoss)
					{
						BOSSMODIFIER = 40;
					}

					int ATKEXP = 10 + (LevelDifference * 2) + BOSSMODIFIER / 4;
					EXP = ATKEXP;
					if (AttackerDied)
					{
						int KILLEXP = ATKEXP + (LevelDifference * 2) + 15 + BOSSMODIFIER;
						EXP = KILLEXP;

						if (ArenaMode)
						{
							pBattleOutcome->Defender->AddArenaKill();
						}
					}

					if (pBattleOutcome->DefenderSupport != nullptr && pBattleOutcome->DefenderSupport->IsPlayerUnit())
					{
						UnitSupportEXPOld = pBattleOutcome->DefenderSupport->getEXP();
						UnitSupportEXPGain = EXP / 4;
						pBattleOutcome->DefenderSupport->giveEXP(UnitSupportEXPGain);
						UnitSupportGotEXP = true;

						SupportEXPRecieved = pBattleOutcome->DefenderSupport;

						UnitSupportEXPPortrait = new sf::Sprite(*pBattleOutcome->DefenderSupport->getCHAR()->SupportSprite->Normal);
						UnitSupportEXPPortrait->setPosition(88, 118);
						PlayerManager::GetInstance()->AddEnergyUnits((EXP + pBattleOutcome->DefenderSupport->getLCK() + BOSSMODIFIER) * 2);
					}

					PlayerManager::GetInstance()->AddEnergyUnits((EXP + pBattleOutcome->Defender->getLCK() + BOSSMODIFIER) * 4);
				}
				UnitEXPOld = pBattleOutcome->Defender->getEXP();
				UnitEXPGain = EXP;
				pBattleOutcome->Defender->giveEXP(UnitEXPGain);
				UnitGotEXP = true;
				UnitEXPRecieved = pBattleOutcome->Defender;

				CurrentPhase = EXPSCREEN;

				UnitEXPPortrait = new sf::Sprite(*pBattleOutcome->Defender->getCHAR()->SupportSprite->Normal);
				UnitEXPPortrait->setPosition(88, 52);

				Timer = 0;
			}
			else
			{
				CurrentPhase = END;
				Timer = 0;
			}
		}
		else if (CurrentPhase == EXPSCREEN)
		{
			if (UnitGotEXP && UnitEXPGain > 0)
			{
				UnitEXPGain -= 20 * GameTime::DeltaTime;
				UnitEXPOld += 20 * GameTime::DeltaTime;

				if (UnitEXPOld >= 100)
				{
					UnitEXPOld = 0;
					UnitLevelsGained += 1;
				}

				UnitExpCounter->setString(std::to_string(static_cast<int>(UnitEXPOld)));
				UnitExpGainCounter->setString(std::to_string(static_cast<int>(UnitEXPGain)));

				if (static_cast<int>(UnitEXPGain) <= 0 && !UnitExpGainDone)
				{
					UnitExpGainDone = true;
					Timer = 0;
				}
			}
			else if (!UnitGotEXP)
			{
				UnitExpGainDone = true;
			}

			if (UnitSupportGotEXP && UnitSupportEXPGain > 0)
			{
				UnitSupportEXPGain -= 20 * GameTime::DeltaTime;
				UnitSupportEXPOld += 20 * GameTime::DeltaTime;

				if (UnitSupportEXPOld >= 100)
				{
					UnitSupportEXPOld = 0;
					SupportLevelsGained += 1;
				}

				UnitSupportExpCounter->setString(std::to_string(static_cast<int>(UnitSupportEXPOld)));
				UnitSupporGainExpCounter->setString(std::to_string(static_cast<int>(UnitSupportEXPGain)));


				if (static_cast<int>(UnitSupportEXPGain) <= 0 && !UnitExpGainDone)
				{
					SupportExpGainDone = true;
					Timer = 0;
				}
			}
			else if (!UnitSupportGotEXP)
			{
				SupportExpGainDone = true;
			}

			if (Timer >= 1.5 && UnitExpGainDone && SupportExpGainDone || Timer >= 1.5 && UnitEXPGain == 0)
			{
				if (UnitLevelsGained > 0 || SupportLevelsGained > 0)
				{
					CurrentPhase = LEVELUP;
				}
				else
				{
					CurrentPhase = END;
				}
			}
		}
		else if (CurrentPhase == LEVELUP)
		{
			ResetGains();
			if (UnitLevelsGained > 0)
			{
				LevelUp(UnitEXPRecieved);
				UnitLevelsGained -= 1;
				Timer = 0;
			}
			else if (SupportLevelsGained > 0)
			{
				LevelUp(SupportEXPRecieved);
				SupportLevelsGained -= 1;
				Timer = 0;
			}
			else
			{
				CurrentPhase = END;
			}
		}
		else if (CurrentPhase == LEVELUPUNIT)
		{

			if (MovementSpeed > 60)
			{
				MovementSpeed -= 500 * DeltaTime;
			}

			if (UnitLevelUpPortrait->getPosition().x < 0)
			{
				UnitLevelUpPortrait->setPosition(UnitLevelUpPortrait->getPosition().x + MovementSpeed * DeltaTime, 0);
			}
			else
			{
				UnitLevelUpPortrait->setPosition(0, 0);
			}

			if (Timer >= 4.0F)
			{
				CurrentPhase = LEVELUP;
			}
		}
		else if (CurrentPhase == END)
		{
			if(!TransitioningBack)
			{
				SafeDelete(Snapshot);

				Snapshot = new sf::Sprite(*GetSnapshot());

				BattleTransitionShader->SetSecondTexture(ScreenManager::GetInstance()->GetLevelScreen()->GetSnapshot());
				BattleTransitionShader->Reset();
				TransitioningBack = true;
				Timer = 0;

				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->BattleTransition, false);
			}
			else
			{
				BattleTransitionShader->Update();
				if(Timer >= 1.75)
				{
					ScreenManager::GetInstance()->GetLevelScreen()->EndBattle(pBattleOutcome->Attacker);
					ScreenManager::GetInstance()->CloseScreen();		
				}
			}
		}

		Timer += DeltaTime;
	}
	else
	{
		pDialogueOverlay->Update();
	}
}

void BattleScreen::LevelUp(Unit* _pUnit)
{
	boost::random::mt19937 Generator;
	Generator.seed(static_cast<unsigned int>(std::time(0)));
	boost::random::uniform_int_distribution<> Chance(1, 100);

	int ATK = _pUnit->getCLASS()->GetATKGrowth() + _pUnit->getCHAR()->GetATKGrowth();
	if (Chance(Generator) < ATK)
	{
		ATKGained += 1;
		_pUnit->addATK(ATKGained);
	}

	sf::Text* ATKVal = new sf::Text();

	ATKVal->setCharacterSize(16);
	ATKVal->setFont(*ResourceManager::GetInstance()->GetAceFont());
	ATKVal->setString(std::to_string(_pUnit->getATK()));
	TextSanitizer::CenterOriginHorizontally(ATKVal);
	ATKVal->setPosition(332, 125);
	UnitATK.Value = std::shared_ptr<sf::Text>(ATKVal);

	int DEF = _pUnit->getCLASS()->GetDEFGrowth() + _pUnit->getCHAR()->GetDEFGrowth();
	if (Chance(Generator) < DEF)
	{
		DEFGained += 1;
		_pUnit->addDEF(DEFGained);
	}

	sf::Text* DEFVal = new sf::Text();

	DEFVal->setCharacterSize(16);
	DEFVal->setFont(*ResourceManager::GetInstance()->GetAceFont());
	DEFVal->setString(std::to_string(_pUnit->getDEF()));
	TextSanitizer::CenterOriginHorizontally(DEFVal);
	DEFVal->setPosition(332, 156);
	UnitDEF.Value = std::shared_ptr<sf::Text>(DEFVal);

	int SPD = _pUnit->getCLASS()->GetSPDGrowth() + _pUnit->getCHAR()->GetSPDGrowth();
	if (Chance(Generator) < SPD)
	{
		SPDGained += 1;
		_pUnit->addSPD(SPDGained);
	}

	Stat SPDStat;
	sf::Text* SPDVal = new sf::Text();

	SPDVal->setCharacterSize(16);
	SPDVal->setFont(*ResourceManager::GetInstance()->GetAceFont());
	SPDVal->setString(std::to_string(_pUnit->getSPD()));
	TextSanitizer::CenterOriginHorizontally(SPDVal);
	SPDVal->setPosition(416, 156);
	UnitSPD.Value = std::shared_ptr<sf::Text>(SPDVal);

	int LCK = _pUnit->getCLASS()->GetLCKGrowth() + _pUnit->getCHAR()->GetLCKGrowth();
	if (Chance(Generator) < LCK)
	{
		LCKGained += 1;
		_pUnit->addLCK(LCKGained);
	}

	sf::Text* LCKVal = new sf::Text();

	LCKVal->setCharacterSize(16);
	LCKVal->setFont(*ResourceManager::GetInstance()->GetAceFont());
	LCKVal->setString(std::to_string(_pUnit->getLCK()));
	TextSanitizer::CenterOriginHorizontally(LCKVal);
	LCKVal->setPosition(416, 125);
	UnitLCK.Value = std::shared_ptr<sf::Text>(LCKVal);

	int HP = _pUnit->getCLASS()->GetHPGrowth() + _pUnit->getCHAR()->GetHPGrowth();
	if (Chance(Generator) < LCK)
	{
		HPGained += 1;
		_pUnit->addMAXHP(HPGained);
	}

	sf::Text* HPVal = new sf::Text();

	HPVal->setCharacterSize(16);
	HPVal->setFont(*ResourceManager::GetInstance()->GetAceFont());
	HPVal->setString(std::to_string(_pUnit->getMAXHP()));
	TextSanitizer::CenterOriginHorizontally(HPVal);
	HPVal->setPosition(416, 188);
	UnitHP.Value = std::shared_ptr<sf::Text>(HPVal);

	OldLevel->setString(std::to_string(_pUnit->getLVL()));
	TextSanitizer::CenterOrigin(OldLevel);
	OldLevel->setPosition(310, 61);

	_pUnit->addLVL(1);

	NewLevel->setString(std::to_string(_pUnit->getLVL()));
	TextSanitizer::CenterOrigin(NewLevel);
	NewLevel->setPosition(419, 61);

	sf::Sprite* UnitPortrait = new sf::Sprite(*_pUnit->getCHAR()->DialougeSprite->Normal);
	UnitPortrait->setPosition(-284, 0);
	UnitLevelUpPortrait = std::shared_ptr<sf::Sprite>(UnitPortrait);

	CurrentPhase = LEVELUPUNIT;
	MovementSpeed = MOVEMENTSPEED;
}

void BattleScreen::ResetGains()
{
	ATKGained = 0;
	DEFGained = 0;
	SPDGained = 0;
	LCKGained = 0;
	HPGained = 0;
}

void BattleScreen::Unload()
{
	SafeDelete(AttackerAttackSprite);
	SafeDelete(AttackerBattleSprite);
	SafeDelete(DefenderAttackSprite);
	SafeDelete(DefenderBattleSprite);
	SafeDelete(AttackerSupport);
	SafeDelete(DefenderSupport);
	SafeDelete(UnitEXPPortrait);
	SafeDelete(UnitSupportEXPPortrait);

	AttackerDied = false;
	DefenderDied = false;

	Timer = 0;
	Shaker = 0;
	ColorTimer = 0;

	ArenaMode = false;

	UnitEXPOld = 0;
	UnitSupportEXPOld = 0;
	UnitEXPGain = 0;
	UnitSupportEXPGain = 0;

	UnitGotEXP = 0;
	UnitSupportGotEXP = 0;

	UnitEXPRecieved = nullptr;
	SupportEXPRecieved = nullptr;

	AttackerMiss = false;
	AttackerCrit = false;
	AttackerDied = false;

	DefenderMiss = false;
	DefenderCrit = false;
	DefenderDied = false;

	UnitExpGainDone = false;
	SupportExpGainDone = false;
	TransitioningBack = false;
}

void BattleScreen::InitBattle(BattleOutcome* _pBattle, std::string _scene, bool _ArenaMode)
{
	pBattleOutcome = _pBattle;
	ArenaMode = _ArenaMode; 

	AttackerBattleSprite = new sf::Sprite(*_pBattle->Attacker->getCHAR()->BattleSprite->Normal);
	AttackerAttackSprite = new sf::Sprite(*_pBattle->Attacker->getCHAR()->AttackSprite->Normal);

	int AttackerCharX = AttackerBattleSprite->getTexture()->getSize().x;
	int AttackerCharY = AttackerBattleSprite->getTexture()->getSize().y;

	AttackerBattleSprite->setOrigin(AttackerCharX / 2, 0);
	AttackerBattleSprite->setPosition(112, 209 - AttackerCharY);
	AttackerAttackSprite->setPosition(0, 296);

	DefenderBattleSprite = new sf::Sprite(*_pBattle->Defender->getCHAR()->BattleSprite->Reversed);
	DefenderAttackSprite = new sf::Sprite(*_pBattle->Defender->getCHAR()->AttackSprite->Reversed);

	int DefenderCharX = DefenderBattleSprite->getTexture()->getSize().x;
	int DefenderCharY = DefenderBattleSprite->getTexture()->getSize().y;

	DefenderBattleSprite->setOrigin(DefenderCharX / 2, 0);
	DefenderBattleSprite->setPosition(391, 209 - DefenderCharY);
	DefenderAttackSprite->setPosition(256, 296);

	if (_pBattle->AttackerSupport != nullptr)
	{
		Character* AttackerSupportChar = _pBattle->AttackerSupport->getCHAR();

		AttackerSupportChar->SupportSprite->Normal->setPosition(0, 400);

		AttackerSupport = new sf::Sprite(*_pBattle->AttackerSupport->getCHAR()->SupportSprite->Normal);
	}

	if (_pBattle->DefenderSupport != nullptr)
	{
		Character* DefenderSupportChar = _pBattle->DefenderSupport->getCHAR();

		DefenderSupportChar->SupportSprite->Reversed->setPosition(343, 400);

		DefenderSupport = new sf::Sprite(*_pBattle->DefenderSupport->getCHAR()->SupportSprite->Reversed);
	}

	Platform = ResourceManager::GetInstance()->GetPlatform(_scene);
	Scene = ResourceManager::GetInstance()->GetBackground(_scene);

	DefenderOriginalPos = DefenderBattleSprite->getPosition();
	AttackerOriginalPos = AttackerBattleSprite->getPosition();

	AttackerHP->setString(std::to_string(pBattleOutcome->Attacker->getHP()));
	DefenderHP->setString(std::to_string(pBattleOutcome->Defender->getHP()));

	TextSanitizer::CenterOrigin(AttackerHP);
	TextSanitizer::CenterOrigin(DefenderHP);

	AttackerHP->setPosition(56, 232);
	DefenderHP->setPosition(458, 232);

	CurrentPhase = INIT;
	HitColor.r = 255;
	AlphaTimer = 255;

	float x = (AttackerBattleSprite->getPosition().x) / 512;
	float y = (AttackerBattleSprite->getPosition().y + AttackerBattleSprite->getTexture()->getSize().y / 2) / 256;

	ShockWaveShaderAttacker->SetPosition(sf::Vector2f(x, y));
	ShockWaveShaderAttacker->Reset();

	x = (DefenderBattleSprite->getPosition().x) / 512;
	y = (DefenderBattleSprite->getPosition().y + DefenderBattleSprite->getTexture()->getSize().y / 2) / 256;

	ShockWaveShaderDefender->SetPosition(sf::Vector2f(x, y));
	ShockWaveShaderDefender->Reset();

	_pBattle->Defender->OnEngaged(_pBattle->Attacker->getID());
}

sf::Texture* BattleScreen::GetSnapshot()
{
	sf::RenderTexture RenderTexture;
	RenderTexture.create(512, 256);
	RenderTexture.draw(*Scene);

	pWindow->draw(*Background);

	Platform->setPosition(40, 195);

	RenderTexture.draw(*Platform);
	RenderTexture.draw(*AttackerBattleSprite);
	RenderTexture.draw(*AttackerAttackSprite);

	Platform->setPosition(318, 195);

	RenderTexture.draw(*Platform);
	RenderTexture.draw(*DefenderBattleSprite);
	RenderTexture.draw(*DefenderAttackSprite);
	RenderTexture.display();

	sf::Texture* ScreenShot = new sf::Texture(RenderTexture.getTexture());
	return ScreenShot;
}

void BattleScreen::PollInput()
{
	sf::Event event;

	while (pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			CloseGame = true;
		}
	}

	if(pDialogueOverlay->HasScene)
	{
		pDialogueOverlay->PollInput();
	}
}
