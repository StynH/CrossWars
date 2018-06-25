#include "TypeWriter.h"
#include "GameTime.h"
#include "MusicPlayer.h"

TypeWriter::TypeWriter(int _FontSize, int _TextSpeed, sf::Font* _pFont)
{
	TextSpeed = _TextSpeed;

	pText = new sf::Text();
	pText->setFont(*_pFont);
	pText->setCharacterSize(_FontSize);

	TextDone = false;
	SceneTimer = 0;
	PreviousTextLength = 0;
}

TypeWriter::~TypeWriter()
{
}

const sf::Text& TypeWriter::operator*()
{
	return *pText;
}

void TypeWriter::Reset()
{
	SceneTimer = 0;
	PreviousTextLength = 0;
	TextDone = false;
}

void TypeWriter::Update()
{
	SceneTimer += (TextSpeed * GameTime::DeltaTime);
	CurrentText = TextString.substr(0, SceneTimer);

	if (CurrentText.length() > PreviousTextLength + 2 && !TextDone)
	{
		PreviousTextLength = CurrentText.length();
		SoundPlayer::GetInstance()->PlaySoundFromBuffer(SoundPlayer::GetInstance()->TalkSound, false);
	}

	if (CurrentText.size() == TextString.size() && !TextDone) {
		TextDone = true;
	}

	pText->setString(CurrentText);
}

void TypeWriter::SetPosition(sf::Vector2f _Position)
{
	pText->setPosition(_Position);
}

void TypeWriter::SetText(std::string _String)
{
	TextString = _String;
	pText->setString(_String);
	Reset();
}
