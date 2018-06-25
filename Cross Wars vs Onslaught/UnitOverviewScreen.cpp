#include "UnitOverviewScreen.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "UnitSkill.h"
#include "TextSanitizer.h"
#include "ScreenManager.h"
#include "MusicPlayer.h"

UnitOverviewScreen::UnitOverviewScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pUnitOverviewScreen = new BattleCharScreen(_pRenderWindow);
	pItemActionMenu = new ItemActionMenu(_pRenderWindow, ItemActionMenu::ININVENTORY);
	pItemActionMenu->AddEventOnSendToStorage<UnitOverviewScreen>(&UnitOverviewScreen::ItemSendToStorage, this);
	pItemActionMenu->AddEventOnItemUse<UnitOverviewScreen>(&UnitOverviewScreen::ItemUse, this);
	pItemActionMenu->AddEventOnItemDelete<UnitOverviewScreen>(&UnitOverviewScreen::ItemDelete, this);

	pCharCursor = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overview\\charcursor.png", Folders::HUD));
	pCharCursor->setPosition(47, 42);
	pItemCursor = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overview\\itemcursor.png", Folders::HUD));
	pSkillCursor = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overview\\skillcursor.png", Folders::HUD));

	pBackground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overview\\charimagecontainer.png", Folders::HUD));
	pForeground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overview\\infobox.png", Folders::HUD));
	pForeground->setPosition(197, 0);

	pCamera = new Camera(sf::FloatRect(0, 0, 512, 256));
	pCamera->setViewport(sf::FloatRect(0, 0, 1, 0.5f));

	pInfoContainer = new TextContainer(_pRenderWindow, "", ResourceManager::GetInstance()->GetAceFont(), sf::Vector2f(230, 34), 254);
	pHeaderText = new sf::Text();
	pHeaderText->setFont(*ResourceManager::GetInstance()->GetAceFont());
	pHeaderText->setStyle(sf::Text::Bold);
	pHeaderText->setCharacterSize(18);
}


UnitOverviewScreen::~UnitOverviewScreen()
{
}

void UnitOverviewScreen::Start()
{
	pItemActionMenu->Hide();
}

void UnitOverviewScreen::Draw()
{
	pCamera->DrawUpdate();

	pWindow->draw(*pBackground);
	pWindow->draw(*pCurrentUnit->getCHAR()->DialougeSprite->Normal);
	pWindow->draw(*pForeground);

	pWindow->draw(*pHeaderText);
	pInfoContainer->Draw();

	pUnitOverviewScreen->Draw();

	pWindow->draw(*pCursor);

	if(!pItemActionMenu->CloseScreen)
	{
		pItemActionMenu->Draw();
	}
}

void UnitOverviewScreen::Unload()
{

}

void UnitOverviewScreen::Update()
{
	if (!pItemActionMenu->CloseScreen)
	{
		pItemActionMenu->Update();
	}
}

void UnitOverviewScreen::SetUnit(Unit* _pUnit)
{
	pCurrentUnit = _pUnit;
	pUnitOverviewScreen->ChangeUnit(_pUnit);
	pCursor = pCharCursor;
	CurrentState = CHAR;

	pItemCursor->setPosition(205, 74);
	pSkillCursor->setPosition(89, 150);

	SanitizedCharacterDescription = pInfoContainer->WordWrap(TextSanitizer::SanitizeString(pCurrentUnit->getCHAR()->GetDescription()), pInfoContainer->ContainerWidth);
	ShowUnit();

	SkillIndex = 1;
	ItemIndex = 1;

	if(_pUnit->getALLIANCE() == Unit::PLAYER)
	{
		CanRearrangeItems = true;
	}
	else
	{
		CanRearrangeItems = false;
	}
}

void UnitOverviewScreen::ShowSkill(UnitSkill* _pSkill)
{
	pInfoContainer->SetText(TextSanitizer::SanitizeString(_pSkill->GetDescription()), false);
	pHeaderText->setString(_pSkill->GetName());
	TextSanitizer::CenterLeftAndTop(pHeaderText);
	pHeaderText->setPosition(229, 4);
}

void UnitOverviewScreen::ShowItem(Item* _pItem)
{
	pInfoContainer->SetText(_pItem->getDescription(), false);
	pHeaderText->setString(_pItem->getName());
	TextSanitizer::CenterLeftAndTop(pHeaderText);
	pHeaderText->setPosition(229, 4);
}

void UnitOverviewScreen::ShowUnit()
{
	pInfoContainer->SetText(SanitizedCharacterDescription, true);
	pHeaderText->setString(pCurrentUnit->getCHAR()->GetNAME());
	TextSanitizer::CenterLeftAndTop(pHeaderText);
	pHeaderText->setPosition(229, 4);
}

void UnitOverviewScreen::ItemSendToStorage()
{
	pUnitOverviewScreen->InitInventory();
	ReadjustCounter();
}

void UnitOverviewScreen::ItemUse()
{
	Useable* pItem = static_cast<Useable*>(pCurrentUnit->getItemFromInventory(ItemIndex - 1));
	pItem->Use(pCurrentUnit);
	pCurrentUnit->RemoveItemFromInventory(pItem, false);

	pUnitOverviewScreen->ChangeUnit(pCurrentUnit);

	ReadjustCounter();

	ExternOnItemUse(nullptr);
	ExternOnItemUse.Clear();
}

