#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class TextContainer
{
public:
	void Draw();

	sf::Text* pCurrentText;
	sf::Font* pFont;
	int ContainerWidth;

	TextContainer(sf::RenderWindow* _pRenderWindow, std::string _RawText, sf::Font* _pFont, sf::Vector2f _Pos, int _Width);
	~TextContainer();

	std::string WordWrap(std::string _String, int _Width);
	void SetText(std::string _String, bool _isPreWrapped);

private:
	sf::RenderWindow* pRenderWindow;
};

