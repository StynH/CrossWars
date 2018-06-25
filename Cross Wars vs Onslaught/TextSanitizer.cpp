#include "TextSanitizer.h"
#include "PlayerManager.h"

std::string TextSanitizer::SanitizeString(std::string _string)
{
	std::string SanitizedString;

	SanitizedString = ReplaceWord(_string, "$player", PlayerManager::GetInstance()->GetPlayerName());
	SanitizedString.erase(remove_if(SanitizedString.begin(), SanitizedString.end(), InvalidChar), SanitizedString.end());

	return SanitizedString;
}

std::string TextSanitizer::ReplaceWord(std::string _haystack, std::string _needle, std::string _replacement)
{
	while (_haystack.find(_needle) != std::string::npos)
	{
		_haystack.replace(_haystack.find(_needle), _needle.length(), _replacement);
	}

	return _haystack;
}

void TextSanitizer::CenterOrigin(sf::Text* _pText)
{
	sf::FloatRect textRect = _pText->getLocalBounds();
	_pText->setOrigin(static_cast<int>(textRect.left + textRect.width / 2.0f), static_cast<int>(textRect.top + textRect.height / 2.0f));
}

void TextSanitizer::CenterOriginHorizontally(sf::Text* _pText)
{
	sf::FloatRect textRect = _pText->getLocalBounds();
	_pText->setOrigin(textRect.left, static_cast<int>(textRect.top + textRect.height / 2.0f));
}

void TextSanitizer::CenterOriginHorizontallyRight(sf::Text* _pText)
{
	sf::FloatRect textRect = _pText->getLocalBounds();
	_pText->setOrigin(textRect.left + textRect.width, static_cast<int>(textRect.top + textRect.height / 2.0f));
}

void TextSanitizer::CenterOriginHorizontallyLeft(sf::Text* _pText)
{
	sf::FloatRect textRect = _pText->getLocalBounds();
	_pText->setOrigin(textRect.left, static_cast<int>(textRect.top + textRect.height / 2.0f));
}


void TextSanitizer::CenterLeftAndBottom(sf::Text* _pText)
{
	sf::FloatRect textRect = _pText->getLocalBounds();
	_pText->setOrigin(textRect.left, textRect.top + textRect.height);
}

void TextSanitizer::CenterLeftAndTop(sf::Text* _pText)
{
	sf::FloatRect textRect = _pText->getLocalBounds();
	_pText->setOrigin(textRect.left, textRect.top);
}

bool TextSanitizer::InvalidChar(char _Char)
{
	return !(_Char >= 0 && _Char <128);
}