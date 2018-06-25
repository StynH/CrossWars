#include "ItemCreationScreen.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "TextSanitizer.h"
#include "MusicPlayer.h"
#include "ScreenManager.h"
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/random/mersenne_twister.hpp"

ItemCreationScreen::ItemCreationScreen(sf::RenderWindow* _pRenderWindow) : GameScreen(_pRenderWindow)
{
	pBackground = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("itemcreation\\background.png", Folders::HUD));

	pContainer = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("itemcreation\\container.png", Folders::HUD));
	pContainer->setPosition(222, 68);

	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("itemcreation\\itemcursor.png", Folders::HUD));

	pEnergyText = new sf::Text();
	pEnergyText->setFont(*ResourceManager::GetInstance()->GetCodersCruxFont());
	pEnergyText->setCharacterSize(32);
	pEnergyText->setColor(sf::Color::White);

	sf::Texture* pScrollTexture = TextureManager::GetInstance()->LoadTexture("scrollbuttons.png", Folders::MENU);
	pScrollDown = new sf::Sprite(*pScrollTexture, sf::IntRect(0, 0, 16, 16));
	pScrollDown->setPosition(238, 292);
	pScrollUp = new sf::Sprite(*pScrollTexture, sf::IntRect(16, 0, 16, 16));
	pScrollUp->setPosition(238, 239);

	pSpeechBubble = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("itemcreation\\speechbubble.png", Folders::HUD));
	pSpeechBubble->setPosition(4, 70);

	pTypeWriter = new TypeWriter(16, 50, ResourceManager::GetInstance()->GetAceFont());
	pTypeWriter->SetPosition(sf::Vector2f(19, 85));

	MustUnload = false;
}

ItemCreationScreen::~ItemCreationScreen()
{
}

void ItemCreationScreen::Start()
{
	UpdateCounter();

	pTypeWriter->SetText(GetSombraText());

	if(CreateableItems.size() > 9)
	{
		CanMoveDown = true;
	}

	ItemCounter = 0;
	Offset = 0;

	UpdateCursorPosition();
}

void ItemCreationScreen::Update()
{
	pTypeWriter->Update();
}


void ItemCreationScreen::Draw()
{
	pWindow->draw(*pBackground);
	pWindow->draw(*pContainer);

	pWindow->draw(*pSpeechBubble);
	pWindow->draw(**pTypeWriter);

	pWindow->draw(*pEnergyText);

	int Start = 9 * Offset;
	for (int i = Start; i < Start + 9; ++i)
	{
		if (CreateableItems.size() < i + 1)
		{
			break;
		}

		CreatableItem* pItem = CreateableItems.at(i);
		pWindow->draw(*pItem->pContainerSprite);
		pWindow->draw(*pItem->pItemSprite);
		pWindow->draw(*pItem->pItemText);
		pWindow->draw(*pItem->pItemPrice);
	}

	if(CanMoveUp)
	{
		pWindow->draw(*pScrollUp);
	}

	if (CanMoveDown)
	{
		pWindow->draw(*pScrollDown);
	}

	pWindow->draw(*pCursorSprite);
}

void ItemCreationScreen::Unload()
{

}

void ItemCreationScreen::UpdateCounter()
{
	pEnergyText->setString(std::to_string(PlayerManager::GetInstance()->GetEnergyUnits()) + "e");
	TextSanitizer::CenterOrigin(pEnergyText);
	pEnergyText->setPosition(42, 494);
}

void ItemCreationScreen::UpdateCursorPosition()
{
	pCursorSprite->setPosition(276, 95 + (40 * (ItemCounter - (Offset * 9))));
}

