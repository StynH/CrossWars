#pragma once
#include <string>
#include "Unit.h"
#include "Singleton.h"
#include "Folders.h"

namespace sf{
	class Texture;
}

class TextureManager: public TSingleton<TextureManager> 
{
public:
	TextureManager();
	~TextureManager();

	sf::Texture* LoadTexture(std::string _fileName, Folders::FolderType _type);
	sf::Sprite* GetItemTexture(int _x, int _y);
	sf::Sprite* GetSkillTexture(int _x, int _y);

	//Asset root folders
	static const std::string ITEM_SPREADSHEET;
	static const std::string SKILL_SPREADSHEET;

	sf::Texture* ItemSheet;
	sf::Texture* SkillSheet;

private:
	std::map<std::string, sf::Texture*> TextureLibrary;
};

