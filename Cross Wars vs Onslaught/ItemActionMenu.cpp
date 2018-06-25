#include "ItemActionMenu.h"
#include "UnitSelectScreen.h"
#include "MusicPlayer.h"
#include "ItemManager.h"

ItemActionMenu::ItemActionMenu(sf::RenderWindow* _pRenderWindow, STATE _State) : GameScreenOverlay(_pRenderWindow)
{
	sf::Texture* pMenuTexture = TextureManager::GetInstance()->LoadTexture("storage\\itemmenu.png", Folders::HUD);

	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons//menuselector.png", Folders::FolderType::BATTLE));

	pUnitSelectScreen = new UnitSelectScreen(_pRenderWindow);
	pUnitSelectScreen->SetCallback<ItemActionMenu>(&ItemActionMenu::SendToUnit, this);

	pUseButton = new UIButton(new sf::Sprite(*pMenuTexture, sf::IntRect(0, 0, 177, 34)), _pRenderWindow, sf::Vector2f(326, 78));
	pUseButton->Fire += std::bind(&ItemActionMenu::Hide, this);
	pUseButton->Available = false;

	pDeleteButton = new UIButton(new sf::Sprite(*pMenuTexture, sf::IntRect(0, 102, 177, 34)), _pRenderWindow, sf::Vector2f(326, 146));
	pDeleteButton->Fire += std::bind(&ItemActionMenu::DeleteItem, this);
	pDeleteButton->Fire += std::bind(&ItemActionMenu::Hide, this);
	pDeleteButton->Available = false;

	ButtonList.Insert(pUseButton);
	
	if(_State == INSTORAGE)
	{
		pSendToUnitButton = new UIButton(new sf::Sprite(*pMenuTexture, sf::IntRect(0, 34, 177, 34)), _pRenderWindow, sf::Vector2f(326, 112));
		pSendToUnitButton->Fire += std::bind(&ItemActionMenu::SelectUnit, this);
		pSendToUnitButton->Available = true;

		ButtonList.Insert(pSendToUnitButton);
	}
	else if(_State == ININVENTORY)
	{
		pStoreInStorageButton = new UIButton(new sf::Sprite(*pMenuTexture, sf::IntRect(0, 68, 177, 34)), _pRenderWindow, sf::Vector2f(326, 112));
		pStoreInStorageButton->Fire += std::bind(&ItemActionMenu::SendItemToStorage, this);
		pStoreInStorageButton->Available = true;  

		ButtonList.Insert(pStoreInStorageButton);
	}

	ButtonList.Insert(pDeleteButton);

	CurrentState = _State;

	UpdateCursorPosition();
}


ItemActionMenu::~ItemActionMenu()
{

}

void ItemActionMenu::Draw()
{
	if(!pUnitSelectScreen->CloseScreen)
	{
		pUnitSelectScreen->Draw();
	}
	else
	{
		for(UIButton* pButton : ButtonList.GetInnerVector())
		{
			if(pButton->Available)
			{
				pButton->Draw();
			}
		}
		pWindow->draw(*pCursorSprite);
	}
}

void ItemActionMenu::Load()
{
	pUnitSelectScreen->Hide();
	CloseScreen = false;

	if(pSelectedItem->getType() == Item::ACTION)
	{
		pUseButton->Available = true;
	}

	if(pSelectedItem->getPrice() != 0)
	{
		pDeleteButton->Available = true;
	}

	while (!ButtonList.Front()->Available)
	{
		ButtonList.BackToFront();
	}

	UpdateCursorPosition();
}

void ItemActionMenu::Update()
{
	if (!pUnitSelectScreen->CloseScreen)
	{
		pUnitSelectScreen->Update();
		if (pUnitSelectScreen->CloseGame)
		{
			CloseGame = true;
		}
	}
}

void ItemActionMenu::UpdateCursorPosition()
{
	pCursorSprite->setPosition(ButtonList.Front()->GetPosition());
}


void ItemActionMenu::Unload()
{
	
}

void ItemActionMenu::Hide()
{
	pSelectedItem = nullptr;
	pSelectedUnit = nullptr;
	CloseScreen = true;

	pUseButton->Available = false;
	pDeleteButton->Available = false;
}

void ItemActionMenu::SetItem(Item* _pItem)
{
	pSelectedItem = _pItem;
}

void ItemActionMenu::SetUnit(Unit* _pUnit)
{
	pSelectedUnit = _pUnit;
}

void ItemActionMenu::SelectUnit()
{
	pUnitSelectScreen->Load();
}

void ItemActionMenu::SendToUnit(Unit* _pUnit)
{
	if(_pUnit->getPointerToInventory()->size() != 5)
	{
		ItemManager::GetInstance()->RemoveItemFromStorage(pSelectedItem, false);
		_pUnit->addItemToInventory(pSelectedItem);
		pOnSendToUnit(nullptr);
		Hide();
	}
	else
	{
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->ErrorSound, false);
		Hide();
	}
}

void ItemActionMenu::SendItemToStorage()
{
	pSelectedUnit->RemoveItemFromInventory(pSelectedItem, false);
	ItemManager::GetInstance()->AddItemToStorage(pSelectedItem);
	Hide();
}

void ItemActionMenu::DeleteItem()
{
	if (CurrentState == INSTORAGE)
	{
		ItemManager::GetInstance()->RemoveItemFromStorage(pSelectedItem, true);
	}
	else if (CurrentState == ININVENTORY)
	{
		pSelectedUnit->RemoveItemFromInventory(pSelectedItem, true);
	}
}

void ItemActionMenu::PollInput()
{
	sf::Event event;

	if(!pUnitSelectScreen->CloseScreen)
	{
		pUnitSelectScreen->PollInput();
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
				if (event.key.code == sf::Keyboard::Up)
				{
					ButtonList.BackToFront();
					while (!ButtonList.Front()->Available)
					{
						ButtonList.BackToFront();
					}
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					UpdateCursorPosition();
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					ButtonList.FrontToBack();
					while (!ButtonList.Front()->Available)
					{
						ButtonList.FrontToBack();
					}
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					UpdateCursorPosition();
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
					ButtonList.Front()->Fire(nullptr);
				}
				else if (event.key.code == sf::Keyboard::X)
				{
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
					Hide();
				}
			}
		}
	}
}