void UnitOverviewScreen::ItemDelete()
{
	pUnitOverviewScreen->InitInventory();
	ReadjustCounter();
}

void UnitOverviewScreen::ReadjustCounter()
{
	if (ItemIndex > pCurrentUnit->getPointerToInventory()->size())
	{
		ItemIndex = pCurrentUnit->getPointerToInventory()->size();
		if (ItemIndex == 0)
		{
			if (pCurrentUnit->getSkills().size() > 0)
			{
				CurrentState = SKILLS;
				pCursor = pSkillCursor;
				SkillIndex = pCurrentUnit->getSkills().size();
				UnitSkill* pSkill = pCurrentUnit->getSkills().at(SkillIndex - 1);
				pCursor->setPosition(pSkill->SkillSprite->getPosition().x - 6, pSkill->SkillSprite->getPosition().y - 6);
				ShowSkill(pSkill);
			}
			else
			{
				CurrentState = CHAR;
				pCursor = pCharCursor;
				ShowUnit();
			}
		}
		else
		{
			pCursor->setPosition(205, 74 + (28 * (ItemIndex - 1)));
			ShowItem(pCurrentUnit->getItemFromInventory(ItemIndex - 1));
		}
	}
}

void UnitOverviewScreen::PollInput()
{
	sf::Event event;

	if(!pItemActionMenu->CloseScreen)
	{
		pItemActionMenu->PollInput();
	}
	else
	{
		while (pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				CloseGame = true;
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Down)
				{
					switch (CurrentState)
					{
					case CHAR:
						if (pCurrentUnit->getSkills().size() > 0)
						{
							CurrentState = SKILLS;
							pCursor = pSkillCursor;
							ShowSkill(pCurrentUnit->getSkills().at(0));
						}
						else if (pCurrentUnit->getInventory().size() > 0)
						{
							CurrentState = ITEMS;
							pCursor = pItemCursor;
							ShowItem(pCurrentUnit->getInventory().at(0));
						}
						break;
					case ITEMS:
						if (pCurrentUnit->getInventory().size() >= ItemIndex + 1)
						{
							ItemIndex += 1;
							pCursor->setPosition(205, 74 + (28 * (ItemIndex - 1)));
							ShowItem(pCurrentUnit->getItemFromInventory(ItemIndex - 1));
						}
						break;
					case SKILLS:
						if (pCurrentUnit->getSkills().size() >= SkillIndex + 1)
						{
							SkillIndex += 1;
							UnitSkill* pSkill = pCurrentUnit->getSkills().at(SkillIndex - 1);
							pCursor->setPosition(pSkill->SkillSprite->getPosition().x - 6, pSkill->SkillSprite->getPosition().y - 6);
							ShowSkill(pSkill);
						}
						else
						{
							if (pCurrentUnit->getInventory().size() > 0)
							{
								CurrentState = ITEMS;
								pCursor = pItemCursor;
								ShowItem(pCurrentUnit->getItemFromInventory(0));
							}
						}
						break;
					}

					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					switch (CurrentState)
					{
					case ITEMS:
						if (ItemIndex - 1 > 0)
						{
							ItemIndex -= 1;
							pCursor->setPosition(205, 74 + (28 * (ItemIndex - 1)));
							ShowItem(pCurrentUnit->getItemFromInventory(ItemIndex - 1));
						}
						else
						{
							if (pCurrentUnit->getSkills().size() > 0)
							{
								CurrentState = SKILLS;
								pCursor = pSkillCursor;
								ShowSkill(pCurrentUnit->getSkills().at(pCurrentUnit->getSkills().size() - 1));
							}
							else
							{
								CurrentState = CHAR;
								pCursor = pCharCursor;
								ShowUnit();
							}
						}
						break;
					case SKILLS:
						if (SkillIndex - 1 > 0)
						{
							SkillIndex -= 1;
							UnitSkill* pSkill = pCurrentUnit->getSkills().at(SkillIndex - 1);
							pCursor->setPosition(pSkill->SkillSprite->getPosition().x - 6, pSkill->SkillSprite->getPosition().y - 6);
							ShowSkill(pSkill);
						}
						else
						{
							CurrentState = CHAR;
							pCursor = pCharCursor;
							ShowUnit();
						}
						break;
					}

					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					if (CurrentState == ITEMS)
					{
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
						pItemActionMenu->SetUnit(pCurrentUnit);
						pItemActionMenu->SetItem(pCurrentUnit->getItemFromInventory(ItemIndex - 1));
						pItemActionMenu->Load();
						/*if (!pSelectedItem)
						{
						pSelectedItem = pCurrentUnit->getItemFromInventory(ItemIndex - 1);
						SelectedItemIndex = ItemIndex - 1;
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
						}
						else
						{
						Item* pOtherSelectedItem = pCurrentUnit->getItemFromInventory(ItemIndex - 1);
						std::vector<Item*>* Inventory = pCurrentUnit->getPointerToInventory();
						if (pSelectedItem->getGUID() != pOtherSelectedItem->getGUID())
						{
						iter_swap(Inventory->begin() + SelectedItemIndex, Inventory->begin() + ItemIndex - 1);

						pSelectedItem = nullptr;
						SelectedItemIndex = 0;

						pUnitOverviewScreen->InitInventory();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
						}
						}*/
					}
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
				}
				else if (event.key.code == sf::Keyboard::X)
				{
					pSelectedItem = nullptr;
					ScreenManager::GetInstance()->CloseScreen();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
				}
			}
		}
	}
}
