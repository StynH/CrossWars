#include "ItemTradeOverlay.h"
#include "TextureManager.h"
#include "MusicPlayer.h"
#include "ResourceManager.h"

ItemTradeOverlay::ItemTradeOverlay(sf::RenderWindow* _pRenderWindow) : GameScreenOverlay(_pRenderWindow)
{
	sf::Texture* TradeContainer = TextureManager::GetInstance()->LoadTexture("chardisplay//inv_bg.png", Folders::FolderType::HUD);

	ItemContainerOne = std::make_shared<sf::Sprite>(*TradeContainer);
	ItemContainerOne->setPosition(16, 54);

	ItemContainerTwo = std::make_shared<sf::Sprite>(*TradeContainer);
	ItemContainerTwo->setPosition(362, 54);

	TradeArrows = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("chardisplay//arrows.png", Folders::FolderType::HUD));
	TradeArrows->setPosition(197, 73);

	pInvslotSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("chardisplay\\inv_slot.png", Folders::FolderType::HUD));
	pInvslotActiveSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("chardisplay\\inv_slot_selected.png", Folders::FolderType::HUD));

	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("overview\\itemcursor.png", Folders::HUD));

	DeselectUnitAfterClose = true;
	InPlayerTwoInventory = false;

	UpdateCursor();
}


ItemTradeOverlay::~ItemTradeOverlay()
{
}

void ItemTradeOverlay::Update()
{
}

void ItemTradeOverlay::Unload()
{
}

void ItemTradeOverlay::Hide()
{
	CloseScreen = true;
}

void ItemTradeOverlay::PollInput()
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
			if (event.key.code == sf::Keyboard::Up)
			{
				if(Counter > 0)
				{
					Counter -= 1;
					UpdateCursor();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
			}
			else if (event.key.code == sf::Keyboard::Right)
			{
				if(!InPlayerTwoInventory)
				{
					InPlayerTwoInventory = true;
					UpdateCursor();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				if(Counter < 4)
				{			
					Counter += 1;
					UpdateCursor();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
			}
			else if (event.key.code == sf::Keyboard::Left)
			{
				if (InPlayerTwoInventory)
				{
					InPlayerTwoInventory = false;
					UpdateCursor();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				}
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				if(!pSelectedItem)
				{
					if(!InPlayerTwoInventory)
					{
						if(Counter + 1 <= InventoryOne.size())
						{
							pSelectedItem = InventoryOne.at(Counter);
							pSelectedItem->IsSelected = true;
							SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);

							SelectedItemInPlayerTwoInventory = false;

							SelectedItemIndex = Counter;
						}
					}
					else
					{
						if (Counter + 1 <= InventoryTwo.size())
						{
							pSelectedItem = InventoryTwo.at(Counter);
							pSelectedItem->IsSelected = true;
							SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);

							SelectedItemInPlayerTwoInventory = true;

							SelectedItemIndex = Counter;
						}
					}
				}
				else
				{
					if(InPlayerTwoInventory && SelectedItemInPlayerTwoInventory)
					{
						if (Counter + 1 <= InventoryTwo.size())
						{
							if (pSelectedItem->GUID == InventoryTwo.at(Counter)->GUID)
							{
								pSelectedItem->IsSelected = false;
								pSelectedItem = nullptr;
							}
							else
							{
								SwapItem(pUnitTwo);
							}
						}
					}
					else if (!InPlayerTwoInventory && !SelectedItemInPlayerTwoInventory)
					{
						if (Counter + 1 <= InventoryOne.size())
						{
							if (pSelectedItem->GUID == InventoryOne.at(Counter)->GUID)
							{
								pSelectedItem->IsSelected = false;
								pSelectedItem = nullptr;
							}
							else
							{
								SwapItem(pUnitOne);
							}
						}
					}
					else if (InPlayerTwoInventory && !SelectedItemInPlayerTwoInventory)
					{
						if (Counter + 1 <= InventoryTwo.size())
						{
							SwapBetweenUnits(SelectedItemIndex, Counter);
						}
						else
						{
							Item* pItem = pUnitOne->getPointerToInventory()->at(SelectedItemIndex);
							pUnitOne->getPointerToInventory()->erase(pUnitOne->getPointerToInventory()->begin() + SelectedItemIndex);
							pUnitTwo->getPointerToInventory()->push_back(pItem);
							SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
							Reset();
							InitInventories();
						}
					}
					else if (!InPlayerTwoInventory && SelectedItemInPlayerTwoInventory)
					{
						if (Counter + 1 <= InventoryOne.size())
						{
							SwapBetweenUnits(Counter, SelectedItemIndex);
						}
						else
						{
							Item* pItem = pUnitTwo->getPointerToInventory()->at(SelectedItemIndex);
							pUnitTwo->getPointerToInventory()->erase(pUnitTwo->getPointerToInventory()->begin() + SelectedItemIndex);
							pUnitOne->getPointerToInventory()->push_back(pItem);
							SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
							Reset();
							InitInventories();
						}
					}
				}
			}
			else if (event.key.code == sf::Keyboard::X)
			{
				if(pSelectedItem)
				{
					Reset();
				}
				else
				{
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
					Hide();
				}
			}
		}
	}
}

