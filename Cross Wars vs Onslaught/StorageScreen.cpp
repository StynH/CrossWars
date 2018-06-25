#include "StorageScreen.h"
#include "ScreenManager.h"
#include "MusicPlayer.h"
#include "TextSanitizer.h"

StorageScreen::StorageScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pItemActionMenu = new ItemActionMenu(_pRenderWindow, ItemActionMenu::INSTORAGE);
	pItemActionMenu->AddEventOnSendToUnit<StorageScreen>(&StorageScreen::RefreshStorage, this);
	pItemActionMenu->AddEventOnItemDelete<StorageScreen>(&StorageScreen::RefreshStorage, this);

	pItemActionMenu->AddEventOnItemUse<StorageScreen>(&StorageScreen::SelectUnitToUseItemOn, this);
	pItemActionMenu->AddEventOnItemUse<StorageScreen>(&StorageScreen::RefreshStorage, this);

	pUnitSelectScreen = new UnitSelectScreen(_pRenderWindow);
	pUnitSelectScreen->SetCallback<StorageScreen>(&StorageScreen::UseItemOn, this);

	pBackground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("storage\\background.png", Folders::HUD));

	pContainer = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("storage\\container.png", Folders::HUD));
	pContainer->setPosition(30, 78);

	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("itemcreation\\itemcursor.png", Folders::HUD));

	Offset = 0;
	ItemCounter = 0;

	MustUnload = true;
}


StorageScreen::~StorageScreen()
{
}

void StorageScreen::UpdateCursorPosition()
{
	if(StoredItems.size() > 0)
	{
		pCursorSprite->setPosition(StoredItems.at(ItemCounter)->pContainerSprite->getPosition().x, StoredItems.at(ItemCounter)->pContainerSprite->getPosition().y - 5);
	}
}

void StorageScreen::SelectUnitToUseItemOn()
{
	pUnitSelectScreen->Load();
	pCurrentOverlay = pUnitSelectScreen;
}

void StorageScreen::UseItemOn(Unit* _pUnit)
{
	pUnitSelectScreen->Hide();
	Useable* Item = static_cast<Useable*>(ItemManager::GetInstance()->GetItemFromStorage(StoredItems.at(ItemCounter)->ItemID));
	Item->Use(_pUnit);

	RefreshStorage();

	pCurrentOverlay = nullptr;
}

void StorageScreen::Start()
{
	Offset = 0;
	ItemCounter = 0;
	pItemActionMenu->Hide();
	InitStorage();
	UpdateCursorPosition();

	pCurrentOverlay = nullptr;
}

void StorageScreen::Draw()
{
	pWindow->draw(*pBackground);
	pWindow->draw(*pContainer);

	int Start = 20 * Offset;
	for (int i = Start; i < Start + 20; ++i)
	{
		if (StoredItems.size() < i + 1)
		{
			break;
		}

		StoredItem* pItem = StoredItems.at(i);
		pWindow->draw(*pItem->pContainerSprite);
		pWindow->draw(*pItem->pItemSprite);
		pWindow->draw(*pItem->pItemText);
	}

	if (StoredItems.size() > 0)
	{
		pWindow->draw(*pCursorSprite);
	}

	if(pCurrentOverlay)
	{
		pCurrentOverlay->Draw();
	}
}

void StorageScreen::Update()
{
	if (pCurrentOverlay && !pCurrentOverlay->CloseScreen)
	{
		pCurrentOverlay->Update();
	}
	else if(pCurrentOverlay && pCurrentOverlay->CloseScreen)
	{
		pCurrentOverlay = nullptr;
	}
}

void StorageScreen::RefreshStorage()
{
	EmptyStorageStructs();
	InitStorage();
	ReadjustCounter();
	UpdateCursorPosition();
}

void StorageScreen::Unload()
{
	EmptyStorageStructs();
}

void StorageScreen::InitStorage()
{
	int XCounter = 0;
	int YCounter = 0;

	sf::Font* pFont = ResourceManager::GetInstance()->GetAceFont();
	for(Item* pItem : ItemManager::GetInstance()->GetItemStorage())
	{
		StoredItem* NewItem = new StoredItem();

		NewItem->pContainerSprite = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("itemcreation\\item_slot.png", Folders::HUD));
		NewItem->pContainerSprite->setPosition(51 + (213 * XCounter), 98 + (35 * YCounter));

		NewItem->pItemText = std::make_shared<sf::Text>();
		NewItem->pItemText->setString(pItem->getName());
		NewItem->pItemText->setCharacterSize(16);
		NewItem->pItemText->setFont(*pFont);
		TextSanitizer::CenterOriginHorizontallyLeft(NewItem->pItemText.get());
		NewItem->pItemText->setPosition(77 + (213 * XCounter), 110 + (35 * YCounter));

		NewItem->pItemSprite = std::make_shared<sf::Sprite>(*pItem->getTexture());
		NewItem->pItemSprite->setPosition(55 + (213 * XCounter), 102 + (35 * YCounter));

		NewItem->ItemID = pItem->getGUID();

		StoredItems.push_back(NewItem);

		if(XCounter == 1)
		{
			XCounter = 0;

			if(YCounter == 9)
			{
				YCounter = 0;
			}
			else
			{
				++YCounter;
			}
		}
		else
		{
			++XCounter;
		}
	}

	CanMoveUp = false;
	if(ItemManager::GetInstance()->GetStorageSize() > 20)
	{
		CanMoveDown = true;
	}
}

void StorageScreen::ReadjustCounter()
{
	if(StoredItems.size() < ItemCounter + 1 && StoredItems.size() > 0)
	{
		ItemCounter = StoredItems.size() - 1;
		Offset = ceil(StoredItems.size() / 20.0) - 1;
	}
}

void StorageScreen::EmptyStorageStructs()
{
	if (!StoredItems.empty())
	{
		for (StoredItem* pItem : StoredItems)
		{
			delete pItem;
		}
		StoredItems.clear();
	}
}

void StorageScreen::PollInput()
{
	sf::Event event;

	if (pCurrentOverlay)
	{
		pCurrentOverlay->PollInput();
	}
	else
	{
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
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
					ScreenManager::GetInstance()->CloseScreen();
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					if(StoredItems.size() > 0)
					{
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
						pItemActionMenu->SetItem(ItemManager::GetInstance()->FindItemInStorage(StoredItems.at(ItemCounter)->ItemID));
						pItemActionMenu->Load();

						pCurrentOverlay = pItemActionMenu;
					}
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					if (ItemCounter - (20 * Offset) + 1 < 20)
					{
						if (ItemCounter + 1 < StoredItems.size())
						{
							++ItemCounter;
							UpdateCursorPosition();
							SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
						}
					}
					else if (CanMoveDown)
					{
						++Offset;
						++ItemCounter;
						UpdateCursorPosition();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);

						CanMoveUp = true;
						if (StoredItems.size() > ItemCounter + (20 * Offset))
						{
							CanMoveDown = true;
						}
						else
						{
							CanMoveDown = false;
						}
					}
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					if (ItemCounter - (20 * Offset) > 0)
					{
						--ItemCounter;
						UpdateCursorPosition();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					}
					else if (CanMoveUp)
					{
						--Offset;
						--ItemCounter;
						UpdateCursorPosition();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);

						if (Offset == 0)
						{
							CanMoveUp = false;
						}
						if (StoredItems.size() > ItemCounter + (20 * Offset))
						{
							CanMoveDown = true;
						}
						else
						{
							CanMoveDown = false;
						}
					}
				}
			}
		}
	}
}
