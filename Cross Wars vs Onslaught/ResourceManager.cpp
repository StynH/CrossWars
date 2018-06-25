#include "ResourceManager.h"
#include "TextureManager.h"
#include <boost/filesystem/string_file.hpp>

ResourceManager::ResourceManager(sf::RenderWindow* _pWindow)
{
	pWindow = _pWindow;	
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadResources()
{
	boost::filesystem::path Path(Folders::ROOT_FOLDER + Folders::BATTLE_FOLDER + "backgrounds");
	boost::filesystem::directory_iterator EndIterator;
	for (boost::filesystem::directory_iterator itr(Path); itr != EndIterator; ++itr)
	{
		if (is_regular_file(itr->path())) {
			std::string Name = itr->path().stem().string();
			AddPlatform(Name, new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("battleplatform\\" + Name + ".png", Folders::FolderType::BATTLE)));
			AddBackground(Name, new sf::Sprite(*TextureManager::GetInstance()->LoadTexture("backgrounds\\" + Name + ".png", Folders::FolderType::BATTLE)));
		}
	}

	std::cout << "All scenes loaded!" << std::endl;

	GetInstance()->AceFont = new sf::Font;
	if (!GetInstance()->AceFont->loadFromFile(Folders::ROOT_FOLDER + Folders::DIALOGUE_FOLDER + "Ace-Attorney.ttf"))
	{
		std::cout << "Failed to load AceFont font!" << std::endl;
	}

	GetInstance()->CodersCrux = new sf::Font;
	if (!GetInstance()->CodersCrux->loadFromFile(Folders::ROOT_FOLDER + Folders::DIALOGUE_FOLDER + "coders_crux.ttf"))
	{
		std::cout << "Failed to load CodersCrux font!" << std::endl;
	}

	GetInstance()->pBlackBackground = TextureManager::GetInstance()->LoadTexture("black.png", Folders::MENU);
}

void ResourceManager::AddPlatform(std::string _id, sf::Sprite* _sprite)
{
	GetInstance()->PlatformList.insert({ _id, _sprite });
}

sf::Sprite* ResourceManager::GetPlatform(std::string _ID)
{
	auto search = GetInstance()->PlatformList.find(_ID);
	if (search != GetInstance()->PlatformList.end()) {
		return (sf::Sprite*)search->second;
	}
	return GetInstance()->PlatformList["unknown"];
}

void ResourceManager::AddBackground(std::string _id, sf::Sprite* _sprite)
{
	GetInstance()->BackgroundList.insert({ _id, _sprite });
}

sf::Sprite* ResourceManager::GetBackground(std::string _ID)
{
	auto search = GetInstance()->BackgroundList.find(_ID);
	if (search != GetInstance()->BackgroundList.end()) {
		return (sf::Sprite*)search->second;
	}
	return GetInstance()->BackgroundList["unknown"];
}

sf::RenderWindow* ResourceManager::pWindow = nullptr;