void ItemTradeOverlay::UpdateCursor()
{
	if(!InPlayerTwoInventory)
	{
		pCursorSprite->setPosition(28, 75 + (28 * Counter));
	}
	else
	{
		pCursorSprite->setPosition(374, 75 + (28 * Counter));
	}
}

void ItemTradeOverlay::Load()
{
	Counter = 0;
	SelectedItemIndex = 0;
	CloseScreen = false;
	InPlayerTwoInventory = false;

	pSelectedItem = nullptr;
	SelectedItemInPlayerTwoInventory = false;

	InitInventories();
	UpdateCursor();
}

void ItemTradeOverlay::SetTraders(Unit* _pUnitOne, Unit* _pUnitTwo)
{
	pUnitOne = _pUnitOne;
	pUnitTwo = _pUnitTwo;
}

void ItemTradeOverlay::InitInventories()
{
	EmptyInventories();

	sf::Font* pFont = ResourceManager::GetInstance()->GetCodersCruxFont();

	for (auto i = 0; i < pUnitOne->getInventory().size(); ++i)
	{
		TradeableItem* InvItem = new TradeableItem;

		int ItemY = 85 + (28 * i);
		int SpriteY = 85 + (28 * i);
		Item* CurrentItem = pUnitOne->getItemFromInventory(i);

		std::shared_ptr<sf::Text> InventoryTitle = std::make_shared<sf::Text>();
		InventoryTitle->setFont(*pFont);
		InventoryTitle->setColor(sf::Color::Black);
		InventoryTitle->setCharacterSize(14);
		InventoryTitle->setString(CurrentItem->getName());
		InventoryTitle->setOrigin(InventoryTitle->getLocalBounds().left, static_cast<int>(InventoryTitle->getLocalBounds().height / 2.0f));
		InventoryTitle->setPosition(52, ItemY);

		InvItem->Sprite = std::make_shared<sf::Sprite>(*CurrentItem->getTexture());
		InvItem->Sprite->setPosition(32, SpriteY);
		InvItem->Title = InventoryTitle;
		InvItem->IsSelected = false;
		InvItem->GUID = CurrentItem->getGUID();

		InventoryOne.push_back(InvItem);
	}

	for (auto i = 0; i < pUnitTwo->getInventory().size(); ++i)
	{
		TradeableItem* InvItem = new TradeableItem;

		int ItemY = 85 + (28 * i);
		int SpriteY = 85 + (28 * i);
		Item* CurrentItem = pUnitTwo->getItemFromInventory(i);

		std::shared_ptr<sf::Text> InventoryTitle = std::make_shared<sf::Text>();
		InventoryTitle->setFont(*pFont);
		InventoryTitle->setColor(sf::Color::Black);
		InventoryTitle->setCharacterSize(14);
		InventoryTitle->setString(CurrentItem->getName());
		InventoryTitle->setOrigin(InventoryTitle->getLocalBounds().left, static_cast<int>(InventoryTitle->getLocalBounds().height / 2.0f));
		InventoryTitle->setPosition(398, ItemY);

		InvItem->Sprite = std::make_shared<sf::Sprite>(*CurrentItem->getTexture());
		InvItem->Sprite->setPosition(378, SpriteY);
		InvItem->Title = InventoryTitle;
		InvItem->IsSelected = false;
		InvItem->GUID = CurrentItem->getGUID();

		InventoryTwo.push_back(InvItem);
	}
}

