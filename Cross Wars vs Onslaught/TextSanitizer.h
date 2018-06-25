#pragma once
#include <string>
#include <algorithm>
#include <SFML/Graphics/Text.hpp>

class TextSanitizer
{
public:
	
	static std::string SanitizeString(std::string _string);
	static std::string ReplaceWord(std::string _haystack, std::string _needle, std::string _replacement);
	static void CenterOrigin(sf::Text* _pText);
	static void CenterOriginHorizontally(sf::Text* _pText);
	static void CenterOriginHorizontallyRight(sf::Text* _pText);
	static void CenterOriginHorizontallyLeft(sf::Text* _pText);
	static void CenterLeftAndBottom(sf::Text* _pText);
	static void CenterLeftAndTop(sf::Text* _pText);
	static bool InvalidChar(char _Char);
};

