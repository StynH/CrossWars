#include "TextContainer.h"
#include <iostream>


void TextContainer::Draw()
{
	pRenderWindow->draw(*pCurrentText);
}

TextContainer::TextContainer(sf::RenderWindow* _pRenderWindow, std::string _RawText, sf::Font* _pFont, sf::Vector2f _Pos, int _Width)
{
	pRenderWindow = _pRenderWindow;
	pCurrentText = new sf::Text();
	pCurrentText->setFont(*_pFont);
	pCurrentText->setStyle(sf::Text::Bold);
	pCurrentText->setCharacterSize(18);
	pCurrentText->setPosition(_Pos);
	pCurrentText->setString(WordWrap(_RawText, _Width));

	ContainerWidth = _Width;
}

std::string TextContainer::WordWrap(std::string _String, int _Width)
{
	std::string NewString = "";

	int CurrentWidth = 0;
	int ReductionWidth = 0;
	int Counter = -1;
	int LastWhitespace = 0;

	for (char& c : _String)
	{
		pCurrentText->setString(NewString);

		CurrentWidth = pCurrentText->getLocalBounds().width;

		if(CurrentWidth > _Width)
		{
			NewString.at(LastWhitespace) = '\n';
			ReductionWidth = CurrentWidth;
		}

		NewString += c;

		++Counter;

		if (isspace(c))
		{
			LastWhitespace = Counter;
		}
	}

	return NewString;
}

void TextContainer::SetText(std::string _String, bool _isPreWrapped)
{
	if(!_isPreWrapped)
	{
		pCurrentText->setString(WordWrap(_String, ContainerWidth));
	}
	else
	{
		pCurrentText->setString(_String);
	}
	
}

TextContainer::~TextContainer()
{

}
