#pragma once
#include "Singleton.h"
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>
#include <json.hpp>
#include "ItemManager.h"

namespace sf{
	class RenderWindow;
}

class ResourceManager: public TSingleton<ResourceManager>
{
public:
	ResourceManager(){};
	ResourceManager(sf::RenderWindow* _pWindow);
	~ResourceManager();

	//Getters
	static sf::RenderWindow* GetWindow() { return pWindow; }

	void LoadResources();

	void AddPlatform(std::string _id, sf::Sprite* _sprite);
	sf::Sprite* GetPlatform(std::string _ID);

	void AddBackground(std::string _id, sf::Sprite* _sprite);
	sf::Sprite* GetBackground(std::string _ID);

	sf::Font* GetAceFont() { return AceFont; }
	sf::Font* GetCodersCruxFont() { return CodersCrux; }

	sf::Texture* GetBlackBackground() { return pBlackBackground; }
private:
	static sf::RenderWindow* pWindow;

	std::unordered_map<std::string, sf::Sprite*> PlatformList;
	std::unordered_map<std::string, sf::Sprite*> BackgroundList;

	sf::Texture* pBlackBackground;

	sf::Font* AceFont;
	sf::Font* CodersCrux;
};

