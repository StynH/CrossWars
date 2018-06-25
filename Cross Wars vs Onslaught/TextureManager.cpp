#include "TextureManager.h"
#include <SFML/Graphics/Texture.hpp>
#include "Unit.h"
#include "Folders.h"
#include "EventLogger.h"

TextureManager::TextureManager()
{
	ItemSheet = LoadTexture(ITEM_SPREADSHEET, Folders::HUD);
	SkillSheet = LoadTexture(SKILL_SPREADSHEET, Folders::HUD);
}


TextureManager::~TextureManager()
{
}

sf::Texture* TextureManager::LoadTexture(std::string _fileName, Folders::FolderType _type)
{
	sf::Texture* pTexture = new sf::Texture();
	std::string File = "";

	switch (_type)
	{
	case Folders::HUD:
		File = Folders::ROOT_FOLDER + Folders::HUD_FOLDER + _fileName;
		break;
	case Folders::LEVELS:
		File = Folders::ROOT_FOLDER + Folders::LEVELS_FOLDER + _fileName;
		break;
	case Folders::BATTLE:
		File = Folders::ROOT_FOLDER + Folders::BATTLE_FOLDER + _fileName;
		break;
	case Folders::DIALOGUE:
		File = Folders::ROOT_FOLDER + Folders::DIALOGUE_FOLDER + _fileName;
		break;
	case Folders::MENU:
		File = Folders::ROOT_FOLDER + Folders::MENU_FOLDER + _fileName;
		break;
	case Folders::INTERMISSION:
		File = Folders::ROOT_FOLDER + Folders::INTERMISSION_FOLDER + _fileName;
		break;
	case Folders::ARENA:
		File = Folders::ROOT_FOLDER + Folders::ARENA_FOLDER + _fileName;
		break;
	default:
		File = Folders::ROOT_FOLDER + _fileName;
		break;
	}

	EventContext EC("Loading Texture: ", &File);

	if(TextureLibrary.find(File) == TextureLibrary.end())
	{
		pTexture->loadFromFile(File);
		TextureLibrary.insert(std::pair<std::string, sf::Texture*>(File, pTexture));
	}
	else
	{
		pTexture = TextureLibrary.at(File);
	}
	
	return pTexture;
}

sf::Sprite* TextureManager::GetItemTexture(int _x, int _y)
{
	EventContext EC("Getting Item Texture at ", &(std::to_string(_x) + "/" + std::to_string(_y)));

	return new sf::Sprite(*ItemSheet, sf::IntRect(_x * 16, _y * 16, 16, 16));
}

sf::Sprite* TextureManager::GetSkillTexture(int _x, int _y)
{
	EventContext EC("Getting Skill Texture at ", &(std::to_string(_x) + "/" + std::to_string(_y)));

	return new sf::Sprite(*SkillSheet, sf::IntRect(_x * 20, _y * 20, 20, 20));
}

const std::string TextureManager::ITEM_SPREADSHEET = "itemsheet.png";
const std::string TextureManager::SKILL_SPREADSHEET = "skillsheet.png";