void ItemTradeOverlay::DrawInventories()
{
	int PosX;
	int PosY = 80;

	for (auto i = 0; i < 5; ++i)
	{
		PosX = 28;
		if(InventoryOne.size() > i)
		{
			if(InventoryOne.at(i)->IsSelected)
			{
				pInvslotActiveSprite->setPosition(PosX, PosY);
				pWindow->draw(*pInvslotActiveSprite);
			}
			else
			{
				pInvslotSprite->setPosition(PosX, PosY);
				pWindow->draw(*pInvslotSprite);
			}
			pWindow->draw(*InventoryOne.at(i)->Sprite);
			pWindow->draw(*InventoryOne.at(i)->Title);
		}
		else
		{
			pInvslotSprite->setPosition(PosX, PosY);
			pWindow->draw(*pInvslotSprite);
		}

		PosX = 374;
		if (InventoryTwo.size() > i)
		{
			if (InventoryTwo.at(i)->IsSelected)
			{
				pInvslotActiveSprite->setPosition(PosX, PosY);
				pWindow->draw(*pInvslotActiveSprite);
			}
			else
			{
				pInvslotSprite->setPosition(PosX, PosY);
				pWindow->draw(*pInvslotSprite);
			}
			pWindow->draw(*InventoryTwo.at(i)->Sprite);
			pWindow->draw(*InventoryTwo.at(i)->Title);
		}
		else
		{
			pInvslotSprite->setPosition(PosX, PosY);
			pWindow->draw(*pInvslotSprite);
		}

		PosY += 28;
	}

	pWindow->draw(*pCursorSprite);
}

void ItemTradeOverlay::EmptyInventories()
{
	for (TradeableItem* InventoryItem : InventoryOne)
	{
		delete InventoryItem;
	}

	for (TradeableItem* InventoryItem : InventoryTwo)
	{
		delete InventoryItem;
	}

	InventoryOne.clear();
	InventoryTwo.clear();
}

void ItemTradeOverlay::Draw()
{
	pWindow->draw(*pUnitOne->getCHAR()->DialougeSprite->Normal);
	pWindow->draw(*ItemContainerOne);

	pWindow->draw(*pUnitTwo->getCHAR()->DialougeSprite->Reversed);
	pWindow->draw(*ItemContainerTwo);

	DrawInventories();

	pWindow->draw(*TradeArrows);
}

void ItemTradeOverlay::SwapItem(Unit* _pUnit)
{
	std::vector<Item*>* Inventory = _pUnit->getPointerToInventory();
	iter_swap(Inventory->begin() + SelectedItemIndex, Inventory->begin() + Counter);

	Reset();

	InitInventories();
	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
}

void ItemTradeOverlay::SwapBetweenUnits(int _FirstIndex, int _SecondIndex)
{
	std::swap(pUnitOne->getPointerToInventory()->at(_FirstIndex), pUnitTwo->getPointerToInventory()->at(_SecondIndex));
	Reset();
	InitInventories();
	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
}

void ItemTradeOverlay::Reset()
{
	SelectedItemIndex = 0;
	CloseScreen = false;
	if(pSelectedItem)
	{
		pSelectedItem->IsSelected = false;
	}
	pSelectedItem = nullptr;
	SelectedItemInPlayerTwoInventory = false;
}