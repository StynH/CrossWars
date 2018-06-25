#pragma once
#include <SFML/Graphics/Text.hpp>

class TypeWriter
{
public:
	TypeWriter(int _FontSize, int _TextSpeed, sf::Font* _pFont);
	~TypeWriter();

	const sf::Text& TypeWriter::operator*();

	void Reset();
	void Update();
	
	void SetPosition(sf::Vector2f _Position);
	void SetText(std::string _String);
private:
	sf::Text* pText;
	std::string CurrentText;
	std::string TextString;

	int TextSpeed;
	bool TextDone;

	double SceneTimer = 0;
	int PreviousTextLength = 0;
};