void ItemCreationScreen::AddItem(std::string _ItemID)
{
	int Counter = CreateableItems.size() % 9;
	sf::Font* pFont = ResourceManager::GetInstance()->GetAceFont();

	Item* pItem = ItemManager::GetInstance()->GetItemTemplate(_ItemID);

	CreatableItem* NewItem = new CreatableItem();

	NewItem->pContainerSprite = std::make_shared<sf::Sprite>(*TextureManager::GetInstance()->LoadTexture("itemcreation\\item_slot.png", Folders::HUD));
	NewItem->pContainerSprite->setPosition(276, 100 + (40 * Counter));

	NewItem->pItemText = std::make_shared<sf::Text>();
	NewItem->pItemText->setString(pItem->getName());
	NewItem->pItemText->setCharacterSize(16);
	NewItem->pItemText->setFont(*pFont);
	TextSanitizer::CenterOriginHorizontallyLeft(NewItem->pItemText.get());
	NewItem->pItemText->setPosition(302, 112 + (40 * Counter));

	NewItem->pItemPrice = std::make_shared<sf::Text>();
	NewItem->pItemPrice->setString(std::to_string(pItem->getPrice()) + "e");
	NewItem->pItemPrice->setCharacterSize(16);
	NewItem->pItemPrice->setFont(*pFont);
	TextSanitizer::CenterOriginHorizontallyRight(NewItem->pItemPrice.get());
	NewItem->pItemPrice->setPosition(464, 112 + (40 * Counter));

	NewItem->pItemSprite = std::make_shared<sf::Sprite>(*pItem->getTexture());
	NewItem->pItemSprite->setPosition(280, 104 + (40 * Counter));

	NewItem->ItemID = _ItemID;
	NewItem->Price = pItem->getPrice();

	CreateableItems.push_back(NewItem);

	LoadSombraText();
}

const std::vector<CreatableItem*>& ItemCreationScreen::GetCreateableItems()
{
	return CreateableItems;
}

bool ItemCreationScreen::CanCreateItem(CreatableItem* pItem) 
{
	return PlayerManager::GetInstance()->GetEnergyUnits() >= pItem->Price;
}

void ItemCreationScreen::CreateItem(std::string _ItemID)
{
	Item* pItem = ItemManager::GetInstance()->GetItem(_ItemID);
	ItemManager::GetInstance()->AddItemToStorage(pItem);

	SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->RecieveSound, false);

	PlayerManager::GetInstance()->DeductEnergyUnits(pItem->getPrice());
	UpdateCounter();
}

void ItemCreationScreen::LoadSombraText()
{
	std::ifstream DataFile(Folders::ROOT_FOLDER + Folders::HUD_FOLDER + "itemcreation\\sombralines.json");
	nlohmann::json ParsedJson = nlohmann::json::parse(DataFile);
	SombraMorningText = ParsedJson["morning"].get<std::vector<std::string>>();
	SombraAfternoonText = ParsedJson["afternoon"].get<std::vector<std::string>>();
	SombraEveningText = ParsedJson["evening"].get<std::vector<std::string>>();
	SombraNightText = ParsedJson["night"].get<std::vector<std::string>>();
}

std::string ItemCreationScreen::GetSombraText()
{
	int CurrenTime = atoi(to_iso_string(boost::posix_time::second_clock::local_time().time_of_day()).c_str());
	std::vector<std::string> TextVector;

	if(CurrenTime >= 70000 && CurrenTime <= 120000)
	{
		TextVector = SombraMorningText;
	}
	else if(CurrenTime > 120000 && CurrenTime <= 180000)
	{
		TextVector = SombraAfternoonText;
	}
	else if (CurrenTime > 180000 && CurrenTime <= 235959)
	{
		TextVector = SombraEveningText;
	}
	else
	{
		TextVector = SombraNightText;
	}

	boost::random::mt19937 RNG;
	RNG.seed(static_cast<unsigned int>(time(0)));
	boost::random::uniform_int_distribution<> Index(0, TextVector.size() - 1);

	return TextSanitizer::SanitizeString(TextVector.at(Index(RNG)));
}

void ItemCreationScreen::PollInput()
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
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
				ScreenManager::GetInstance()->CloseScreen();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				if (ItemCounter - (9 * Offset) + 1 < 9)
				{
					if (ItemCounter + 1 < CreateableItems.size())
					{
						++ItemCounter;
						UpdateCursorPosition();
						SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
					}
				}
				else
				{
					++Offset;
					++ItemCounter;
					UpdateCursorPosition();
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);

					CanMoveUp = true;
					if(CreateableItems.size() > ItemCounter + (9 * Offset))
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
				if (ItemCounter - (9 * Offset) > 0)
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
					if (CreateableItems.size() > ItemCounter + (9 * Offset))
					{
						CanMoveDown = true;
					}
					else
					{
						CanMoveDown = false;
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				if (CanCreateItem(CreateableItems.at(ItemCounter)))
				{
					CreateItem(CreateableItems.at(ItemCounter)->ItemID);
				}
				else
				{
					SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->ErrorSound, false);
				}
			}
		}
	}
}
