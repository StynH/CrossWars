#include "TextureManager.h"
#include <SFML/Graphics/Texture.hpp>
#include "UnitMenuOverlay.h"
#include "Cursor.h"
#include "LevelScreen.h"
#include "ScreenManager.h"
#include "MusicPlayer.h"

UnitMenuOverlay::UnitMenuOverlay(sf::RenderWindow* _pRenderWindow, Cursor* _pCursor, LevelScreen* _pLevelScreen) : GameScreenOverlay(_pRenderWindow)
{
	sf::Texture* MenuTexture = TextureManager::GetInstance()->LoadTexture("unitaction.png", Folders::FolderType::MENU);

	pAttackButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 0, 177, 34)), pWindow, sf::Vector2f(326, 112));
	pInventoryButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 34, 177, 34)), pWindow, sf::Vector2f(326, 146));
	pWaitButton = new UIButton(new sf::Sprite(*MenuTexture, sf::IntRect(0, 68, 177, 34)), pWindow, sf::Vector2f(326, 180));

	pWaitButton->Fire += std::bind(&UnitMenuOverlay::UnitWait, this);

	pAttackButton->Fire += std::bind(&LevelScreen::PrepareAttack, _pLevelScreen);
	pAttackButton->Fire += std::bind(&UnitMenuOverlay::Hide, this);

	pInventoryButton->Fire += std::bind(&UnitMenuOverlay::ShowInventoryScreen, this);

	pCursorSprite = new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("mapicons//menuselector.png", Folders::FolderType::BATTLE));

	pCursor = _pCursor;

	pLevelScreen = _pLevelScreen;
}


UnitMenuOverlay::~UnitMenuOverlay()
{
	SafeDelete(pAttackButton);
	SafeDelete(pInventoryButton);
	SafeDelete(pWaitButton);
	SafeDelete(pCursorSprite);
}

void UnitMenuOverlay::Draw()
{
	if(CanAttack)
	{
		pAttackButton->Draw();
	}
	
	pInventoryButton->Draw();
	pWaitButton->Draw();

	pWindow->draw(*pCursorSprite);
}

void UnitMenuOverlay::UpdateCursorPosition()
{
	pCursorSprite->setPosition(ButtonList.Front()->GetPosition());
}

void UnitMenuOverlay::Update()
{
	
}

void UnitMenuOverlay::Load()
{
	CloseScreen = false;
	IsHiding = false;
	CanAttack = false;

	//Only if he can attack, will we show the attack button.
	if(pCursor->GetSelectedUnit()->getItemFromInventory(0) != nullptr && pCursor->GetSelectedUnit()->getItemFromInventory(0)->getType() == Item::WEAPON)
	{
		ButtonList.Insert(pAttackButton);
		CanAttack = true;
	}

	ButtonList.Insert(pInventoryButton);
	ButtonList.Insert(pWaitButton);

	UpdateCursorPosition();
}

void UnitMenuOverlay::UnitWait()
{
	pCursor->GetSelectedUnit()->DeactivateUnit();
	pCursor->DeselectUnit();
	CloseScreen = true;
}

void UnitMenuOverlay::ShowInventoryScreen()
{
	pLevelScreen->ShowInventoryScreen(pCursor->GetSelectedUnit());
}

void UnitMenuOverlay::Unload()
{
	ButtonList.Clear();
}

void UnitMenuOverlay::Hide()
{
	IsHiding = true;
	MustUnload = true;
}

void UnitMenuOverlay::PollInput()
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
				ButtonList.BackToFront();
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				UpdateCursorPosition();
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				ButtonList.FrontToBack();
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CursorSound, false);
				UpdateCursorPosition();
			}
			else if (event.key.code == sf::Keyboard::Z)
			{
				ButtonList.Front()->Fire(nullptr);
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->SelectSound, false);
			}
			else if (event.key.code == sf::Keyboard::X)
			{
				pCursor->GetSelectedUnit()->RevertPosition();
				pCursor->DeselectUnit();
				CloseScreen = true;
				SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->CancelSound, false);
			}
		}
	}
}
